#pragma once
#include <queue>


// verified at:
// https://atcoder.jp/contests/abc289/submissions/40441513
// slope : ascending order
// query : descending order
// min query
class ConvexHullTrick {
    using Line = std::pair<long long, long long>;
    std::deque<Line> lines;

    bool check(const Line &l1, const Line &l2, const Line &l3) {
        return (l3.second-l2.second) * (l2.first-l1.first) 
            >= (l2.second-l1.second) * (l3.first-l2.first);
    }

    long long f(long long x, Line &l) {
        return l.first * x + l.second;
    }
public:
    ConvexHullTrick() {}

    void add_line(long long a, long long b) {
        Line lmin = std::make_pair(a, b);
        while (lines.size() >= 2 && check(lines[lines.size()-2], lines[lines.size()-1], lmin))
            lines.pop_back();
        lines.push_back(std::make_pair(a, b));
    }

    long long query(long long x) {
        while (lines.size() >= 2 && f(x, lines[0]) >= f(x, lines[1]))
            lines.pop_front();
        return f(x, lines.front());
    }
};