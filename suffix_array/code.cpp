#include "bits/stdc++.h"
using namespace std;

struct suff_t {
    const char* s;
    int n;
    vector<int> p, rnk;
    suff_t(const string& in)
        : s(in.c_str()), n(in.size()), p(n), rnk(n) {
        vector<int> aux(n);
        vector<int> F(3 +
                      max<int>(numeric_limits<char>::max(), n - 1));
        for (int i = 0; i < n; i++) p[i] = i, rnk[i] = s[i];
        for (int d = 1; 0 < n; d <<= 1) {
            stable_sort(d, F, aux);
            stable_sort(0, F, aux);
            aux[p[0]] = 0;
            for (int i = 1, j = 0; i < n; i++, j++)
                aux[p[i]] = aux[p[j]] +
                            (to_pair(p[i], d) != to_pair(p[j], d));
            copy(begin(aux), end(aux), begin(rnk));
            if (rnk[p[n - 1]] == n - 1) break;
        }
    }
    const char* operator[](int i) const { return s + p[i]; }
    int operator()(int i) const { return rnk[i]; }
    bool query(const char* q) const {
        auto it = lower_bound(begin(p), end(p), q,
                              [&](int i, const char* c) {
                                  return strcmp(s + i, c) < 0;
                              });
        for (size_t d = 0; *q; d++, q++)
            if (s[*it + d] != *q) return false;
        return true;
    }
    tuple<int, int, int> traverse(tuple<int, int, int> curr,
                                  char q) const {
        int lo, hi, d;
        tie(lo, hi, d) = curr;
        lo =
            lower_bound(&p[0] + lo, &p[0] + hi, q,
                        [&](int i, char c) { return s[i + d] < c; }) -
            &p[0];
        hi =
            upper_bound(&p[0] + lo, &p[0] + hi, q,
                        [&](char c, int i) { return c < s[i + d]; }) -
            &p[0];
        return {lo, hi, d + 1};
    }

  private:
    int at(int i) { return i < n ? rnk[i] : -1; }
    pair<int, int> to_pair(int i, int d) {
        return {at(i), at(i + d)};
    }
    void stable_sort(int d, vector<int>& F, vector<int>& spc) {
        fill(begin(F), end(F), 0);
        for (int i : p) F[at(i + d) + 2]++;
        partial_sum(begin(F), end(F), begin(F));
        for (int i : p) spc[F[at(i + d) + 1]++] = i;
        copy(begin(spc), end(spc), begin(p));
    }
};

struct lcp_t {
    int n;
    vector<int> tree;
    lcp_t(const suff_t& sa) {
        n = sa.n;
        tree = vector<int>(2 * n);
        int h = 0;
        for (int i = 0; i < n; i++)
            if (sa(i) == n - 1)
                tree[n + sa(i)] = h = 0;
            else {
                for (int j = sa.p[sa(i) + 1];
                     i + h < n && j + h < n &&
                     sa.s[i + h] == sa.s[j + h];
                     h++)
                    ;
                tree[n + sa(i)] = h;
                h -= (!!h);
            }
        for (int i = n - 1; i + 1; i--)
            tree[i] = min(tree[i << 1], tree[i << 1 | 1]);
    }
    int operator[](int i) const { return tree[n + i]; }
    int operator()(int l, int r) const {
        int ans = INT_MAX;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) ans = min(ans, tree[l++]);
            if (r & 1) ans = min(ans, tree[--r]);
        }
        return ans;
    }
};

string parse_bits(const uint8_t* data, size_t size) {
    string ans;
    ans.reserve(size);
    for (size_t i = 0; i < size; i++) {
        auto c = static_cast<char>(data[i]);
        if (isprint(c)) ans.push_back(c);
    }
    return ans;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data,
                                      size_t size) {
    string s = parse_bits(data, size);
    suff_t sa(s);
    lcp_t lcp(sa);

    const char* send = &s[0] + s.size();
    for (size_t i = 0; i + 1 < sa.n; ++i) {
        assert(strcmp(sa[i], sa[i + 1]) < 0);

        size_t _lcp = 0;
        for (const char *si = sa[i], *sj = sa[i + 1];
             si != send && sj != send && *si == *sj;
             si++, sj++, _lcp++)
            ;
        assert(_lcp == lcp[i]);
    }

    return 0;
}
