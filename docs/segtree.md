# Segment Tree

実体は AtCoder Library の `atcoder::segtree` をそのまま使い、頻用モノイドと探索 helper
だけを追加しています。用途別の Segment Tree 本体は複製していません。

## モノイド

`segtree/Monoids.hpp`:

| 型 | 演算 | 単位元 |
|---|---|---|
| `RangeMinMonoid<T>` | `min` | `numeric_limits<T>::max()` |
| `RangeMaxMonoid<T>` | `max` | `numeric_limits<T>::lowest()` |
| `RangeSumMonoid<T>` | `+` | `T{}` |
| `RangeGcdMonoid<T>` | `gcd` | `T{}` |
| `BracketMonoid` | 括弧列情報の連結 | 空列 |

短縮名は `RangeMinSegTree<T>`, `RangeMaxSegTree<T>`, `RangeSumSegTree<T>`,
`RangeGcdSegTree<T>`, `BracketSegTree` です。独自モノイドには
`MonoidSegTree<MyMonoid>` を使えます。

## 括弧列

`BracketValue` は区間の合計と最小 prefix 和を持ちます。

```cpp
vector<BracketValue> values;
for (char c : s) values.push_back(BracketMonoid::from_char(c));
BracketSegTree seg(values);

bool valid = BracketMonoid::is_valid(seg.prod(left, right));
bool never_negative = BracketMonoid::is_prefix_valid(seg.prod(left, right));
```

点更新は `seg.set(index, BracketMonoid::from_char(new_char))` です。

## 木上二分探索

`segtree/BinarySearch.hpp` は ACL の探索条件を明示する補助 API です。

```cpp
int first_bad = segtree_first_break(seg, left, predicate);
int leftmost = segtree_left_boundary(seg, right, predicate);
```

どちらも `predicate(Monoid::e()) == true` が必要で、述語は区間を伸ばしたとき単調で
なければなりません。

非負値の range sum には次が使えます。

```cpp
int index = segtree_prefix_lower_bound(sum_seg, target);
int kth_index = segtree_kth(frequency_seg, zero_based_k);
```

見つからない場合は `seg.size()` を返します。負値を含む和には使えません。

旧 `segtree/segtree_operation/segtree_op.hpp` は両ヘッダを読む互換エントリです。
