#include "color_walk.hpp"
#include <vector>
#include <climits>

using namespace cs251;

// Min-heap implementation
template<typename T>
class MinHeap {
private:
    std::vector<T> heap;

    // Helper functions to maintain heap property
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[parent] > heap[index]) {
                std::swap(heap[parent], heap[index]);
                index = parent;
            } else {
                break;
            }
        }
    }

    void heapifyDown(int index) {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        int smallest = index;

        if (leftChild < heap.size() && heap[leftChild] < heap[smallest]) {
            smallest = leftChild;
        }
        if (rightChild < heap.size() && heap[rightChild] < heap[smallest]) {
            smallest = rightChild;
        }

        if (smallest != index) {
            std::swap(heap[index], heap[smallest]);
            heapifyDown(smallest);
        }
    }

public:
    // Check if the heap is empty
    bool empty() const {
        return heap.empty();
    }

    // Insert an element into the heap
    void push(const T& value) {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
    }

    // Get the minimum element from the heap
    T top() const {
        return heap.front();
    }

    // Remove the minimum element from the heap
    void pop() {
        if (!empty()) {
            std::swap(heap.front(), heap.back());
            heap.pop_back();
            heapifyDown(0);
        }
    }
};

graph transform_graph(const graph& originalGraph) {
    graph transformedGraph;
    std::vector<graph_vertex> vertices = originalGraph.peek_vertices();

    // For each vertex in the original graph
    for (const auto& vertex : vertices) {
        // Split the vertex into three separate vertices for each color
        for (color c : {RED, GREEN, BLUE}) {
            // Create a new vertex for the current color
            graph_vertex newVertex;
            newVertex.m_handle = vertex.m_handle * 3 + c;

            // Add edges to the new vertex based on the outgoing edges of the original vertex
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

            // Add the new vertex to the transformed graph
            transformedGraph.addVertex(newVertex);
        }
    }
    return transformedGraph;
}


std::vector<std::pair<char, int>> color_walk::calculate(const graph& g, const handle startHandle) {
    // Create a new graph with 3 vertices for each original vertex
    graph transformedGraph = transform_graph(g);

    // Initialize the distances to each vertex in the transformed graph
    std::vector<int> distances(transformedGraph.peek_vertices().size(), INT_MAX);
    std::vector<char> sourceColors(transformedGraph.peek_vertices().size(), '-');

    for (int i = 0; i < 3; i++) {
        distances[startHandle * 3 + i] = 0;
    }

    // Initialize the priority queue for Dijkstra's algorithm
    MinHeap<std::pair<int, handle>> pq;

    // Perform Dijkstra's algorithm on the transformed graph from of the 3 starting vertices
    for (int i = 0; i < 3; i++) {

        // Push the starting vertices into the priority queue
        pq.push({0, startHandle * 3 + i});

        // Perform Dijkstra's algorithm
        while (!pq.empty()) {
            
            // Get the vertex with the minimum distance
            int distance = pq.top().first;
            int vertex = pq.top().second;

            pq.pop();

            // Skip if the distance is already greater than the current distance
            if (distance > distances[vertex]) {
                continue;
            }


            // Update the distances to the adjacent vertices
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

        // Compare the distances manually
        for (int j = 0; j < 3; ++j) {
            if (distances[i * 3 + j] < minDistance) {
                minDistance = distances[i * 3 + j];
                color = sourceColors[i * 3 + j];
            }
        }

        // If there is no path to the vertex, set color to '-' and distance to -1
        if (minDistance == INT_MAX) {
            color = '-';
            minDistance = -1;
        }

        // If the minimum distance is 0, set color to '-'
        if (minDistance == 0) {
            color = '-';
        }

        result.push_back({color, minDistance});
    }

    return result;
}