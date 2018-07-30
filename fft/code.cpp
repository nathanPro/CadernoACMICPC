#include "bits/stdc++.h"
using namespace std;
using ll = int64_t;

template <typename Field> struct dft {
    using F = typename Field::F;
    template <typename It> dft(It a, int n, bool inv = false) {
        assert((n & (n - 1)) == 0);
        {
            vector<size_t> p(n);
            for (int i = 0; i < n; i++)
                p[i] = (p[i >> 1] >> 1) | ((i & 1) * (n / 2));
            for (auto i : p)
                if (i < p[i]) swap(a[i], a[p[i]]);
        }
        F r = Field::nth_root(n);
        if (inv) r = Field::inv(r);
        rec(a, n, r);
    }
    template <typename It> void rec(It a, int n, F r) {
        if (n == 1) return;
        n >>= 1;
        rec(a, n, r * r);
        rec(a + n, n, r * r);
        F w = static_cast<F>(1);
        for (int i = 0; i < n; i++, w = w * r) {
            F tmp = w * a[n + i];
            a[n + i] = a[i] - tmp;
            a[i] = a[i] + tmp;
        }
    }
};

constexpr int P = 7 * 17 * (1 << 23) + 1;
constexpr int g = 3;

struct mod {
    int x;
    mod() : x(0) {}
    mod(int a) : x(a) {}
    mod operator+(const mod& b) const {
        int sum = x + b.x;
        return sum < P ? sum : sum - P;
    }
    mod operator-(const mod& b) const {
        int sub = x - b.x;
        return sub < 0 ? P + sub : sub;
    }
    mod operator*(const mod& b) const {
        return (ll(x) * ll(b.x)) % P;
    }
};

inline mod fexp(mod x, size_t e) {
    mod t(1);
    for (; e; e >>= 1, x = x * x)
        if (e & 1) t = t * x;
    return t;
}

struct M {
    using F = mod;
    static F nth_root(size_t n) { return fexp(g, (P - 1) / n); }
    static F inv(F x) { return fexp(x, P - 2); }
};

static double tau = 2.0 * acos(-1.0);
struct C {
    using F = complex<double>;
    static F nth_root(size_t n) {
        return polar(static_cast<double>(1.0),
                     tau / static_cast<double>(n));
    }
    static F inv(F x) { return static_cast<double>(1.0) / x; }
};

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data,
                                      size_t size) {
    const size_t bits = 8;
    size_t K = (1 << bits);
    M::F a[K], b[K], c[K], ans[K];

    for (size_t i = 0; i < K; i++) a[i] = 0;

    int x = 0;
    int cnt = 0;
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < 8; j++) {
            if (cnt + 1 == bits) {
                a[x].x++;
                x = 0;
                cnt = 0;
            }
            x <<= 1;
            x |= ((data[i] >> j) & 1);
            cnt++;
        }
    }
    if (cnt) a[x].x++;
    for (size_t i = 0; i < K; i++) b[i] = a[i];

    for (size_t i = 0; i < K; i++) ans[i] = 0;
    for (size_t i = 0; i < K; i++)
        for (size_t j = 0; j < K; j++)
            ans[(i + j) % K] = ans[(i + j) % K] + a[i] * b[j];

    dft<M>(a, K);
    dft<M>(b, K);
    for (size_t i = 0; i < K; i++) c[i] = a[i] * b[i];
    dft<M>(c, K, true);
    for (size_t i = 0; i < K; i++) c[i] = M::inv(K) * c[i];

    for (size_t i = 0; i < K; i++) assert(ans[i].x == c[i].x);
    return 0;
}
