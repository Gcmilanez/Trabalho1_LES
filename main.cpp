#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include <set>
#include <algorithm>
#include <iomanip>
#include <string>
#include <sstream>

#include "Tree.h"
#include "Graph.h"

// --- Funções de Benchmark ---

void benchmark_tree() {
    std::cout << "\n--- Iniciando Benchmark da Árvore (BST) ---\n";
    const char* filename = "tree_data.bin";
    
    std::ifstream infile(filename, std::ios::binary);
    if (!infile) {
        std::cerr << "Erro: Não foi possível abrir o arquivo " << filename << std::endl;
        return;
    }

    infile.seekg(0, std::ios::end);
    size_t file_size = infile.tellg();
    infile.seekg(0, std::ios::beg);
    size_t num_elements = file_size / sizeof(uint64_t);

    std::vector<uint64_t> data(num_elements);
    infile.read(reinterpret_cast<char*>(data.data()), file_size);

    BST bst;

    // 1. Teste de Inserção
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& val : data) {
        bst.insert(val);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> insert_duration = end - start;
    std::cout << "Tempo de inserção de " << num_elements << " elementos: " << std::fixed << std::setprecision(4) << insert_duration.count() << " segundos.\n";

    // 2. Teste de Busca
    // Embaralha os dados para simular buscas aleatórias
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(data.begin(), data.end(), g);
    
    const int num_searches = 1000000; // 1 milhão de buscas
    int found_count = 0;

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_searches; ++i) {
        if (bst.search(data[i])) { // Busca elementos que existem
            found_count++;
        }
    }
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> search_duration = end - start;
    std::cout << "Tempo de busca de " << num_searches << " elementos existentes: " << search_duration.count() << " segundos.\n";
    std::cout << " (encontrados: " << found_count << "/" << num_searches << ")\n";
}

void benchmark_graphs() {
    std::cout << "\n--- Iniciando Benchmark dos Grafos ---\n";
    const char* filename = "soc-Slashdot0811.txt";

    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Erro: Não foi possível abrir o arquivo " << filename << std::endl;
        return;
    }

    std::vector<std::pair<uint32_t, uint32_t>> edges;
    uint32_t u, v;
    uint32_t max_node_id = 0;
    std::string line;
    while (std::getline(infile, line)) {
        if (line[0] == '#') continue;
        std::stringstream ss(line);
        ss >> u >> v;
        edges.emplace_back(u, v);
        max_node_id = std::max({max_node_id, u, v});
    }
    uint32_t num_vertices = max_node_id + 1;
    std::cout << "Dataset do grafo carregado: " << num_vertices << " vértices, " << edges.size() << " arestas.\n";

    // --- Benchmark para cada tipo de grafo ---
    
    // Função lambda para rodar o benchmark em um tipo de grafo
    auto run_benchmark = [&](IGraph& graph, const std::string& type) {
        std::cout << "\nTestando " << type << "...\n";

        // 1. Teste de Construção
        auto start = std::chrono::high_resolution_clock::now();
        for (const auto& edge : edges) {
            graph.add_edge(edge.first, edge.second);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> build_duration = end - start;
        std::cout << "  Tempo de construção: " << std::fixed << std::setprecision(4) << build_duration.count() << " segundos.\n";
        
        // 2. Teste de Travessia (BFS)
        const int num_bfs_runs = 100;
        std::vector<uint32_t> start_nodes;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, num_vertices - 1);
        for(int i = 0; i < num_bfs_runs; ++i) {
            start_nodes.push_back(distrib(gen));
        }
        
        start = std::chrono::high_resolution_clock::now();
        for(uint32_t node : start_nodes) {
            graph.bfs(node);
        }
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> bfs_duration = end - start;
        std::cout << "  Tempo de " << num_bfs_runs << " execuções de BFS: " << bfs_duration.count() << " segundos.\n";
    };

    // Matriz de Adjacência
    // ATENÇÃO: pode falhar se não houver RAM suficiente (~6 GB)
    try {
        AdjacencyMatrixGraph matrix_graph(num_vertices);
        run_benchmark(matrix_graph, "Matriz de Adjacência");
    } catch (const std::bad_alloc& e) {
        std::cerr << "PULANDO Matriz de Adjacência por falta de memória.\n";
    }

    // Lista de Adjacência
    AdjacencyListGraph list_graph(num_vertices);
    run_benchmark(list_graph, "Lista de Adjacência");

    // Hash Map de Adjacência
    AdjacencyHashMapGraph hash_graph(num_vertices);
    run_benchmark(hash_graph, "Hash Map de Adjacência");
}

int main() {
    benchmark_tree();
    benchmark_graphs();
    return 0;
}