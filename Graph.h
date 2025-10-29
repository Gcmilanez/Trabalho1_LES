#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <cstdint>
#include <unordered_map>
#include <cstddef>
#include <queue>

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
// Grafo com Matriz de Adjacência (bit-packed em uint64_t)
class AdjacencyMatrixGraph : public IGraph {
public:
    explicit AdjacencyMatrixGraph(uint32_t num_vertices);
    void add_edge(uint32_t u, uint32_t v) override;
    bool has_edge(uint32_t u, uint32_t v) override;
    void bfs(uint32_t start_node) override;
    inline uint32_t get_num_vertices() const override { return V; }

private:
    uint32_t V;
    uint32_t words_per_row;          // = ceil(V/64)
    std::vector<uint64_t> matrix;    // armazenamento bit-packed

    inline void set_edge_bit(uint32_t u, uint32_t v) {
        const uint32_t w = v >> 6;   // v / 64
        const uint32_t b = v & 63;   // v % 64
        matrix[static_cast<size_t>(u) * words_per_row + w] |= (1ull << b);
    }

    inline bool test_edge_bit(uint32_t u, uint32_t v) const {
        const uint32_t w = v >> 6, b = v & 63;
        return (matrix[static_cast<size_t>(u) * words_per_row + w] >> b) & 1ull;
    }
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