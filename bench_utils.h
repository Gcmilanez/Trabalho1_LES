#ifndef BENCH_UTILS_H
#define BENCH_UTILS_H

#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include <chrono>
#include <random>

// -------- Timing helpers --------
struct Timer {
    std::chrono::high_resolution_clock::time_point t0;
    void start() { t0 = std::chrono::high_resolution_clock::now(); }
    double stop_ms() const {
        auto t1 = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(t1 - t0).count();
    }
};

// Load binary file with uint64_t values (used by the tree benchmark)
inline std::vector<uint64_t> load_tree_data(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        std::cerr << "[load_tree_data] Falha ao abrir arquivo: " << filename << std::endl;
        return {};
    }
    in.seekg(0, std::ios::end);
    auto len = in.tellg();
    in.seekg(0, std::ios::beg);
    if (len % static_cast<std::streamoff>(sizeof(uint64_t)) != 0) {
        std::cerr << "[load_tree_data] Tamanho do arquivo não é múltiplo de sizeof(uint64_t)." << std::endl;
        return {};
    }
    size_t n = static_cast<size_t>(len / sizeof(uint64_t));
    std::vector<uint64_t> data(n);
    in.read(reinterpret_cast<char*>(data.data()), len);
    return data;
}

// Load SNAP-like edge list (two integers per non-comment line).
// Returns (num_vertices, edges)
inline std::pair<uint32_t, std::vector<std::pair<uint32_t,uint32_t>>>
load_snap_graph(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "[load_snap_graph] Falha ao abrir arquivo: " << filename << std::endl;
        return {0, {}};
    }
    std::string line;
    std::vector<std::pair<uint32_t,uint32_t>> edges;
    uint32_t maxv = 0;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        uint32_t u, v;
        if (!(iss >> u >> v)) continue;
        edges.emplace_back(u, v);
        if (u > maxv) maxv = u;
        if (v > maxv) maxv = v;
    }
    // Vertices are assumed 0..maxv or 1..maxv; we make it safe by +1
    uint32_t V = maxv + 1;
    return {V, edges};
}

// Create a pseudo-random subset of queries from a base vector (without bounds issues)
template<typename T>
inline std::vector<T> sample_queries(const std::vector<T>& base, size_t q, uint64_t seed = 42) {
    if (base.empty()) return {};
    std::vector<T> queries;
    queries.reserve(q);
    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<size_t> dist(0, base.size() - 1);
    for (size_t i = 0; i < q; ++i) {
        queries.push_back(base[dist(rng)]);
    }
    return queries;
}

#endif // BENCH_UTILS_H
