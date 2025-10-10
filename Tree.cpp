#include "Tree.h"
#include <iostream>

// --- Implementação do Nó ---
Node::Node(uint64_t k) : key(k), left(nullptr), right(nullptr) {}

// --- Implementação da BST ---
BST::BST() : root(nullptr) {}

BST::~BST() {
    destroy_recursive(root);
}

void BST::destroy_recursive(Node* node) {
    if (node) {
        destroy_recursive(node->left);
        destroy_recursive(node->right);
        delete node;
    }
}

void BST::insert(uint64_t key) {
    root = insert_recursive(root, key);
}

Node* BST::insert_recursive(Node* node, uint64_t key) {
    if (node == nullptr) {
        return new Node(key);
    }

    if (key < node->key) {
        node->left = insert_recursive(node->left, key);
    } else if (key > node->key) {
        node->right = insert_recursive(node->right, key);
    }
    // Chaves iguais não são inseridas

    return node;
}

bool BST::search(uint64_t key) {
    return search_recursive(root, key);
}

bool BST::search_recursive(Node* node, uint64_t key) {
    if (node == nullptr) {
        return false;
    }

    if (key == node->key) {
        return true;
    }

    return key < node->key ? search_recursive(node->left, key) : search_recursive(node->right, key);
}