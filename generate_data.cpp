#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>
#include <cstdint>

int main() {
    const uint64_t num_elements = 128000000;
    const char* filename = "tree_data.bin";

    std::vector<uint64_t> data(num_elements);
    for (uint64_t i = 0; i < num_elements; ++i) {
        data[i] = i;
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(data.begin(), data.end(), g);

    std::ofstream outfile(filename, std::ios::binary);
    if (!outfile) {
        std::cerr << "Erro ao abrir o arquivo para escrita: " << filename << std::endl;
        return 1;
    }

    outfile.write(reinterpret_cast<const char*>(data.data()), data.size() * sizeof(uint64_t));
    
    std::cout << "Dataset '" << filename << "' gerado com " << num_elements << " elementos." << std::endl;
    
    return 0;
}

// Compilar com: g++ -std=c++17 -O3 generate_data.cpp -o generate_data ./generate_data