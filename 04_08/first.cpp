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
    void BackEdge(VertexId, VertexId) override {
        hasCycle_ = true;
        // here we can obtain cycle from parents array
        // parents can be saved in TreeEdge
    }

    void PrintCycle() const {
        std::cout << std::boolalpha << hasCycle_ << std::endl;
    }

    bool hasCycle_ = false;
    std::vector<VertexId> parents_;
};

void Dfs(const Graph& graph, Graph::VertexId vertex, std::vector<Color>& colors, Visitor& visitor) {
    colors[vertex] = Color::GRAY;
    visitor.DiscoverVertex(vertex);
    for (Graph::VertexId neighbor : graph.GetNeighbors(vertex)) {
        if (colors[neighbor] == Color::WHITE) {
            Dfs(graph, neighbor, colors, visitor);
            visitor.TreeEdge(vertex, neighbor);
        } else if (colors[neighbor] == Color::GRAY) {
            visitor.BackEdge(vertex, neighbor);
        } else {
            visitor.ForwardOrCrossEdge(vertex, neighbor);
        }
    }
    colors[vertex] = Color::BLACK;
    visitor.FinishVertex(vertex);
}

void Dfs(const Graph& graph, Visitor& visitor) {
    std::vector<Color> colors(graph.NumVertices());
    for (Graph::VertexId vertex = 0; vertex < graph.NumVertices(); ++vertex) {
        if (colors[vertex] == Color::WHITE) {
            Dfs(graph, vertex, colors, visitor);
        }
    }
}

int main() {
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    auto graph = ReadUndirectedGraph();
    ComponentVisitor visitor;

    std::vector<Color> colors(graph.NumVertices());
    Dfs(graph, 0, colors, visitor);

    visitor.PrintComponent();
}

//int main() {
//    auto graph = ReadUndirectedGraph();
//    CycleVisitor visitor;
//
//    Dfs(graph, visitor);
//
//    visitor.PrintCycle();
//}





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
