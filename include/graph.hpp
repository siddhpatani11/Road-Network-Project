#pragma once
#include <iostream>
#include <fstream>
#include <vector>

namespace cs251
{
    typedef int handle;

    enum color
    {
        RED, GREEN, BLUE, NONE
    };

    struct graph_edge
    {
        int m_weight = -1;
        handle m_sourceHandle = -1;
        handle m_destinationHandle = -1;
        color col;
    };

    struct graph_vertex
    {
        std::vector<graph_edge> m_edges{};
        handle m_handle = -1;
    };

    class graph
    {
        std::vector<graph_vertex> m_vertices;
    public:
        void read_edge_weights(const std::string& filePath);
        void read_edge_colors(const std::string& filePath);
		// function to access m_vertices
		const std::vector<graph_vertex>& peek_vertices() const {
			return m_vertices;
		}
		void addVertex(const graph_vertex& vertex) {
        	m_vertices.push_back(vertex);
    	}
    };
}
