#include <iostream>
#include <vector>

using namespace std;

template <class T> class SplayTree {
    struct Node {
        Node *left, *right;
        T key;
    };

    enum dir_t {
        LEFT,
        RIGHT,
    };


    Node* splay(vector<pair<Node*, dir_t>> path) {
        dir_t dir, pdir;
        Node* node, pnode;
        while (path.size() > 1) {
            tie(node, dir) = path.pop_back();
            tie(pnode, pdir) = path.pop_back();
            if (dir == pdir) { // zig-zig step
                if (dir == LEFT) {

                } else if(dir == RIGHT) {

                }
            } else if (dir != pdir){ // zig-zag step
                if (dir == LEFT) {

                } else if(dir == RIGHT) {

                }
            }
        }
        if (path.size() == 0) return node;
        tie(node, dir) = path.pop_back();
        if (dir == LEFT){

        } else {

        }
        return node;
    }

    // we assume the tree is not empty (checked at find)
    Node* find_node(Node* node, T x) {
        vector<pair<Node*, dir_t>> path;
        dir_t dir;
        while (node) {
            if (node.key == x) return splay(node, path);
            if (x < node.key) {
                path.push_back({node, LEFT});
                node = node.left;
            } else if(x > node.key) {
                path.push_back({node, RIGHT});
                node = node.right;
            }
        }
        tie(node, dir) = path.pop_back();
        return splay(path);
    }

    Node* root;
public:
    bool find(T x) {
        if (!root) return false;
        Node *node = find_node(root, x);
        return (node->key == x);
    }
};