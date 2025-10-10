#ifndef TREE_H
#define TREE_H

#include <cstdint>

// Estrutura do Nó da Árvore
struct Node {
    uint64_t key;
    Node *left;
    Node *right;
    Node(uint64_t k);
};

// Árvore de Busca Binária (BST)
class BST {
public:
    BST();
    ~BST();
    void insert(uint64_t key);
    bool search(uint64_t key);

private:
    Node* root;
    Node* insert_recursive(Node* node, uint64_t key);
    bool search_recursive(Node* node, uint64_t key);
    void destroy_recursive(Node* node);
};

#endif // TREE_H