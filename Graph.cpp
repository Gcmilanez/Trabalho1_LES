#include "Graph.h"
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

AdjacencyMatrixGraph::AdjacencyMatrixGraph(uint32_t num_vertices)
    : V(num_vertices)
{
    words_per_row = (V + 63u) / 64u;
    matrix.assign(static_cast<size_t>(V) * words_per_row, 0ull);
}

void AdjacencyMatrixGraph::add_edge(uint32_t u, uint32_t v) {
    set_edge_bit(u, v);
    // Se o grafo for não-direcionado, descomente:
    // set_edge_bit(v, u);
}

bool AdjacencyMatrixGraph::has_edge(uint32_t u, uint32_t v) {
    return test_edge_bit(u, v);
}

void AdjacencyMatrixGraph::bfs(uint32_t start_node) {
    std::vector<bool> visited(V, false);
    std::queue<uint32_t> q;
    visited[start_node] = true;
    q.push(start_node);

    while (!q.empty()) {
        uint32_t u = q.front(); q.pop();

        const uint64_t* row = &matrix[static_cast<size_t>(u) * words_per_row];
        for (uint32_t w = 0; w < words_per_row; ++w) {
            uint64_t word = row[w];
            while (word) {
                unsigned tz = (unsigned)__builtin_ctzll(word);
                uint32_t v = (w << 6) + tz;
                if (v < V && !visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
                word &= (word - 1);
            }
        }
    }
}

// --- Lista de Adjacência ---
AdjacencyListGraph::AdjacencyListGraph(uint32_t num_vertices) : V(num_vertices) {
    adj_list.resize(V);
}

void AdjacencyListGraph::add_edge(uint32_t u, uint32_t v) {
    adj_list[u].push_back(v);
}

bool AdjacencyListGraph::has_edge(uint32_t u, uint32_t v) {
    for (uint32_t neighbor : adj_list[u]) {
        if (neighbor == v) {
            return true;
        }
    }
    return false;
}

void AdjacencyListGraph::bfs(uint32_t start_node) {
    std::vector<bool> visited(V, false);
    std::queue<uint32_t> q;

    visited[start_node] = true;
    q.push(start_node);

    while (!q.empty()) {
        uint32_t u = q.front();
        q.pop();
        // std::cout << u << " "; // Descomente para ver o percurso
        for (uint32_t v : adj_list[u]) {
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }
}

// --- Hash Map de Adjacência ---
AdjacencyHashMapGraph::AdjacencyHashMapGraph(uint32_t num_vertices) : V(num_vertices) {}

void AdjacencyHashMapGraph::add_edge(uint32_t u, uint32_t v) {
    adj_map[u].push_back(v);
}

bool AdjacencyHashMapGraph::has_edge(uint32_t u, uint32_t v) {
    if (adj_map.find(u) == adj_map.end()) {
        return false;
    }
    const auto& neighbors = adj_map.at(u);
    return std::find(neighbors.begin(), neighbors.end(), v) != neighbors.end();
}

void AdjacencyHashMapGraph::bfs(uint32_t start_node) {
    std::vector<bool> visited(V, false);
    std::queue<uint32_t> q;

    visited[start_node] = true;
    q.push(start_node);

    while (!q.empty()) {
        uint32_t u = q.front();
        q.pop();
        // std::cout << u << " "; // Descomente para ver o percurso
        if (adj_map.count(u)) {
            for (uint32_t v : adj_map.at(u)) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
    }
}