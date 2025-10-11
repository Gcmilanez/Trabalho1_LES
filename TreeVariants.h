#ifndef TREE_VARIANTS_H
#define TREE_VARIANTS_H

#include <cstdint>
#include <vector>
#include <memory>
#include <algorithm>

// ------------ Iterative BST (if-then-else style, no recursion) ------------
class BSTIter {
public:
    BSTIter() : root(nullptr) {}
    ~BSTIter() { destroy(root); }

    void insert(uint64_t key) {
        if (!root) { root = new Node(key); return; }
        Node* cur = root;
        for (;;) {
            if (key < cur->key) {
                if (cur->left) cur = cur->left;
                else { cur->left = new Node(key); break; }
            } else if (key > cur->key) {
                if (cur->right) cur = cur->right;
                else { cur->right = new Node(key); break; }
            } else {
                break; // ignore duplicates
            }
        }
    }

    bool search(uint64_t key) const {
        Node* cur = root;
        while (cur) {
            if (key == cur->key) return true;
            if (key < cur->key)  cur = cur->left;
            else                 cur = cur->right;
        }
        return false;
    }

private:
    struct Node {
        uint64_t key;
        Node* left;
        Node* right;
        explicit Node(uint64_t k) : key(k), left(nullptr), right(nullptr) {}
    };
    Node* root;

    static void destroy(Node* n) {
        if (!n) return;
        // iterative post-order to avoid recursion depth
        std::vector<Node*> stack;
        Node* last = nullptr;
        while (n || !stack.empty()) {
            if (n) {
                stack.push_back(n);
                n = n->left;
            } else {
                Node* peek = stack.back();
                if (peek->right && last != peek->right) {
                    n = peek->right;
                } else {
                    stack.pop_back();
                    last = peek;
                    delete peek;
                }
            }
        }
    }
};

// ----------------------------- AVL Tree (balanced BST) -----------------------------
class AVLTree {
public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() { destroy(root); }

    void insert(uint64_t key) {
        root = insert_rec(root, key);
    }

    bool search(uint64_t key) const {
        Node* cur = root;
        while (cur) {
            if (key == cur->key) return true;
            if (key < cur->key) cur = cur->left;
            else cur = cur->right;
        }
        return false;
    }

private:
    struct Node {
        uint64_t key;
        int height;
        Node* left;
        Node* right;
        explicit Node(uint64_t k) : key(k), height(1), left(nullptr), right(nullptr) {}
    };
    Node* root;

    static int h(Node* n) { return n ? n->height : 0; }
    static int balance(Node* n) { return n ? h(n->left) - h(n->right) : 0; }
    static void upd(Node* n) { if (n) n->height = 1 + std::max(h(n->left), h(n->right)); }

    static Node* rotate_right(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        upd(y); upd(x);
        return x;
    }
    static Node* rotate_left(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        upd(x); upd(y);
        return y;
    }

    static Node* insert_rec(Node* node, uint64_t key) {
        if (!node) return new Node(key);
        if (key < node->key) node->left = insert_rec(node->left, key);
        else if (key > node->key) node->right = insert_rec(node->right, key);
        else return node; // ignore duplicates

        upd(node);
        int bf = balance(node);

        // LL
        if (bf > 1 && key < node->left->key)
            return rotate_right(node);
        // RR
        if (bf < -1 && key > node->right->key)
            return rotate_left(node);
        // LR
        if (bf > 1 && key > node->left->key) {
            node->left = rotate_left(node->left);
            return rotate_right(node);
        }
        // RL
        if (bf < -1 && key < node->right->key) {
            node->right = rotate_right(node->right);
            return rotate_left(node);
        }
        return node;
    }

    static void destroy(Node* n) {
        if (!n) return;
        std::vector<Node*> st; Node* last=nullptr;
        while (n || !st.empty()) {
            if (n) { st.push_back(n); n = n->left; }
            else {
                Node* p = st.back();
                if (p->right && last != p->right) { n = p->right; }
                else { st.pop_back(); last = p; delete p; }
            }
        }
    }
};

#endif // TREE_VARIANTS_H
