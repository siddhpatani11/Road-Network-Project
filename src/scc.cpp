#include "scc.hpp"

using namespace cs251;

void strongconnect(cs251::handle v, const cs251::graph& g, int& index, std::vector<cs251::handle>& S, std::vector<std::vector<cs251::handle>>& SCCs, std::vector<cs251::handle>& indexMap, std::vector<cs251::handle>& lowLinkMap, std::vector<bool>& onStack) {
    indexMap[v] = index;
    lowLinkMap[v] = index;
    index++;
    S.push_back(v);
    onStack[v] = true;

    const std::vector<cs251::graph_vertex>& vertices = g.peek_vertices();
    const std::vector<cs251::graph_edge>& edges = vertices[v].m_edges;

    for (const auto& edge : edges) {
        cs251::handle w = edge.m_destinationHandle;

        if (indexMap[w] == -1) {
            strongconnect(w, g, index, S, SCCs, indexMap, lowLinkMap, onStack);
            if (lowLinkMap[v] < lowLinkMap[w]) {
              lowLinkMap[v] = lowLinkMap[v];
            } else {
              lowLinkMap[v] = lowLinkMap[w];
            }
        } else if (onStack[w]) {
            if (lowLinkMap[v] < indexMap[w]) {
              lowLinkMap[v] = lowLinkMap[v];
            } else {
              lowLinkMap[v] = indexMap[w];
            }
        }
    }

    if (lowLinkMap[v] == indexMap[v]) {
        std::vector<cs251::handle> currentSCC;
        cs251::handle w = S.back();
        S.pop_back();
        onStack[w] = false;
        currentSCC.push_back(w);
    
        while (w != v) {
            w = S.back();
            S.pop_back();
            onStack[w] = false;
            currentSCC.push_back(w);
        }

        SCCs.push_back(currentSCC);
    }
}

int scc::search(const graph& g) {
  int index = 0;
  std::vector<cs251::handle> S;
  std::vector<std::vector<cs251::handle>> SCCs;
  std::vector<cs251::handle> indexMap(g.peek_vertices().size(), -1);
  std::vector<cs251::handle> lowLinkMap(g.peek_vertices().size(), -1);
  std::vector<bool> onStack(g.peek_vertices().size(), false);

  // Iterate through vertices
  for (int v = 0; v < g.peek_vertices().size(); ++v) {
    if (indexMap[v] == -1) {
      strongconnect(v, g, index, S, SCCs, indexMap, lowLinkMap, onStack);
    }
  }

  return SCCs.size();
}
