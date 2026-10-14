# Segment Tree

本体は ACL の `atcoder::segtree<S, op, e>` をそのまま使います。
`Monoids.hpp` は14種類の Monoid を提供し、各 Monoid が `S`, `op`, `e` を持ちます。

**[Monoid 一覧と実際の使い方](../docs/segtree.md)** に、各 Monoid の構築・区間取得・更新、
グローバル変数と判定関数を使う `max_right<f>` / `min_left<f>` の例をまとめています。

```cpp
//@inject segtree/Monoids.hpp
using M = RangeSumMonoid<long long>;
atcoder::segtree<M::S, M::op, M::e> seg(vector<long long>{2, 0, 3, 1});
seg.prod(0, 3); // 5
seg.set(1, 4);
```

- `Monoids.hpp`: min/max/sum/product/gcd/lcm、xor/and/or、min-max、最小値・最大値と個数、一次関数合成、括弧 balance
- `BinarySearch.hpp`: 非負和の lower_bound、頻度配列の k-th（任意利用）
- `BracketSequence.hpp`: 括弧文字列の操作（任意利用）

旧 Segment Tree alias と `segtree_operation/segtree_op.hpp` は互換用に残しています。
