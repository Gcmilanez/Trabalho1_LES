CXX := g++
CXXFLAGS := -std=c++23 -O3 -g -march=native -Wall -Wextra -Wno-unused-parameter

SRCS_TREE := Tree.cpp
SRCS_GRAPH := Graph.cpp

all: bst_bench graph_matrix_bench graph_list_bench graph_hash_bench bst_iter_bench avl_bench

bst_bench: bst_bench.cpp bench_utils.h $(SRCS_TREE)
	$(CXX) $(CXXFLAGS) bst_bench.cpp $(SRCS_TREE) -o $@

graph_matrix_bench: graph_matrix_bench.cpp bench_utils.h $(SRCS_GRAPH)
	$(CXX) $(CXXFLAGS) graph_matrix_bench.cpp $(SRCS_GRAPH) -o $@

graph_list_bench: graph_list_bench.cpp bench_utils.h $(SRCS_GRAPH)
	$(CXX) $(CXXFLAGS) graph_list_bench.cpp $(SRCS_GRAPH) -o $@

graph_hash_bench: graph_hash_bench.cpp bench_utils.h $(SRCS_GRAPH)
	$(CXX) $(CXXFLAGS) graph_hash_bench.cpp $(SRCS_GRAPH) -o $@

bst_iter_bench: bst_iter_bench.cpp bench_utils.h TreeVariants.h
	$(CXX) $(CXXFLAGS) bst_iter_bench.cpp -o $@

avl_bench: avl_bench.cpp bench_utils.h TreeVariants.h
	$(CXX) $(CXXFLAGS) avl_bench.cpp -o $@

clean:
	rm -f bst_bench graph_matrix_bench graph_list_bench graph_hash_bench bst_iter_bench avl_bench

.PHONY: all clean
