#pragma once
#include <iostream>
#include "graph.hpp"

namespace cs251 {

class max_height {
	
public:
	struct S {
    	int par;
    	int rk;
	};

	static int find(S list[], int p) {
        while (p != list[p].par) {
            p = list[p].par;
        }
        return p;
    }

    static void Union(S list[], int p, int q) {
        int idP = find(list, p);
        int idQ = find(list, q);
        if (idP == idQ) return;
        if (list[idP].rk < list[idQ].rk) {
            list[idP].par = idQ;
        } else if (list[idP].rk > list[idQ].rk) {
            list[idQ].par = idP;
        } else {
            list[idQ].par = idP;
            list[idP].rk++;
        }
        count--;
    }

	static void bsort(std::vector<graph_edge>& edges) {
    	int n = edges.size();
    	bool repeat = true;

    	while (repeat) {
        	repeat = false;

        	for (int i = n - 1; i > 0; --i) {
            	if (edges[i].m_weight > edges[i - 1].m_weight) {
                	graph_edge temp = edges[i];
                	edges[i] = edges[i - 1];
                	edges[i - 1] = temp;
                	repeat = true;
            	}
        	}
    	}
	}

	static int calculate(const graph &g);

	private:
    	static int count;
};

}