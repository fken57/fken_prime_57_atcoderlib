# Template / utility

## 標準ヘッダ

新しい標準エントリは `template.hpp` です。旧来の `templete.hpp` は
`template.hpp` を include する互換 shim として残しています。

`template.hpp` が標準で提供するものは次のとおりです。

- `<bits/stdc++.h>` と AtCoder Library
- `debug.hpp`（`LOCAL` 未定義時は no-op）
- 基本型、グラフ型、modint エイリアス
- `chmin`, `chmax`, `sz`
- `rep`, `all`, `rall`
- `arr_max`, `arr_min`, `arr_sum`
- `infint`, `inf`、代表的な mod、8 方向配列、2/10 の冪表
- 高速 IO 設定済みの `solve/main`

## 入出力の分離

`input.hpp` と `print.hpp` は `template.hpp` から完全に分離されています。
必要な問題でだけ直接 inject してください。通常の `cin` / `cout` 利用時には不要です。

```cpp
//@inject input.hpp
//@inject print.hpp
```

## 型

従来の `ll`, `vll`, `vvll`, `vvvll`, `ull` 系、`Graph`, `vch`, `vs`,
`Pll`, `vPll`, `tup`, `bl` 系、`ld` 系、`mint`, `vmint` 系は維持しています。

modint は次の名前も利用できます。

```cpp
mint998                   // mod 998244353
mint107                   // mod 1000000007
mint                      // mint998 の互換 alias
static_mint<1000000007>   // compile-time modulus
dynamic_mint<0>           // runtime modulus; ID ごとに modulus を持つ
```

重み付きグラフは `WeightedEdge<Cost>` / `WeightedGraphT<Cost>` を基盤とし、
従来名 `WeightedGraphEdge` / `WeightedGraph` は `ll` コスト版を指します。辺には
`to`, `weight`, `id` があります。`id` は省略すると `-1` です。

## 小 utility

```cpp
chmin(current, candidate); // 更新した場合 true
chmax(current, candidate);
sz(container);             // int へ変換済みの size

arr_max(container);
arr_min(container);
arr_sum(container);
```

`arr_*` は関数を薄いマクロで呼ぶだけで、引数を 1 回しか評価しません。
`arr_max` / `arr_min` は空 range を許しません。`arr_sum` の戻り値型は要素型です。

## マクロ

標準で維持するマクロは次だけです。

```cpp
rep(i, begin, end)
all(container)
rall(container)
arr_max(container)
arr_min(container)
arr_sum(container)
```

`chmin` / `chmax` / `sz` は補完・型検査を優先して関数にしています。

## 定数と方向配列

```cpp
infint                 // 従来値 (1 << 30) - 1
inf                    // 1LL << 60
mod1000000007
mod998244353
dx[0..7], dy[0..7]
pow2ll[0..63]
pow10ll[0..19]
```

`pow2ll`, `pow10ll` の要素型は `ull` です。`2^63` と `10^19` も保持できます。
意図しない sentinel の混用を避けるため `INF` / `LINF` は定義しません。

## Debug

ローカル実行時だけ `-DLOCAL` を付けます。`debug.hpp` の実装は `LOCAL` 時だけ有効で、
提出ビルドでは no-op です。`_GLIBCXX_DEBUG` は計算量や ABI に影響するため、テンプレートから
自動定義しません。
