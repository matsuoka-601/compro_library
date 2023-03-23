#pragma once
#include <vector>
#include <assert.h>
#include <string>
#include <functional>
#include <stack>

// Multi-way Trie
template <int CHAR_SIZE>
class TrieNode {
public: 
    static constexpr int INVALID_NODE = -1;
    int nxt[CHAR_SIZE];
    std::vector<int> accept;
    int subtree_str_count;

    TrieNode(): subtree_str_count(0) {
        for (int i = 0; i < CHAR_SIZE; i++)
            nxt[i] = INVALID_NODE;
    }

};

template<int CHAR_SIZE, int MARGIN>
class Trie {
    using Node = TrieNode<CHAR_SIZE>;

    std::vector<Node> nodes;
    static constexpr int ROOT_NODE = 0;

    void sort_dfs(std::vector<int> &result_index, int cur_node_index) 
    const {
        for (auto &id: nodes[cur_node_index].accept)
            result_index.push_back(id);
        for (int c = 0; c < CHAR_SIZE; c++) 
            if (nodes[cur_node_index].nxt[c] != Node::INVALID_NODE)
                sort_dfs(result_index, nodes[cur_node_index].nxt[c]);
    }
public:
    Trie() {
        nodes.push_back(Node());
    }

    void add(const std::string &str) {
        const int id = nodes[ROOT_NODE].subtree_str_count;
        int cur_node_index = ROOT_NODE;
        for (auto &c_: str) {
            const int c = c_ - MARGIN;
            nodes[cur_node_index].subtree_str_count++;
            if (nodes[cur_node_index].nxt[c] == Node::INVALID_NODE) {
                nodes[cur_node_index].nxt[c] = (int)nodes.size();
                nodes.push_back(Node());
            }
            cur_node_index = nodes[cur_node_index].nxt[c];
        }
        nodes[cur_node_index].accept.push_back(id);
        nodes[cur_node_index].subtree_str_count++;
    }

    void query(const std::string &str, 
        const std::function< void (int) > &f) const 
    {
        int cur_node_index = ROOT_NODE;
        for (auto &c_: str) {
            for (auto &idx: nodes[cur_node_index].accept)
                f(idx);
            const int c = c_ - MARGIN;
            if (nodes[cur_node_index].nxt[c] == Node::INVALID_NODE)
                return;
            cur_node_index = nodes[cur_node_index].nxt[c];
        }
        for (auto &idx: nodes[cur_node_index].accept)
            f(idx);
    }

    std::vector<int> sort(const std::vector<std::string> &str_vec) 
    {
        // TODO : write
        assert(nodes[ROOT_NODE].subtree_str_count == 0);
        std::vector<int> result;
        for (auto &str: str_vec)
            add(str);
        sort_dfs(result, ROOT_NODE);
        return result;
    }

    int node_count() const {
        return (int)nodes.size();
    }
};