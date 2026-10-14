# Segment Tree

本体は ACL の `atcoder::segtree<S, op, e>` をそのまま使います。
`Monoids.hpp` はよく使う `S`, `op`, `e` の定義集です。
各例の `vector` は `std::vector` です。提出テンプレートの `using namespace std;` を前提にしています。
通常の include では `std::vector` と書いてください。

## 基本の使い方

```cpp
//@inject segtree/Monoids.hpp

using M = RangeSumMonoid<long long>;
atcoder::segtree<M::S, M::op, M::e> seg(vector<long long>{2, 0, 3, 1});

seg.prod(0, 3);                 // 5: 半開区間 [0, 3)
seg.set(1, 4);                  // 点代入
seg.set(1, seg.get(1) + 2);     // 点加算
seg.all_prod();                 // 全区間の和
```

通常の include でも使えます。`Monoids.hpp` は提出用 `main` や型エイリアスに依存しません。

```cpp
#include "_fib/segtree/Monoids.hpp"
#include <vector>

int main() {
    using M = RangeSumMonoid<long long>;
    atcoder::segtree<M::S, M::op, M::e> seg(std::vector<long long>{2, 0, 3, 1});
    return seg.prod(0, 3) == 5 ? 0 : 1;
}
```

## モノイド一覧

すべて `S`, `op`, `e` を持ちます。`value_type` は `S` の互換名です。

| Monoid | `S` | 演算 | 単位元 |
|---|---|---|---|
| `RangeMinMonoid<T>` | `T` | min | `numeric_limits<T>::max()` |
| `RangeMaxMonoid<T>` | `T` | max | `numeric_limits<T>::lowest()` |
| `RangeSumMonoid<T>` | `T` | 加算 | `0` |
| `RangeProductMonoid<T>` | `T` | 乗算 | `1` |
| `RangeGcdMonoid<T>` | `T` | gcd | `0` |
| `RangeLcmMonoid<T>` | `T` | lcm | `1` |
| `RangeXorMonoid<T>` | `T` | bitwise xor | `0` |
| `RangeAndMonoid<T>` | `T` | bitwise and | 全 bit が1 |
| `RangeOrMonoid<T>` | `T` | bitwise or | `0` |
| `RangeMinMaxMonoid<T>` | `pair<T,T>` | 最小値と最大値 | `{max, lowest}` |
| `RangeMinCountMonoid<T>` | `pair<T,int>` | 最小値とその個数 | `{max, 0}` |
| `RangeMaxCountMonoid<T>` | `pair<T,int>` | 最大値とその個数 | `{lowest, 0}` |
| `AffineCompositionMonoid<T>` | `pair<T,T>` | 一次関数の合成 | `{1, 0}` |
| `BracketBalanceMonoid` | `BracketBalance` | 合計と最小 prefix 和 | `{0, 0}` |

- gcd/lcm は非負整数、bitwise 演算は整数向けです。
- 和・積・lcm・関数合成の中間値が型の範囲内に収まるようにしてください。
- min/max 系の `T` は `numeric_limits` が使える型向けです。
- 空区間の `prod(l, l)` は `e()` を返します。

## 数値・ビット演算の使い方

型を選び、`M::S / M::op / M::e` を渡します。以下の各ブロックは別々に使う例です。

