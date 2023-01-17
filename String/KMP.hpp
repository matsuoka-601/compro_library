#pragma once
#include <string>
#include <vector>

std::vector<int> compute_prefix_function(const std::string &pattern) {
    const int m = (int)pattern.size();
    std::vector<int> prefix_function(m+1);

    prefix_function[0] = -1;
    prefix_function[1] = 0;
    int k = 0;
    for (int i = 1; i < m; i++) {
        while (k > -1 && pattern[k] != pattern[i])
            k = prefix_function[k];
        prefix_function[i+1] = ++k;
    }

    return prefix_function;
}

std::vector<int> kmp_matcher(
    const std::string &text, 
    const std::string &pattern
) {
    const int n = (int)text.size();
    const int m = (int)pattern.size();

    std::vector<int> prefix_function = compute_prefix_function(pattern);

    std::vector<int> matched_index;
    int q = 0;
    for (int i = 0; i < n; i++) {
        while (q > -1 && pattern[q] != text[i]) 
            q = prefix_function[q];
        ++q;
        if (m == q) {
            matched_index.push_back(i-q+1);
            q = prefix_function[q];
        }
    }

    return matched_index;
}