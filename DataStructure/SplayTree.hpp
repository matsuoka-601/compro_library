#pragma once
#include <assert.h>
#include <tuple>

// this implementation is based on
// https://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-854j-advanced-algorithms-fall-2008/lecture-notes/lec6.pdf

template<class T> class SplayTree {
    struct Node {
        Node *left, *right, *par;
        T value;
        Node(T _value) : left(nullptr), right(nullptr), par(nullptr), value(_value) {}
        bool isRoot() const {
            return (!par);
        }
    };
    Node* root; // There's always one root.
    int tree_size;

    void rotate(Node* u, bool right) {
        Node* p = u->par;
        Node* gp = p->par;
        if (right) {
            if ((p->left = u->right)) u->right->par = p;
            u->right = p, p->par = u;
        } else {
            if ((p->right = u->left)) u->left->par = p;
            u->left = p, p->par = u;
        }
        u->par = gp;
        if(!gp) return;
        if (gp->left == p) gp->left = u;
        if (gp->right == p) gp->right = u;
    }

    Node* splay(Node* u) {
        while (!u->isRoot()) {
            Node* p = u->par;
            Node* gp = p->par;
            if (p->isRoot()) { // zig
                rotate(u, u == p->left);
            } else {
                bool flag = (u == p->left);
                if ((u == p->left) == (p == gp->left)) { // zig-zig
                    rotate(p, flag), rotate(u, flag);
                } else { // zig-zag
                    rotate(u, flag), rotate(u, !flag);
                }
            }
        }
        return u;
    }

    Node* find_node(Node* node, T x) {
        Node* last = nullptr;
        while (node) {
            last = node;
            if (node->value == x) return splay(node);
            else if (node->value > x) node = node->left;
            else if (node->value < x) node = node->right;
        }
        return splay(last);
    }

    Node* find_max_node(Node* node) {
        Node* last = nullptr;
        while (node) {
            last = node;
            node = node->right;
        }
        return splay(last);
    }

    // The elements of the tree rooted at root1 should be smaller 
    // than the elements of the tree rooted at root2.
    void join(Node* root1, Node* root2) {
        if (!root1) { root = root2; return; }
        if (!root2) { root = root1; return; }
        root = find_max_node(root1);
        connect(root, root2, true);
    }

    // Given a pivot x, split operation partitions the tree into two BSTs.
    // T1 : {node|node->value <= x}
    // T2 : {node|node->value > x}
    std::pair<Node*, Node*> split(T x) {
        if (!root) return std::make_pair(nullptr, nullptr);
        root = find_node(root, x);
        if (root->value <= x) return std::make_pair(root, disconnect(root, true));
        else return std::make_pair(disconnect(root, false), root);
    }

    // This safely disconnects two nodes
    Node* disconnect(Node* node, bool right) {
        assert(node);
        if (right) {
            Node* right_node = node->right;
            if (!right_node) return nullptr;
            right_node->par = nullptr;
            node->right = nullptr;
            return right_node;
        } else {
            Node* left_node = node->left;
            if (!left_node) return nullptr;
            left_node->par = nullptr;
            node->left = nullptr;
            return left_node;
        }
    }

    // This safely connects two nodes
    void connect(Node* node, Node* to_node, bool right) {
        assert(node);
        if (!to_node) return;
        if (right) {
            node->right = to_node;
            to_node->par = node;
        } else {
            node->left = to_node;
            to_node->par = node;
        }
    }

public:
    SplayTree() {
        root = nullptr;
        tree_size = 0;
    }

    bool find(T x) {
        if (!root) return false;
        root = find_node(root, x);
        return (root->value == x);
    }

    void insert(T x) {
        Node *left_root, *right_root;
        std::tie(left_root, right_root) = split(x);
        // Duplication of values is not allowed.
        if (left_root && (left_root->value == x)) { connect(left_root, right_root, true); return; }
        root = new Node(x);
        connect(root, left_root, false), connect(root, right_root, true);
        tree_size += 1;
    }

    void erase(T x) {
        if (!root || !find(x)) return;
        // At this point, a node with value x is root.
        Node* left_root = disconnect(root, false);
        Node* right_root = disconnect(root, true);
        delete root;
        join(left_root, right_root);
        tree_size -= 1;
    }

    int size() {
        return tree_size;
    }
};