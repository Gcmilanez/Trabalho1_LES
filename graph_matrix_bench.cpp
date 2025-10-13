#include <iostream>
#include <vector>
#include <cstdint>
#include "Graph.h"
#include "bench_utils.h"

// This benchmark builds the graph and runs exactly 8 BFS traversals.
// Usage: ./graph_matrix_bench <graph_file> <start_node>
int main(int argc, char** argv) {
    const std::string filename   = (argc > 1) ? argv[1] : "soc-Slashdot0811.txt";
    const uint32_t    start_node = (argc > 2) ? static_cast<uint32_t>(std::stoul(argv[2])) : 0;

    std::cout << "[Graph-Matrix] Arquivo: " << filename << " | start: " << start_node
              << " | BFS_runs: 8\n";

    // Load edges (SNAP-like "u v" per line). Returns <V, edges>.
    auto [V, edges] = load_snap_graph(filename);
    if (V == 0) {
        std::cerr << "[Graph-Matrix] Falha ao carregar grafo.\n";
        return 1;
    }

    // Build
    Timer t;
    t.start();
    AdjacencyMatrixGraph g(V);
    for (auto [u, v] : edges) g.add_edge(u, v);
    double build_ms = t.stop_ms();

    // 8 BFS runs (same start node for determinism)
    t.start();
    for (int i = 0; i < 8; ++i) g.bfs(start_node);
    double bfs_ms = t.stop_ms();

    std::cout << "[Graph-Matrix] Tempo construção (ms): " << build_ms << "\n";
    std::cout << "[Graph-Matrix] Tempo 8x BFS (ms): " << bfs_ms << std::endl;
    return 0;
}
