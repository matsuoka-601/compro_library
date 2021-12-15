#pragma once
// This implementation is based on 
// https://megalodon.jp/2021-1214-1128-17/https://kopricky.github.io:443/code/DataStructure_OnGraph/link_cut_tree.html

// verified at
// https://atcoder.jp/contests/joisc2013-day4/submissions/27910218
template<class T> class LinkCutTree {

    struct Node {
        Node *left, *right, *par;
        int id;
        T value;
        Node(int _id) : left(nullptr), right(nullptr), par(nullptr), id(_id), value(T()) {}
        // A node is root when it is the root of an auxiliary tree
        bool isRoot() const {
            return (!par) || (par->left != this && par->right != this);
        }
    };

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

    // cut cuts link between u and u->par
    void cut(Node *u) {
        access(u);
        Node* lc = u->left;
        lc->par = nullptr, u->left = nullptr;
    }

    Node* access(Node* u) {
        Node* last = nullptr;
        for (Node* v = u; v; v = v->par) {
            splay(v);
            v->right = last;
            last = v;
        }
        // With this splay, it is guaranteed that u is the root 
        // in the auxiliary tree after access(u).
        splay(u);  
        return last; // returning last is useful when calculating lca
    }

    void splay(Node* u) {
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
    }

    // link lets u the parent of v
    // v should be the root of the represented tree
    void link(Node* u, Node* v) {
        access(u), access(v);
        u->right = v, v->par = u;
    }

    // It is assumed that u and v is connected.
    // Therefore, if there's a possibility that u and v is not connected,
    // call connected(u, v) before calling lca(u, v).
    Node* lca(Node* u, Node* v) {
        access(u);
        return access(v);
    }

    bool connected(Node *u, Node *v) {
        access(u), access(v);
        return u == v || u->par;
    }

public:
    Node** arr;
    LinkCutTree(int node_size) {
        arr = new Node*[node_size];
        for (int i = 0; i < node_size; i++)
            arr[i] = new Node(i);
    }

    void link(int id1, int id2) { link(arr[id1], arr[id2]); }
    void cut(int id1) { cut(arr[id1]); }
    int lca(int id1, int id2) { return lca(arr[id1], arr[id2])->id; }
    bool connected(int id1, int id2) { return connected(arr[id1], arr[id2]); }
};


