#include "Graph.h"
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

// --- Matriz de Adjacência ---
AdjacencyMatrixGraph::AdjacencyMatrixGraph(uint32_t num_vertices) : V(num_vertices) {
    // Cuidado: Isso aloca V*V booleans. Pode consumir muita memória!
    try {
        matrix.resize( (uint64_t)V * V, false);
    } catch (const std::bad_alloc& e) {
        std::cerr << "Falha ao alocar matriz de adjacência: " << e.what() << std::endl;
        std::cerr << "Memória necessária: " << ((uint64_t)V * V) / (1024*1024) << " MB" << std::endl;
        throw;
    }
}

void AdjacencyMatrixGraph::add_edge(uint32_t u, uint32_t v) {
    matrix[(uint64_t)u * V + v] = true;
}

bool AdjacencyMatrixGraph::has_edge(uint32_t u, uint32_t v) {
    return matrix[(uint64_t)u * V + v];
}

void AdjacencyMatrixGraph::bfs(uint32_t start_node) {
    std::vector<bool> visited(V, false);
    std::queue<uint32_t> q;

    visited[start_node] = true;
    q.push(start_node);

    while (!q.empty()) {
        uint32_t u = q.front();
        q.pop();
        // std::cout << u << " "; // Descomente para ver o percurso
        for (uint32_t v = 0; v < V; ++v) {
            if (has_edge(u, v) && !visited[v]) {
                visited[v] = true;
                q.push(v);
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