```cpp
vector<long long> a{6, 3, 12};

{
    using M = RangeMinMonoid<long long>;
    atcoder::segtree<M::S, M::op, M::e> seg(a);
    auto minimum = seg.prod(0, 3); // 3
}
{
    using M = RangeMaxMonoid<long long>;
    atcoder::segtree<M::S, M::op, M::e> seg(a);
    auto maximum = seg.prod(0, 3); // 12
}
{
    using M = RangeSumMonoid<long long>;
    atcoder::segtree<M::S, M::op, M::e> seg(a);
    auto sum = seg.prod(0, 3); // 21
}
{
    using M = RangeProductMonoid<long long>;
    atcoder::segtree<M::S, M::op, M::e> seg(a);
    auto product = seg.prod(0, 3); // 216
}
{
    using M = RangeGcdMonoid<long long>;
    atcoder::segtree<M::S, M::op, M::e> seg(a);
    auto gcd = seg.prod(0, 3); // 3
}
{
    using M = RangeLcmMonoid<long long>;
    atcoder::segtree<M::S, M::op, M::e> seg(a);
    auto lcm = seg.prod(0, 3); // 12
}
{
    using M = RangeXorMonoid<long long>;
    atcoder::segtree<M::S, M::op, M::e> seg(a);
    auto bits = seg.prod(0, 3); // 9
}
{
    using M = RangeAndMonoid<long long>;
    atcoder::segtree<M::S, M::op, M::e> seg(a);
    auto bits = seg.prod(0, 3); // 0
}
{
    using M = RangeOrMonoid<long long>;
    atcoder::segtree<M::S, M::op, M::e> seg(a);
    auto bits = seg.prod(0, 3); // 15
}
```

どの型でも `seg.set(i, value)` は点代入、`seg.get(i)` は点取得です。
和の点加算は `seg.set(i, seg.get(i) + delta)` と書きます。

## 最小値と最大値

```cpp
using M = RangeMinMaxMonoid<long long>;
vector<M::S> values;
for (long long x : vector<long long>{6, 3, 12}) {
    values.push_back(M::from_value(x)); // {x, x}
}
atcoder::segtree<M::S, M::op, M::e> seg(values);
auto [minimum, maximum] = seg.prod(0, 3); // {3, 12}
seg.set(1, M::from_value(10));
```

## 最小値と個数

```cpp
using M = RangeMinCountMonoid<long long>;
vector<M::S> values;
for (long long x : vector<long long>{3, 1, 1, 4}) {
    values.push_back(M::from_value(x)); // {x, 1}
}
atcoder::segtree<M::S, M::op, M::e> seg(values);
auto [minimum, count] = seg.prod(0, 4); // {1, 2}
seg.set(0, M::from_value(1));
```

## 最大値と個数

```cpp
using M = RangeMaxCountMonoid<long long>;
vector<M::S> values;
for (long long x : vector<long long>{3, 1, 3, 2}) {
    values.push_back(M::from_value(x)); // {x, 1}
}
atcoder::segtree<M::S, M::op, M::e> seg(values);
auto [maximum, count] = seg.prod(0, 4); // {3, 2}
seg.set(1, M::from_value(3));
```

## 一次関数の合成

`{a, b}` が `a*x+b` を表し、`op(left, right)` は左の関数、右の関数の順に適用します。
`prod(l, r)` は添字の昇順に関数を適用した結果です。

```cpp
#include <atcoder/modint>
using M = AffineCompositionMonoid<atcoder::modint998244353>;
atcoder::segtree<M::S, M::op, M::e> seg(vector<M::S>{{2, 3}, {5, 7}});
auto answer = M::apply(seg.prod(0, 2), 4); // 5*(2*4+3)+7 = 62
```

## 木上二分探索：グローバル変数と判定関数

探索条件をグローバル変数に置き、通常の関数を `max_right<判定関数>` に渡します。
判定関数の引数型は Monoid の `S`、戻り値は `bool` です。
以下は通常の include でそのままコンパイルできる例です。

```cpp
#include "_fib/segtree/Monoids.hpp"
#include <cassert>
#include <vector>

using M = RangeSumMonoid<long long>;
long long limit;

bool within_limit(M::S sum) {
    return sum <= limit;
}

int main() {
    std::vector<long long> values{2, 1, 3, 1};
    int n = static_cast<int>(values.size());
    atcoder::segtree<M::S, M::op, M::e> seg(values);

    limit = 4;
    int r = seg.max_right<within_limit>(0);
    assert(r == 2); // [0, 2) の和は3、[0, 3) の和は6

    int l = seg.min_left<within_limit>(n);
    assert(l == 2); // [2, 4) の和は4、[1, 4) の和は5

    limit = 100; // 次の query の条件を変更
    assert(seg.max_right<within_limit>(0) == n);
}
```

