#pragma once
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>


class SuffixArray
{

    std::vector<int> sort_cyclic_shifts(const std::string &s)
    {
        int n = s.size();

        std::vector<int> cn(n), pn(n), p(n), c(n), cnt(n);
        std::iota(p.begin(), p.end(), 0);
        std::sort(p.begin(), p.end(), [&](int a, int b)
                  { return s[a] == s[b] ? a < b : s[a] < s[b]; });
        c[p[0]] = 0;
        for (int i = 1; i < n; i++) c[p[i]] = (s[p[i]] == s[p[i - 1]]) ? c[p[i - 1]] : i;

        for (int len = 1; len < n; len <<= 1)
        {
            for (int i = 0; i < n; i++) {
                pn[i] = p[i] - len;
                if (pn[i] < 0) pn[i] += n;
            }

            std::iota(cnt.begin(), cnt.end(), 0);
            for (int i = 0; i < n; i++) p[cnt[c[pn[i]]]++] = pn[i];

            if (2 * len >= n) break;

            c[p[0]] = 0;
            for (int i = 1; i < n; i++)
            {
                if (c[p[i]] == c[p[i - 1]] && c[(p[i] + len) % n] == c[(p[i - 1] + len) % n])
                    cn[p[i]] = cn[p[i - 1]];
                else
                    cn[p[i]] = i;
            }
            std::swap(c, cn);
        }
        return p;
    }

public:
    std::vector<int> sa;
    SuffixArray(std::string &s)
    {
        s.push_back('$');
        sa = sort_cyclic_shifts(s);
        sa.erase(sa.begin());
        s.pop_back();
    }
};