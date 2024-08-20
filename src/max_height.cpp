#include "max_height.hpp"
#include <vector>
#include <climits>

using namespace cs251;

int max_height::count = 0;

int max_height::calculate(const graph& g) {
    const auto& vertices = g.peek_vertices();
    int length = vertices.size();
    std::vector<S> list(length);
    std::vector<graph_edge> result;

    for (int i = 0; i < length; ++i) {
        list[i].par = i;
        list[i].rk = 0;
    }

    std::vector<graph_edge> edges;
    for (int i = 0; i < length; ++i) {
        for (const auto& edge : vertices[i].m_edges) {
            edges.push_back(edge);
        }
    }
    
	bsort(edges);

    int min_weight = INT_MAX;
    for (const auto& edge : edges) {
        int x = find(list.data(), edge.m_sourceHandle);
        int y = find(list.data(), edge.m_destinationHandle);

        if (x != y) {
            result.push_back(edge);
            Union(list.data(), x, y);
            if (edge.m_weight < min_weight) {
    			min_weight = edge.m_weight;
			}
        }
    }

    return min_weight;
}