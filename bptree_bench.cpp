#include <iostream>
#include <vector>
#include <cstdint>
#include "bench_utils.h"
#include "TreeVariants.h"

int main(int argc, char** argv) {
    const std::string data_file = (argc > 1) ? argv[1] : "tree_data.bin";
    const size_t num_searches = (argc > 2) ? std::stoull(argv[2]) : 2'000'000;
    const int order = (argc > 3) ? std::stoi(argv[3]) : 64; // B+Tree order

    std::cout << "[B+Tree] Arquivo: " << data_file << " | buscas: " << num_searches << " | ordem: " << order << std::endl;

    auto data = load_tree_data(data_file);
    if (data.empty()) {
        std::cerr << "[B+Tree] Dataset vazio ou não encontrado.\n";
        return 1;
    }

    BPlusTree tree(order);
    Timer t;
    t.start();
    for (auto v : data) tree.insert(v);
    double insert_ms = t.stop_ms();

    auto queries = sample_queries<uint64_t>(data, num_searches);
    uint64_t found_count = 0;
    t.start();
    for (auto q : queries) {
        if (tree.search(q)) found_count++;
    }
    double search_ms = t.stop_ms();

    std::cout << "[B+Tree] Tempo inserção (ms): " << insert_ms << "\n";
    std::cout << "[B+Tree] Tempo busca    (ms): " << search_ms << "\n";
    std::cout << "[B+Tree] found_count: " << found_count << std::endl;
    return 0;
}
