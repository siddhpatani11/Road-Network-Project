#include <iostream>
#include "graph.hpp"

using namespace cs251;

void graph::read_edge_weights(const std::string& filePath) {
	// TODO: Implement read edge weights	
	// throw std::logic_error("graph::" + std::string(__FUNCTION__) + "() not implemented");
    std::ifstream file(filePath);

    int numVertices, numEdges;
    file >> numVertices >> numEdges;

    m_vertices.resize(numVertices);

    for (int i = 0; i < numEdges; ++i) {
        int source, destination, weight;
        file >> source >> destination;

        if (file.peek() != '\n') {
            file >> weight;
        } else {
            weight = 0;
        }

        graph_edge edge;
        edge.m_weight = weight;
        edge.m_sourceHandle = source;
        edge.m_destinationHandle = destination;
        m_vertices[source].m_edges.push_back(edge);
    }

    file.close();
}

void graph::read_edge_colors(const std::string& filePath) {
	// TODO: Implement read edge colors	
	// throw std::logic_error("graph::" + std::string(__FUNCTION__) + "() not implemented");
	std::ifstream file(filePath);

    int numVertices, numEdges;
    file >> numVertices >> numEdges;

    m_vertices.resize(numVertices);

    for (int i = 0; i < numEdges; ++i) {
        int source, destination, weight;
        char colorChar;
        file >> source >> destination >> weight >> colorChar;

        graph_edge edge;
        edge.m_weight = weight;
        edge.m_sourceHandle = source;
        edge.m_destinationHandle = destination;
        
        if (colorChar == 'R') {
            edge.col = RED;
        } else if (colorChar == 'G') {
            edge.col = GREEN;
        } else if (colorChar == 'B') {
            edge.col = BLUE;
        }

        m_vertices[source].m_edges.push_back(edge);

    }

    file.close();
}
