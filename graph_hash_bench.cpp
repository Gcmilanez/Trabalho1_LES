#include <iostream>
#include <vector>
#include <cstdint>
#include "Graph.h"
#include "bench_utils.h"

int main(int argc, char** argv) {
    const std::string graph_file = (argc > 1) ? argv[1] : "soc-Slashdot0811.txt";
    const uint32_t start_node = (argc > 2) ? static_cast<uint32_t>(std::stoul(argv[2])) : 0;

    std::cout << "[Graph-Hash] Arquivo: " << graph_file << " | start: " << start_node << std::endl;

    auto [V, edges] = load_snap_graph(graph_file);
    if (V == 0) {
        std::cerr << "[Graph-Hash] Falha ao carregar grafo.\n";
        return 1;
    }

    AdjacencyHashMapGraph g(V);

    Timer t;
    t.start();
    for (auto [u, v] : edges) g.add_edge(u, v);
    double build_ms = t.stop_ms();

    t.start();
    g.bfs(start_node);
    double bfs_ms = t.stop_ms();

    std::cout << "[Graph-Hash] Tempo construção (ms): " << build_ms << "\n";
    std::cout << "[Graph-Hash] Tempo BFS         (ms): " << bfs_ms << std::endl;
    return 0;
}
