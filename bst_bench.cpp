#include <iostream>
#include <vector>
#include <cstdint>
#include "Tree.h"
#include "bench_utils.h"

// Benchmark steps:
// 1) Carrega tree_data.bin (uint64_t)
// 2) Insere todos os elementos na BST
// 3) Faz um conjunto de buscas pseudo-aleatórias
int main(int argc, char** argv) {
    const std::string data_file = (argc > 1) ? argv[1] : "tree_data.bin";
    const size_t num_searches = (argc > 2) ? std::stoull(argv[2]) : 2'000'000; 

    std::cout << "[BST] Arquivo: " << data_file << " | buscas: " << num_searches << std::endl;

    auto data = load_tree_data(data_file);
    if (data.empty()) {
        std::cerr << "[BST] Dataset vazio ou não encontrado.\n";
        return 1;
    }

    // Inserção
    BST tree;
    Timer t;
    t.start();
    for (auto v : data) tree.insert(v);
    double insert_ms = t.stop_ms();

    // Buscas
    auto queries = sample_queries<uint64_t>(data, num_searches);
     uint64_t found_count = 0;
    t.start();
    for (auto q : queries) {
        if (tree.search(q)) found_count++;
    }
    double search_ms = t.stop_ms();

    std::cout << "[BST] Tempo inserção (ms): " << insert_ms << "\n";
    std::cout << "[BST] Tempo busca    (ms): " << search_ms << "\n";
    std::cout << "[BST] found_count: " << found_count << std::endl;
    return 0;
}
