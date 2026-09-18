#pragma once

#include <bits/stdc++.h>
#include <atcoder/all>

#include "debug.hpp"

using namespace std;
using namespace atcoder;

/* frequently used types */
using ll = long long;
using vll = vector<ll>;
using vvll = vector<vll>;
using vvvll = vector<vvll>;

using ull = unsigned long long;
using vull = vector<ull>;
using vvull = vector<vull>;
using vvvull = vector<vvull>;

using Graph = vvll;

using vch = vector<char>;
using vvch = vector<vector<char>>;
using vs = vector<string>;

using Pll = pair<ll, ll>;
using vPll = vector<Pll>;
using pll = pair<ll, ll>;
using pii = pair<int, int>;

using tup = tuple<ll, ll, ll>;

using bl = bool;
using vbl = vector<bool>;
using vvbl = vector<vbl>;
using vvvbl = vector<vvbl>;

using ld = long double;
using vld = vector<ld>;
using vvld = vector<vld>;
using vvvld = vector<vvld>;

/* modint aliases: mint remains compatible with the old template. */
using mint998 = atcoder::modint998244353;
using mint107 = atcoder::modint1000000007;
using mint = mint998;
using vmint = vector<mint>;
using vvmint = vector<vmint>;
using vvvmint = vector<vvmint>;
template <int Mod>
using static_mint = atcoder::static_modint<Mod>;
template <int Id = 0>
using dynamic_mint = atcoder::dynamic_modint<Id>;

constexpr int infint = (1 << 30) - 1;
constexpr ll inf = 1LL << 60;

template <class Cost>
struct WeightedEdge {
    int to;
    Cost weight;
    int id = -1;
};

template <class Cost>
using WeightedGraphT = vector<vector<WeightedEdge<Cost>>>;

using WeightedGraphEdge = WeightedEdge<ll>;
using WeightedGraph = WeightedGraphT<ll>;

template <class T>
inline bool chmax(T& a, const T& b) {
    if (a < b) {
        a = b;
        return true;
    }
    return false;
}

template <class T>
inline bool chmin(T& a, const T& b) {
    if (b < a) {
        a = b;
        return true;
    }
    return false;
}

#define rep(i, x, lim) for (ll i = (x); i < static_cast<ll>(lim); ++i)
#define all(x) std::begin(x), std::end(x)
#define rall(x) std::rbegin(x), std::rend(x)

template <class Container>
constexpr int sz(const Container& container) {
    return static_cast<int>(container.size());
}

namespace fib {
namespace utility {

template <class Range>
auto array_max(const Range& range) {
    using Value = std::decay_t<decltype(*std::begin(range))>;
    const auto first = std::begin(range);
    const auto last = std::end(range);
    assert(first != last && "arr_max requires a non-empty range");
    return Value(*std::max_element(first, last));
}

template <class Range>
auto array_min(const Range& range) {
    using Value = std::decay_t<decltype(*std::begin(range))>;
    const auto first = std::begin(range);
    const auto last = std::end(range);
    assert(first != last && "arr_min requires a non-empty range");
    return Value(*std::min_element(first, last));
}

template <class Range>
auto array_sum(const Range& range) {
    using Value = std::decay_t<decltype(*std::begin(range))>;
    Value result{};
    for (const auto& value : range) result += value;
    return result;
}

}  // namespace utility
}  // namespace fib

// Function-backed macros evaluate the range expression exactly once.
#define arr_max(range) (::fib::utility::array_max((range)))
#define arr_min(range) (::fib::utility::array_min((range)))
#define arr_sum(range) (::fib::utility::array_sum((range)))

constexpr ll mod1000000007 = 1000000007LL;
constexpr ll mod998244353 = 998244353LL;

inline constexpr array<ll, 8> dx{1, -1, 0, 0, 1, 1, -1, -1};
inline constexpr array<ll, 8> dy{0, 0, 1, -1, 1, -1, 1, -1};

inline constexpr array<ull, 64> pow2ll{
    1ULL, 2ULL, 4ULL, 8ULL, 16ULL, 32ULL, 64ULL, 128ULL,
    256ULL, 512ULL, 1024ULL, 2048ULL, 4096ULL, 8192ULL,
    16384ULL, 32768ULL, 65536ULL, 131072ULL, 262144ULL,
    524288ULL, 1048576ULL, 2097152ULL, 4194304ULL, 8388608ULL,
    16777216ULL, 33554432ULL, 67108864ULL, 134217728ULL,
    268435456ULL, 536870912ULL, 1073741824ULL, 2147483648ULL,
    4294967296ULL, 8589934592ULL, 17179869184ULL, 34359738368ULL,
    68719476736ULL, 137438953472ULL, 274877906944ULL,
    549755813888ULL, 1099511627776ULL, 2199023255552ULL,
    4398046511104ULL, 8796093022208ULL, 17592186044416ULL,
    35184372088832ULL, 70368744177664ULL, 140737488355328ULL,
    281474976710656ULL, 562949953421312ULL, 1125899906842624ULL,
    2251799813685248ULL, 4503599627370496ULL, 9007199254740992ULL,
    18014398509481984ULL, 36028797018963968ULL, 72057594037927936ULL,
    144115188075855872ULL, 288230376151711744ULL,
    576460752303423488ULL, 1152921504606846976ULL,
    2305843009213693952ULL, 4611686018427387904ULL,
    9223372036854775808ULL,
};

inline constexpr array<ull, 20> pow10ll{
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL, 1000000ULL,
    10000000ULL, 100000000ULL, 1000000000ULL, 10000000000ULL,
    100000000000ULL, 1000000000000ULL, 10000000000000ULL,
    100000000000000ULL, 1000000000000000ULL, 10000000000000000ULL,
    100000000000000000ULL, 1000000000000000000ULL,
    10000000000000000000ULL,
};

#ifndef FIB_NO_MAIN
void solve() {}

int main() {
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);
    ll T = 1;
    // cin >> T;
    while (T--) {
        solve();
    }
}
#endif
