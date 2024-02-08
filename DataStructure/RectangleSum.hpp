#pragma once
#include "DataStructure/BIT.hpp"
#include "DataStructure/Compress.hpp"
#include <variant>
#include <queue>

namespace RectangleSum {
    typedef struct {
        int x, y, w;
    } Point;

    typedef struct {
        int l, d, r, u;
    } Rect;

    typedef struct {
        int r, u;
        bool neg;
        int idx;
    } PartRect;
};


std::vector<long long> StaticRectangleSum(std::vector<RectangleSum::Point> &points, std::vector<RectangleSum::Rect> &queries) {
    std::vector<long long> ans(queries.size());

    std::vector<int> tmpy(points.size());
    for (int i = 0; i < points.size(); i++) tmpy[i] = points[i].y;

    Compress<int> cmp(tmpy);
    BIT<long long> bit(cmp.size());

    // 最初に bit に全部追加する
    for (int i = 0; i < points.size(); i++) bit.add(cmp.lb(points[i].y), points[i].w);

    std::vector<RectangleSum::PartRect> que(4 * queries.size());
    for (int i = 0; i < queries.size(); i++) {
        que[4*i] = {queries[i].r, queries[i].u, 0, i};
        que[4*i + 1] = {queries[i].l, queries[i].d, 0, i};
        que[4*i + 2] = {queries[i].l, queries[i].u, 1, i};
        que[4*i + 3] = {queries[i].r, queries[i].d, 1, i};
    }

    sort(que.begin(), que.end(), [&](auto &fi, auto &se) { return fi.r > se.r; } );
    sort(points.begin(), points.end(), [&](auto &fi, auto &se) { return fi.x > se.x; });

    int point_idx = 0;
    for (int i = 0; i < que.size(); i++) {
        int curr = que[i].r;
        int curu = que[i].u;
        while (point_idx < points.size() && points[point_idx].x >= curr) 
            bit.add(cmp.lb(points[point_idx].y), -points[point_idx].w), point_idx++;
        ans[que[i].idx] += (que[i].neg ? -1LL : 1LL) * bit.query(0, cmp.lb(curu));
    }
    return ans;
}

using RectangleSumQuery = std::variant< RectangleSum::Point, RectangleSum::Rect >;

std::vector<long long> DynamicRectangleSum(std::vector<RectangleSumQuery> &query) {
    size_t Q = query.size();

    std::vector<int> query_idxs(Q);

    int sum_cnt = 0;
    for (int i = 0; i < Q; i++) 
        if (std::holds_alternative<RectangleSum::Rect>(query[i]))
            query_idxs[i] = sum_cnt++;

    std::vector<long long> ans(sum_cnt);

    std::queue< std::pair<int, int> > que;
    que.emplace(0, Q);
    while (!que.empty()) {
        auto [l, r] = que.front(); que.pop();
        if (r - l == 1) continue;

        int m = (l + r) / 2;
        que.emplace(l, m), que.emplace(m, r);

        std::vector<RectangleSum::Point> points;
        for (int i = l; i < m; i++)
            if (std::holds_alternative<RectangleSum::Point>(query[i]))
                points.push_back(std::get<RectangleSum::Point>(query[i]));

        std::vector<RectangleSum::Rect> sum;
        for (int i = m; i < r; i++)
            if (std::holds_alternative<RectangleSum::Rect>(query[i]))
                sum.push_back(std::get<RectangleSum::Rect>(query[i]));

        auto tmp = StaticRectangleSum(points, sum);

        int cur = 0;
        for (int i = m; i < r; i++)
            if (std::holds_alternative<RectangleSum::Rect>(query[i]))
                ans[query_idxs[i]] += tmp[cur++];
    }

    return ans;
}