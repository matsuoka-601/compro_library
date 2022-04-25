#pragma once
#include <vector>
#include <cstring>
#include <string>
#include <functional>
#include <iostream>

template <int char_size> class TrieNode {
public:
    int nxt[char_size];

    static const int NULLNODE = -1;

    int exist, depth;
    std::vector<int> accept_ids; // one node might have multiple ids

    TrieNode(int depth): exist(0), depth(depth) {
        memset(nxt, NULLNODE, sizeof(nxt));
    }
};

template <int char_size, int margin> class Trie {

    void update_exist(int node_index) {
        ++nodes[node_index].exist;
    }

    void update_cur_node(int node_index, int id) { 
        nodes[node_index].accept_ids.push_back(id);
    }

    void add(const std::string &str, int str_index, int node_index, int id) {
        if (str_index == str.size()) update_cur_node(node_index, id);
        else {
            const int c = str[str_index] - margin;
            if (nodes[node_index].nxt[c] == Node::NULLNODE) {
                nodes[node_index].nxt[c] = (int)nodes.size();
                nodes.push_back(Node(str_index));
            }
            add(str, str_index + 1, nodes[node_index].nxt[c], id);
            update_exist(node_index);
        }
    }

public:
    using Node = TrieNode<char_size>;
    std::vector<Node> nodes;
    const int root;

    Trie() : root(0) {
        nodes.push_back(Node(-1));
    }

    void add(const std::string &str, int id) {
        add(str, 0, root, id);
    }

    void add(const std::string &str) { 
        add(str, nodes[0].exist);
    }

    void add(const std::vector<std::string> &patterns) {
        for (auto &str : patterns) add(str);
    }

    // void query(const std::string &str, const std::function<void(int)> &f, int str_index, int node_index) {
    //     for (auto &idx : nodes[node_index].accept) f(idx);

    //     if (str_index == str.size()) return;
    //     else {
    //         const int c = str[str_index] - margin;
    //         if (nodes[node_index].nxt[c] == Node::NULLNODE) return;
    //         query(str, f, str_index + 1, nodes[node_index].nxt[c]);
    //     }
    // }

    // void query(const std::string &str, const std::function<void(int)> &f) {
    //     query(str, f, 0, root);
    // }

    int size() const { // return the number of nodes
        return (int)nodes.size();
    }

    int count() const {
        return nodes[root].exist;
    }

    int is_accept(int node_index) const {
        return nodes[node_index].accept_ids.size() > 0;
    }
};