- `max_right<f>(l)` は `[l, r)` が条件を満たす最大の右端 `r` を返します。最後まで OK なら `n`。
- `min_left<f>(r)` は `[l, r)` が条件を満たす最小の左端 `l` を返します。先頭まで OK なら `0`。
- 区間は半開区間で、右端の要素を含みません。どちらも `O(log n)` です。
- 木は長さを公開しないため、元の長さ `n` を利用側で保持します。

判定は `f(M::e()) == true` を満たし、区間を伸ばすと true から false へ単調に変わる必要が
あります。この区間和の例では `limit >= 0` と全要素が非負であることが必要です。
一回の探索中はグローバル変数の値を変更しません。

最大値の木で「最初に threshold 以上になる要素」を探す場合も同じ書き方です。
次のコードでは判定関数はグローバルに定義します。

```cpp
using M = RangeMaxMonoid<long long>;
long long threshold;

bool below_threshold(M::S maximum) {
    return maximum < threshold;
}

void example() {
    vector<long long> values{2, 1, 7, 3};
    int n = static_cast<int>(values.size());
    atcoder::segtree<M::S, M::op, M::e> seg(values);
    threshold = 5;
    int index = seg.max_right<below_threshold>(0); // 2: values[2] == 7
    if (index == n) {
        // threshold 以上の要素なし
    }
}
```

この例では空区間の判定のため `threshold > M::e()` が必要です。

非負の和・頻度配列には `BinarySearch.hpp` の補助関数も使えます。

```cpp
//@inject segtree/BinarySearch.hpp
using M = RangeSumMonoid<long long>;
atcoder::segtree<M::S, M::op, M::e> seg(vector<long long>{2, 0, 3, 1});
int index = segtree_prefix_lower_bound(seg, 4); // 2: prod(0, 3) >= 4
int kth = segtree_kth(seg, 0);                 // 0: 0-indexed k-th
```

見つからなければ元の配列長、`target <= 0` なら lower_bound は `0` を返します。
値は非負、k は非負整数である必要があります。閾値や k は要素型に変換可能で、
その型の範囲内（k は k+1 も範囲内）である必要があります。
`segtree_first_break` / `segtree_left_boundary` は互換用で、ACL の探索をそのまま呼びます。

## 括弧列

数値情報だけなら `BracketBalanceMonoid` を使います。

```cpp
using M = BracketBalanceMonoid;
vector<M::S> values;
for (int x : vector<int>{1, 1, -1, -1}) values.push_back(M::from_number(x));
atcoder::segtree<M::S, M::op, M::e> seg(values);
bool valid = M::is_valid(seg.all_prod());
```

文字列の操作も必要なら、専用ヘッダを追加します。

```cpp
//@inject segtree/BracketSequence.hpp
BracketSequence brackets("(()())");
brackets.toggle(1);
brackets.is_valid();
brackets.is_valid(1, 3); // 部分文字列 [1, 3)
```

`Monoids.hpp` / `BinarySearch.hpp` は文字列 wrapper を include しません。
`first_invalid(left)` は balance が初めて負になる文字の index、見つからなければ文字列長を
返します。合計が0かどうかは `is_valid` で確認してください。

## 独自モノイドと互換名

独自定義も `struct M { using S = ...; static S op(S, S); static S e(); };` だけです。
演算には結合則と左右の単位元が必要で、可換則は必要ありません。

既存の `MonoidSegTree<M>`、`RangeMinSegTree<T>`、`RangeMaxSegTree<T>`、
`RangeSumSegTree<T>`、`RangeGcdSegTree<T>`、括弧系 alias は互換用に残しています。
旧 `segtree_operation/segtree_op.hpp` は Monoid・探索・括弧文字列の全ヘッダを読み込みます。
区間更新には別途 ACL の `lazy_segtree` を使ってください。
