#include <iostream>
#include <numeric>
#include <vector>
#include <queue>
#include <set>

class Graph {
public:
    using EdgeId = size_t;
    using VertexId = size_t;

    const auto& GetOutgoingEdges(VertexId vertexId) const {
        return adjList_[vertexId];
    }

    const auto& GetEdge(EdgeId edgeId) const {
        return edges_[edgeId];
    }

    VertexId NumVertices() const {
        return adjList_.size();
    }

protected:
    void AddEdge(VertexId from, VertexId to) {
        auto id = edges_.size();
        edges_.push_back({to});
        adjList_[from].push_back(id);
    }

    VertexId AddVertex() {
        adjList_.emplace_back();
        return adjList_.size() - 1;
    }

private:
    struct Edge {
//        EdgeId id;
//        VertexId source;
        VertexId target;
    };

    std::vector<std::vector<EdgeId>> adjList_;
    std::vector<Edge> edges_;
};

class WeighedGraph : public Graph {
public:
    using Weight = int64_t;
    static constexpr inline Weight INF = std::numeric_limits<Weight>::max();
    // be careful with `std::numeric_limits<int>::inf() == 0`

    Weight GetWeight(EdgeId edgeId) const {
        return edgeProperties_[edgeId].weight;
    }

private:
    friend WeighedGraph ReadWeightedUndirectedGraph();

    void AddEdge(VertexId from, VertexId to, Weight weight) {
        Graph::AddEdge(from, to);
        edgeProperties_.push_back({weight});
    }

    struct EdgeProperties {
//        For example:
//        Cost cost;
//        Length length;
//        Quality quality;
//        RoadId roadId;
//        Speed speed;
        Weight weight;
    };

    std::vector<EdgeProperties> edgeProperties_;
};

WeighedGraph ReadWeightedUndirectedGraph() {
    WeighedGraph graph;
    int nv, ne;
    std::cin >> nv >> ne;
    for (int i = 0; i < nv; ++i) {
        graph.AddVertex();
    }
    for (int i = 0; i < ne; ++i) {
        WeighedGraph::VertexId from, to;
        WeighedGraph::Weight weight;
        std::cin >> from >> to >> weight;
        graph.AddEdge(from - 1, to - 1, weight);
        graph.AddEdge(to - 1, from - 1, weight);
    }
    return graph;
}

enum class Color {
    WHITE,
    GRAY,
    BLACK,
};

/*
 * https://www.boost.org/doc/libs/1_41_0/libs/graph/doc/dijkstra_shortest_paths.html
 */
class DijkstraVisitor {
public:
    using VertexId = WeighedGraph::VertexId;
    using EdgeId = WeighedGraph::EdgeId;

    virtual ~DijkstraVisitor() = default;

    virtual void DiscoverVertex(VertexId) {}
    virtual void ExamineVertex(VertexId) {}
    virtual void FinishVertex(VertexId) {}

    virtual void ExamineEdge(EdgeId) {}
    virtual void EdgeRelaxed(EdgeId) {}
    virtual void EdgeNotRelaxed(EdgeId) {}
};

template <class Queue>
void DecreaseKey(Queue& queue, WeighedGraph::VertexId vertexId, WeighedGraph::Weight distance);

template <class Queue>
auto Dijkstra(
    const WeighedGraph& graph,
    WeighedGraph::VertexId source,
    DijkstraVisitor& visitor)
{
    std::vector<WeighedGraph::Weight> distances(
        graph.NumVertices(),
        std::numeric_limits<WeighedGraph::Weight>::max());
    distances[source] = 0;
    std::vector<Color> colors(graph.NumVertices());
    colors[source] = Color::GRAY;
    visitor.DiscoverVertex(source);
    std::vector<WeighedGraph::VertexId> parents(graph.NumVertices(), -1);

    Queue queue;
    queue.push({distances[source], source});

    while (!queue.empty()) {
        auto [distance, from] = queue.top();
        queue.pop();
        if (colors[from] == Color::BLACK) {
            continue;
        }
        visitor.ExamineVertex(source);
        for (auto edgeId : graph.GetOutgoingEdges(from)) {
            visitor.ExamineEdge(edgeId);
            auto to = graph.GetEdge(edgeId).target;
            if (graph.GetWeight(edgeId) < WeighedGraph::INF - distance &&
                    distances[to] > distance + graph.GetWeight(edgeId)) {
                distances[to] = distance + graph.GetWeight(edgeId);
                parents[to] = from;
                visitor.EdgeRelaxed(edgeId);
                if (colors[to] == Color::WHITE) {
                    colors[to] = Color::GRAY;
                    visitor.DiscoverVertex(to);
                    queue.push({distances[to], to});
                } else if (colors[to] == Color::GRAY) {
                    // Can be amortized to O(1) in Fibonacci Heaps:
                    DecreaseKey(queue, to, distances[to]);
                }
            } else {
                 visitor.EdgeNotRelaxed(edgeId);
            }
        }
        visitor.FinishVertex(from);
        colors[from] = Color::BLACK;
    }
    return std::make_pair(distances, parents);
}

struct HeapElement {
    WeighedGraph::Weight distance;
    WeighedGraph::VertexId vertexId;

    auto operator<=>(const HeapElement&) const = default;
};

using MinHeap = std::priority_queue<HeapElement, std::vector<HeapElement>, std::greater<>>;

template <>
void DecreaseKey(MinHeap& queue, WeighedGraph::VertexId vertexId, WeighedGraph::Weight distance) {
    queue.push({distance, vertexId});
}

int main() {
//    std::cin.tie(nullptr);
//    std::ios_base::sync_with_stdio(false);

    auto graph = ReadWeightedUndirectedGraph();

    WeighedGraph::VertexId source, target;
    std::cin >> source >> target;

    DijkstraVisitor visitor;
    auto[distances, _] = Dijkstra<MinHeap>(graph, source - 1, visitor);

    auto weight = distances[target - 1];
    if (weight == WeighedGraph::INF) {
        weight = -1;
    }
    std::cout << weight << std::endl;
}
