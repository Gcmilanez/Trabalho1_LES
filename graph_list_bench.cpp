#include <iostream>
#include <vector>
#include <cstdint>
#include "Graph.h"
#include "bench_utils.h"

// This benchmark builds the graph and runs exactly 32 BFS traversals.
// Usage: ./graph_list_bench <graph_file> <start_node>
int main(int argc, char** argv) {
    const std::string filename   = (argc > 1) ? argv[1] : "soc-Slashdot0811.txt";
    const uint32_t    start_node = (argc > 2) ? static_cast<uint32_t>(std::stoul(argv[2])) : 0;

    std::cout << "[Graph-List] Arquivo: " << filename << " | start: " << start_node
              << " | BFS_runs: 32\n";

    // Load edges (SNAP-like "u v" per line). Returns <V, edges>.
    auto [V, edges] = load_snap_graph(filename);
    if (V == 0) {
        std::cerr << "[Graph-List] Falha ao carregar grafo.\n";
        return 1;
    }

    // Build
    Timer t;
    t.start();
    AdjacencyListGraph g(V);
    for (auto [u, v] : edges) g.add_edge(u, v);
    double build_ms = t.stop_ms();

    // 32 BFS runs (same start node for determinism)
    t.start();
    for (int i = 0; i < 32; ++i) g.bfs(start_node);
    double bfs_ms = t.stop_ms();

    std::cout << "[Graph-List] Tempo construção (ms): " << build_ms << "\n";
    std::cout << "[Graph-List] Tempo 32x BFS (ms): " << bfs_ms << std::endl;
    return 0;
}
