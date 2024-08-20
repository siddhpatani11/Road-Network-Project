#include "color_walk.hpp"
#include <vector>
#include <climits>

using namespace cs251;

template<typename T>
class MinHeap {
private:
    std::vector<T> heap;

    void up(int idx) {
        int par = (idx - 1) / 2;
        while (idx > 0 && heap[par] > heap[idx]) {
            std::swap(heap[par], heap[idx]);
            idx = par;
            par = (idx - 1) / 2;
        }
    }

    void down(int idx) {
        int l = 2 * idx + 1;
        int r = 2 * idx + 2;
        int s = idx;

        if (l < heap.size() && heap[l] < heap[idx]) {
            s = l;
        }

        if (r < heap.size() && heap[r] < heap[s]) {
            s = r;
        }

        while (s != idx) {
            std::swap(heap[idx], heap[s]);
            idx = s;
            l = 2 * idx + 1;
            r = 2 * idx + 2;
            s = idx;

            if (l < heap.size() && heap[l] < heap[idx]) {
                s = l;
            }

            if (r < heap.size() && heap[r] < heap[s]) {
                s = r;
            }
        }
    }

public:
    bool isHeapEmpty() const {
        return heap.empty();
    }

    void push(const T& item) {
        heap.push_back(item);
        up(heap.size() - 1);
    }

    void pop() {
        if (!isHeapEmpty()) {
            std::swap(heap.front(), heap.back());
            heap.pop_back();
            down(0);
        }
    }

    T top() const {
        return heap.front();
    }
};

graph transform_graph(const graph& originalGraph) {
    graph transformedGraph;
    std::vector<graph_vertex> vertices = originalGraph.peek_vertices();

    // For each vertex in the original graph
    for (const auto& vertex : vertices) {
        for (color c : {RED, GREEN, BLUE}) {
            // Create new vertex for current color
            graph_vertex newVertex;
            newVertex.m_handle = vertex.m_handle * 3 + c;

            // Add edges to the new vertex
            for (const auto& edge : vertex.m_edges) {
                if (edge.col == c) {
                    // Add an edge to the new vertex with the same weight
                    graph_edge newEdge;
                    newEdge.m_weight = edge.m_weight;
                    newEdge.m_sourceHandle = newVertex.m_handle;
                    newEdge.m_destinationHandle = edge.m_destinationHandle * 3 + ((c + 1) % 3);
                    newVertex.m_edges.push_back(newEdge);
                }
            }

            // Add the new vertex to new graph
            transformedGraph.addVertex(newVertex);
        }
    }
    return transformedGraph;
}


std::vector<std::pair<char, int>> color_walk::calculate(const graph& g, const handle startHandle) {
    graph transformedGraph = transform_graph(g);

    // Initialize the distances to each vertex in the transformed graph
    std::vector<int> distances(transformedGraph.peek_vertices().size(), INT_MAX);
    std::vector<char> sourceColors(transformedGraph.peek_vertices().size(), '-');

    for (int i = 0; i < 3; i++) {
        distances[startHandle * 3 + i] = 0;
    }

    // Initialize priority queue
    MinHeap<std::pair<int, handle>> pq;

    for (int i = 0; i < 3; i++) {

        // Push starting vertices into priority queue
        pq.push({0, startHandle * 3 + i});

        while (!pq.isHeapEmpty()) {
            
            // Get vertex with minimum distance
            int distance = pq.top().first;
            int vertex = pq.top().second;

            pq.pop();

            // if the distance is already greater than the current distance then skip
            if (distance > distances[vertex]) {
                continue;
            }

            for (const auto& edge : transformedGraph.peek_vertices()[vertex].m_edges) {
                int newDistance;
                if (distances[vertex] != INT_MAX) {
                    newDistance = distance + edge.m_weight;
                } else {
                    newDistance = INT_MAX;
                }

                if (newDistance < distances[edge.m_destinationHandle]) {
                    distances[edge.m_destinationHandle] = newDistance;
                    if (i == 0) {
                        sourceColors[edge.m_destinationHandle] = 'R';
                    } else if (i == 1) {
                        sourceColors[edge.m_destinationHandle] = 'G';
                    } else {
                        sourceColors[edge.m_destinationHandle] = 'B';
                    }
                    pq.push({newDistance, edge.m_destinationHandle});
                }
            }
        }
    }


    std::vector<std::pair<char, int>> result;
    for (int i = 0; i < g.peek_vertices().size(); ++i) {
        int minDistance = INT_MAX;
        char color = '-';

        // Compare the distances
        for (int j = 0; j < 3; ++j) {
            if (distances[i * 3 + j] < minDistance) {
                minDistance = distances[i * 3 + j];
                color = sourceColors[i * 3 + j];
            }
        }

        if (minDistance == INT_MAX) {
            color = '-';
            minDistance = -1;
        }

        if (minDistance == 0) {
            color = '-';
        }

        result.push_back({color, minDistance});
    }

    return result;
}