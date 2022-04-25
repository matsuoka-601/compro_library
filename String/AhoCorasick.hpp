#pragma once 
#include <vector>
#include <string>
#include <map>
#include <queue>
#include "Trie.hpp"

template <int char_size, int margin> class AhoCorasick : 
    public Trie<char_size + 2, margin> {

    using Node = typename Trie<char_size + 2, margin>::Node;
    using Trie<char_size + 2, margin>::nodes;
    using Trie<char_size + 2, margin>::root;
    const int FAIL = char_size;
    const int OUTPUT = char_size + 1;

    void build() {
        std::queue<int> que;

        // nodes[root].nxt[FAIL] is root
        // nodes[root].nxt[OUTPUT] is NULLNODE
        for (int i = 0; i <= FAIL; i++) {
            int nxt_node = nodes[root].nxt[i];
            if (nxt_node != Node::NULLNODE) {
                nodes[nxt_node].nxt[FAIL] = root; // set failure link of the next node
                nodes[nxt_node].nxt[OUTPUT] = Node::NULLNODE; // set output link of the next node
                que.push(nxt_node);
            } else {
                nodes[root].nxt[i] = root; 
            }
        }

        while(!que.empty()) {
            auto &cur_node = nodes[que.front()]; que.pop();
            for (int i = 0; i < char_size; i++) {
                if (cur_node.nxt[i] == Node::NULLNODE) continue;
                auto &nxt_node = nodes[cur_node.nxt[i]];

                // set failure link of the next node
                int fail = cur_node.nxt[FAIL];
                // if fail is root, nodes[fail].nxt[i] is not NULLNODE because of the initialization above.
                while (nodes[fail].nxt[i] == Node::NULLNODE) 
                    fail = nodes[fail].nxt[FAIL];
                nxt_node.nxt[FAIL] = nodes[fail].nxt[i];


                // set output link of the next node
                if (this->is_accept(nxt_node.nxt[FAIL])) nxt_node.nxt[OUTPUT] = nxt_node.nxt[FAIL];
                else nxt_node.nxt[OUTPUT] = nodes[nxt_node.nxt[FAIL]].nxt[OUTPUT]; 

                que.push(cur_node.nxt[i]);
            }
        }
    }

public:

    AhoCorasick(std::vector<std::string> &patterns) {
        this->add(patterns);
        build();
    }

    std::map<std::pair<int,int>, std::vector<int> > match(std::string &str) {
        std::map<std::pair<int,int>, std::vector<int> > result;
        auto f = [&](int node_index, int str_index, int accept_id) {
            result[std::make_pair(str_index - nodes[node_index].depth, str_index)].push_back(accept_id);
        };
        query(str, f);
        return result;
    }

    void query(const std::string &str, const std::function<void(int, int, int)> &f) {
        int node_index = 0;
        int str_index = 0;
        for (auto &c : str) {
            int i = c - margin;
            while (nodes[node_index].nxt[i] == Node::NULLNODE) node_index = nodes[node_index].nxt[FAIL];
            node_index = nodes[node_index].nxt[i];

            int node_index_tmp = node_index;
            while (node_index_tmp != Node::NULLNODE) {
                for (auto &accept_id : nodes[node_index_tmp].accept_ids) f(node_index_tmp, str_index, accept_id);
                node_index_tmp = nodes[node_index_tmp].nxt[OUTPUT];
            }
            str_index++;
        }
    }

};