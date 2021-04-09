#include <iostream>
#include <vector>
#include <ranges>
#include <set>

class Graph {
public:
    using EdgeId = size_t;
    using VertexId = size_t;

    const auto& GetOutgoingEdges(VertexId vertexId) const {
        return adjList_[vertexId];
    }

    auto GetNeighbors(VertexId vertexId) const {
        return std::views::transform(adjList_[vertexId], [this](EdgeId edgeId) {
            return edges_[edgeId].target;
        });
    }

    VertexId NumVertices() const {
        return adjList_.size();
    }

private:
    friend Graph ReadUndirectedGraph();

    VertexId AddVertex() {
        adjList_.emplace_back();
        return adjList_.size() - 1;
    }

    void AddEdge(VertexId from, VertexId to) {
        auto id = edges_.size();
        edges_.push_back({to});
        adjList_[from].push_back(id);
    }

    struct Edge {
        VertexId target;
    };

    std::vector<std::vector<EdgeId>> adjList_;
    std::vector<Edge> edges_;
};

Graph ReadUndirectedGraph() {
    Graph graph;
    int nv, ne;
    std::cin >> nv >> ne;
    for (int i = 0; i < nv; ++i) {
        graph.AddVertex();
    }
    for (int i = 0; i < ne; ++i) {
        int from, to;
        std::cin >> from >> to;
        graph.AddEdge(from - 1, to - 1);
        graph.AddEdge(to - 1, from - 1);
    }
    return graph;
}

enum class Color {
    WHITE,
    GRAY,
    BLACK,
};

/*
 *  https://www.boost.org/doc/libs/1_65_1/libs/graph/doc/DFSVisitor.html
 *  https://www.boost.org/doc/libs/1_65_1/libs/graph/doc/depth_first_search.html
 */
class Visitor {
public:
    using VertexId = Graph::VertexId;

    virtual ~Visitor() = default;

    virtual void DiscoverVertex(VertexId) {}
    virtual void FinishVertex(VertexId) {}

    virtual void TreeEdge(VertexId, VertexId) {}
    virtual void BackEdge(VertexId, VertexId) {}
    virtual void ForwardOrCrossEdge(VertexId, VertexId) {}
};

class ComponentVisitor : public Visitor {
public:
    void DiscoverVertex(VertexId vertex) override {
        component_.insert(vertex);
    }

    void PrintComponent() const {
        std::cout << component_.size() << '\n';
        for (auto vertex : component_) {
            std::cout << vertex + 1 << ' ';
        }
        std::cout << std::endl;
    }

    std::set<VertexId> component_;
};

class CycleVisitor : public Visitor {
public:
    explicit CycleVisitor(VertexId numVertices) : parents_(numVertices) {
    }

    void BackEdge(VertexId from, VertexId to) override {
        if (cycle_.empty() && parents_[from] != to) {
            // YOUR CODE GOES HERE
            // This is the cycle (undirected graph)
        }
    }

    void TreeEdge(VertexId from, VertexId to) override {
        // YOUR CODE GOES HERE
        // Save to parents_
    }

    void PrintCycle() const {
        if (cycle_.empty()) {
            std::cout << "NO\n";
        } else {
            std::cout << "YES\n";
            // YOUR CODE GOES HERE
        }
    }

    std::vector<VertexId> cycle_;
    std::vector<VertexId> parents_;
};

void Dfs(const Graph& graph, Graph::VertexId vertex, std::vector<Color>& colors, Visitor& visitor) {
    visitor.DiscoverVertex(vertex);
    colors[vertex] = Color::GRAY;
    for (Graph::VertexId neighbor : graph.GetNeighbors(vertex)) {
        if (colors[neighbor] == Color::WHITE) {
            visitor.TreeEdge(vertex, neighbor);
            Dfs(graph, neighbor, colors, visitor);
        } else if (colors[neighbor] == Color::GRAY) {
            visitor.BackEdge(vertex, neighbor);
        } else {
            visitor.ForwardOrCrossEdge(vertex, neighbor);
        }
    }
    visitor.FinishVertex(vertex);
    colors[vertex] = Color::BLACK;
}

void Dfs(const Graph& graph, Visitor& visitor) {
    std::vector<Color> colors(graph.NumVertices());
    for (Graph::VertexId vertex = 0; vertex < graph.NumVertices(); ++vertex) {
        if (colors[vertex] == Color::WHITE) {
            Dfs(graph, vertex, colors, visitor);
        }
    }
}

//int main() {
//    std::cin.tie(nullptr);
//    std::ios_base::sync_with_stdio(false);
//
//    auto graph = ReadUndirectedGraph();
//    ComponentVisitor visitor;
//
//    std::vector<Color> colors(graph.NumVertices());
//    Dfs(graph, 0, colors, visitor);
//
//    visitor.PrintComponent();
//}

Graph ReadAdjMatrix() {
    Graph graph;
    Graph::VertexId numVertices;
    std::cin >> numVertices;
    for (Graph::VertexId v = 0; v < numVertices; ++v) {
        graph.AddVertex();
    }
    for (Graph::VertexId from = 0; from < numVertices; ++from) {
        for (Graph::VertexId to = 0; to < numVertices; ++to) {
            char has;
            std::cin >> has;
            if (has == '1') {
                graph.AddEdge(from, to);
            }
        }
    }
    return graph;
}

int main() {
    auto graph = ReadAdjMatrix();
    CycleVisitor visitor(graph.NumVertices());

    Dfs(graph, visitor);

    visitor.PrintCycle();
}





//class Visitor {
//public:
//    using VertexId = Graph::VertexId;
//
//    void DiscoverVertex(VertexId vertex) {
//        time_.push_back(vertex);
//        component_.insert(vertex);
//    }
//
//    void FinishVertex(VertexId vertex) {
//        time_.push_back(vertex);
//    }
//
//    void BackEdge(VertexId, VertexId) {
//        hasCycle_ = true;
//    }
//
//    void TreeEdge(VertexId, VertexId) {
//    }
//
//    void ForwardOrCrossEdge(VertexId, VertexId) {
//        // depends on [timeIn, timeOut] intervals
//        // get it from time_
//    }
//
//    std::vector<VertexId> time_;
//    bool hasCycle_ = false;
//
//    std::set<VertexId> component_;
//};
