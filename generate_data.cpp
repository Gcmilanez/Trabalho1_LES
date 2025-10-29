#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <string>
#include <cstdlib>   // strtoull
#include <cstring>   // strcmp

int main(int argc, char** argv) {
    const char* out_filename = "tree_data.bin";

    // --------------------------------------------------------
    // MODO REAL (dataset externo em txt)
    // --------------------------------------------------------
    // Uso: ./generate_data --from-file ids.txt
    if (argc >= 3 && std::strcmp(argv[1], "--from-file") == 0) {
        const char* in_path = argv[2];
        std::ifstream in(in_path);
        if (!in) {
            std::cerr << "Erro ao abrir arquivo de entrada: " << in_path << "\n";
            return 1;
        }

        std::vector<uint64_t> data;
        data.reserve(1 << 20); // reserva inicial (~1M)
        uint64_t x;
        while (in >> x) data.push_back(x);

        if (data.empty()) {
            std::cerr << "Nenhum inteiro encontrado em " << in_path << "\n";
            return 1;
        }

        std::ofstream out(out_filename, std::ios::binary);
        if (!out) {
            std::cerr << "Erro ao criar arquivo binário de saída: " << out_filename << "\n";
            return 1;
        }

        out.write(reinterpret_cast<const char*>(data.data()), data.size() * sizeof(uint64_t));
        std::cout << "Convertidos " << data.size() << " elementos de '" 
                  << in_path << "' para '" << out_filename << "'.\n";
        return 0;
    }

    // --------------------------------------------------------
    // MODO SINTÉTICO (original), agora configurável
    // --------------------------------------------------------
    uint64_t num_elements = 2'000'000ULL; // antes: 128M, agora padrão mais leve

    // Uso: ./generate_data --count 5000000
    if (argc >= 3 && std::strcmp(argv[1], "--count") == 0 && argv[2]) {
        char* end = nullptr;
        unsigned long long v = std::strtoull(argv[2], &end, 10);
        if (end != argv[2] && v > 0) {
            num_elements = static_cast<uint64_t>(v);
        }
    }

    std::vector<uint64_t> synthetic(num_elements);
    for (uint64_t i = 0; i < num_elements; ++i)
        synthetic[i] = i;

    std::ofstream out(out_filename, std::ios::binary);
    if (!out) {
        std::cerr << "Erro ao criar arquivo '" << out_filename << "'\n";
        return 1;
    }

    out.write(reinterpret_cast<const char*>(synthetic.data()),
              synthetic.size() * sizeof(uint64_t));

    std::cout << "Dataset sintético '" << out_filename << "' gerado com "
              << num_elements << " elementos.\n"
              << "Use --count N para tamanho customizado ou --from-file <txt> para dados reais.\n";

    return 0;
}
