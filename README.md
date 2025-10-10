## Trabalho de Low Energy Softwares

Integrantes:

Giovanni Cabreira Milanez
Gabriel Welter

## Estruturas Implementadas

* **Árvore:**
    * Árvore de Busca Binária (`BST`).
* **Grafo:**
    * Representação com Matriz de Adjacência.
    * Representação com Lista de Adjacência.
    * Representação com Hash Map de Adjacência.

## Estrutura do Projeto

```
.
├── Tree.h                # Definição da estrutura da Árvore
├── Tree.cpp              # Implementação da Árvore
├── Graph.h               # Definição das estruturas de Grafo
├── Graph.cpp             # Implementação dos Grafos
├── main.cpp              # Lógica principal do benchmark
├── generate_data.cpp     # Utilitário para gerar o dataset da árvore
└── README.md             # Este arquivo
```

## Pré-requisitos

Para compilar e executar este projeto, você precisará de:

1.  **Compilador C++ com suporte a C++23:**
    * **GCC:** versão 13 ou superior.
    * **Clang:** versão 16 ou superior.
2.  **Sistema Operacional Linux:** A ferramenta `perf` é uma funcionalidade do kernel do Linux.
3.  **Ferramentas de compilação:** `g++` e `make` (ou apenas `g++`).
4.  **`perf`:** A ferramenta de profiling. Para instalar em sistemas Debian/Ubuntu:
    ```sh
    sudo apt-get update
    sudo apt-get install linux-tools-common linux-tools-$(uname -r)
    ```

---

## Como Compilar e Executar

Siga estes 4 passos para rodar o benchmark.

### Passo 1: Preparar os Datasets

O benchmark requer dois datasets distintos: um para a árvore e outro para o grafo.

**1.1. Gerar o Dataset da Árvore:**
Compile e execute o utilitário `generate_data.cpp` para criar um arquivo binário (`tree_data.bin`) com 50 milhões de inteiros (aprox. 400 MB).

```sh
g++ -std=c++17 -O3 generate_data.cpp -o generate_data
./generate_data
```

**1.2. Baixar o Dataset do Grafo:**
Baixe o dataset `soc-Slashdot0811` da Stanford Network Analysis Platform (SNAP).

* **Link para download:** [soc-Slashdot0811.txt.gz](http://snap.stanford.edu/data/soc-Slashdot0811.txt.gz)

Descompacte o arquivo e garanta que ele se chame `soc-Slashdot0811.txt`, colocando-o no mesmo diretório do projeto.

```sh
# Comando para baixar e descompactar via terminal
wget [http://snap.stanford.edu/data/soc-Slashdot0811.txt.gz](http://snap.stanford.edu/data/soc-Slashdot0811.txt.gz)
gunzip soc-Slashdot0811.txt.gz
```

### Passo 2: Compilar o Benchmark Principal

Compile todos os arquivos `.cpp` do projeto usando a flag `-std=c++23` para habilitar `std::print` e outras funcionalidades modernas.

```sh
g++ -std=c++23 -O3 -g main.cpp Tree.cpp Graph.cpp -o performance_counter
```

* `-std=c++23`: Define o padrão da linguagem C++.
* `-O3`: Habilita o nível máximo de otimização do compilador.
* `-g`: Inclui símbolos de depuração, que ajudam o `perf` a fornecer relatórios mais detalhados.

### Passo 3: Executar a Análise com `perf`

Agora, execute o programa compilado através do `perf stat`. Este comando irá rodar o seu programa e, ao final, exibirá um resumo dos contadores de performance do hardware.

```sh
perf stat -e cache-references,cache-misses,cycles,instructions,branches,branch-misses ./performance_counter
```

* A flag `-e` especifica os eventos que queremos monitorar:
    * `cache-references`: Número de acessos à cache.
    * `cache-misses`: Número de vezes que o dado não foi encontrado na cache (requer busca na RAM).
    * `cycles`, `instructions`: Ciclos de CPU e instruções executadas (permite calcular o IPC - Instruções por Ciclo).
    * `branches`, `branch-misses`: Medem a eficiência da previsão de desvio da CPU.