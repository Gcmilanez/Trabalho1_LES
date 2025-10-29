#include <iostream>
#include <vector>
#include <cstdint>
#include "bench_utils.h"
#include <algorithm>
#include <unordered_set>
#include "TreeVariants.h"

// Inserção em ordem balanceada para melhorar localidade de cache em BSTs.
// Pré-condição: o vetor está ordenado (e, idealmente, com duplicatas removidas).
template <typename T, typename Inserter>
static void insert_balanced(const std::vector<T>& v, Inserter ins, size_t lo, size_t hi) {
    if (lo >= hi) return;
    size_t mid = lo + (hi - lo) / 2;
    ins(v[mid]);
    insert_balanced(v, ins, lo, mid);
    insert_balanced(v, ins, mid + 1, hi);
}

int main(int argc, char** argv) {
    const std::string data_file = (argc > 1) ? argv[1] : "tree_data.bin";
    const size_t num_searches = (argc > 2) ? std::stoull(argv[2]) : 2'000'000;

    std::cout << "[AVL] Arquivo: " << data_file << " | buscas: " << num_searches << std::endl;

    auto data = load_tree_data(data_file);
    // Construir ordem de inserção balanceada
    std::vector<uint64_t> sorted = data;
    std::sort(sorted.begin(), sorted.end());
    sorted.erase(std::unique(sorted.begin(), sorted.end()), sorted.end());

    if (data.empty()) {
        std::cerr << "[AVL] Dataset vazio ou não encontrado.\n";
        return 1;
    }

    AVLTree tree;
    Timer t;
    t.start();
    for (auto v : data) tree.insert(v);
    double insert_ms = t.stop_ms();

    auto queries = sample_queries<uint64_t>(data, num_searches);
    std::sort(queries.begin(), queries.end());
    uint64_t found_count = 0;
    t.start();
    for (auto q : queries) {
        if (tree.search(q)) found_count++;
    }
    double search_ms = t.stop_ms();

    std::cout << "[AVL] Tempo inserção (ms): " << insert_ms << "\n";
    std::cout << "[AVL] Tempo busca    (ms): " << search_ms << "\n";
    std::cout << "[AVL] found_count: " << found_count << std::endl;
    return 0;
}
