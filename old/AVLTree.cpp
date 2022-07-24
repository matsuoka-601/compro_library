#include <iostream>
#include <cassert>

using namespace std;


template<class T> class AVLTree{
    struct Node{
        Node *left, *right, *par;
        T key;
        int height, size;

        Node(T x) {
            left = right = nullptr;
            height = 1;
            key = x;
            size = 1;
        }
    };

    Node* root;

public:
    AVLTree() {
        root = nullptr;
    }

    bool find(T x) {
        if (!root) {
            return false;
        }

        Node *node = find_node(root, x);
        return (node->key == x);
    }

    T at(int k) { // we assume k is 0-indexed
        if (!root) {
            return 0;
        }
        // assert(0 <= k && k < root->size);
        
        Node *node = root;
        k++;
        while(1) {
            int lsize = (node->left ? node->left->size : 0) + 1;
            if (lsize == k)
                break;

            if (k < lsize) {
                node = node->left;
            } else {
                node = node->right;
                k -= lsize;
            }
        }

        return node->key;
    }

    bool insert(T x) {
        if (!root) {
            root = new Node(x);
            return true;
        }

        Node *node = find_node(root, x);
        if (node->key == x)
            return false;
        
        Node *new_node = new Node(x);
        new_node->par = node;
        if (x < node->key) {
            node->left = new_node;
        } else {
            node->right = new_node;
        }
        node = new_node;


        while (node->par) {
            Node* par = node->par;
            par->size++;
            par->update_height();
            // This is a compromise to shorten the code.
            // With this implementation, this loop always goes back to root node.
            // However, by carefully examining each case, we can sometimes break from this loop
            // to reduce the number of computation.
            // see https://megalodon.jp/2021-1110-0016-22/https://tjkendev.github.io:443/procon-library/cpp/binary_search_tree/avl-tree.html
            node = par->balance(); 
        }

        root = node;
        return true;
    }
};


int main() {

}