#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <cstdint>
#include <unordered_map>

// Interface base para o Grafo
class IGraph {
public:
    virtual ~IGraph() = default;
    virtual void add_edge(uint32_t u, uint32_t v) = 0;
    virtual bool has_edge(uint32_t u, uint32_t v) = 0;
    virtual void bfs(uint32_t start_node) = 0;
    virtual uint32_t get_num_vertices() const = 0;
};

// Grafo com Matriz de Adjacência
class AdjacencyMatrixGraph : public IGraph {
public:
    AdjacencyMatrixGraph(uint32_t num_vertices);
    void add_edge(uint32_t u, uint32_t v) override;
    bool has_edge(uint32_t u, uint32_t v) override;
    void bfs(uint32_t start_node) override;
    uint32_t get_num_vertices() const override { return V; }

private:
    uint32_t V;
    std::vector<bool> matrix; // Armazenado como vetor 1D para eficiência
};

// Grafo com Lista de Adjacência
class AdjacencyListGraph : public IGraph {
public:
    AdjacencyListGraph(uint32_t num_vertices);
    void add_edge(uint32_t u, uint32_t v) override;
    bool has_edge(uint32_t u, uint32_t v) override;
    void bfs(uint32_t start_node) override;
    uint32_t get_num_vertices() const override { return V; }

private:
    uint32_t V;
    std::vector<std::vector<uint32_t>> adj_list;
};

// Grafo com Hash Map de Adjacência
class AdjacencyHashMapGraph : public IGraph {
public:
    AdjacencyHashMapGraph(uint32_t num_vertices);
    void add_edge(uint32_t u, uint32_t v) override;
    bool has_edge(uint32_t u, uint32_t v) override;
    void bfs(uint32_t start_node) override;
    uint32_t get_num_vertices() const override { return V; }

private:
    uint32_t V;
    std::unordered_map<uint32_t, std::vector<uint32_t>> adj_map;
};

#endif // GRAPH_H