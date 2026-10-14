# `_fib`

C++17 向けの競技プログラミング用ライブラリです。各ヘッダは Bundler の
`//@inject path/to/header.hpp` で依存関係ごと提出コードへ展開することを前提にしています。

## 基本構成

- `common.hpp`: 標準 include、基本型、定数、短い utility（`main` なし）
- `template.hpp`: `common.hpp` と提出用 `solve/main`
- `templete.hpp`: 旧スペルを維持する互換ヘッダ。新規コードでは `template.hpp` を使用
- `input.hpp`, `print.hpp`: 必要な場合だけ個別に利用する入出力ライブラリ
- `Graph/`, `Tree/`, `DataStructure/`, `segtree/`, `String/`, `math/`: 分野別ライブラリ
- `*.test.cpp`: VS Code の Library Checker 拡張が認識する検証ドライバ
- `tests/`: 複数ライブラリをまとめたローカルテスト

各ライブラリヘッダは提出用 `main` を取り込みません。自分の `main` から直接 include できます。
`template.hpp` は `input.hpp` / `print.hpp` を include しません。両者は標準テンプレートから
独立しています。

## 主なライブラリ

| 分野 | ヘッダ | 主な機能 |
|---|---|---|
| Segment Tree | `segtree/Monoids.hpp` | 14種類のモノイド（`S / op / e`） |
| Segment Tree | `segtree/BracketSequence.hpp` | 括弧文字列の操作（任意利用） |
| Segment Tree | `segtree/BinarySearch.hpp` | `max_right` / `min_left` の補助、prefix lower_bound、k-th |
| Graph | `Graph/Dijkstra.hpp` | 単一始点・多始点最短路、頂点列/辺 ID の復元 |
| Graph | `Graph/CycleDetection.hpp` | 有向・無向サイクルの検出と頂点列/辺 ID の構築 |
| Graph | `Graph/LowLink.hpp` | 関節点、橋、二重辺連結成分、橋木 |
| Tree | `Tree/HeavyLightDecomposition.hpp` | LCA、部分木区間、順序付きパス分解 |
| Tree | `Tree/Rerooting.hpp` | 汎用全方位木 DP |
| Data structure | `DataStructure/PotentialDSU.hpp` | 加法ポテンシャル付き Union-Find |
| Data structure | `DataStructure/OrderStatisticTree.hpp` | index 操作可能な set/map（GNU PBDS、任意利用） |
| Data structure | `DataStructure/LiChaoTree.hpp` | 直線・区間直線追加、整数座標上の最小値取得 |
| String | `String/RollingHash.hpp` | mod `2^61-1` の部分文字列 hash、連結、LCP |
| Math | `math/Combination.hpp` | 選択した modint に対する動的前計算の二項係数 |
| Math | `math/ArbitraryModCombination.hpp` | 複合数を含む任意 modulus の二項係数 |
| Math | `math/Sieve.hpp` | 線形篩、最小素因数、Mobius、Euler phi、約数列挙 |
| Math | `math/PrimeFactorization64.hpp` | 64-bit Miller–Rabin / Pollard–Rho |

既存の BFS、木の直径、doubling、Kruskal、DSU も、経路・採用辺などを返せる API に
拡張されています。全体の一覧と API は [docs/library-index.md](docs/library-index.md) を参照してください。

## グラフの型

`Graph` は `vector<vector<int>>` です。重み付きは
`WeightedGraphT<Cost> = vector<vector<WeightedEdge<Cost>>>` とし、
辺は `{to, weight, id}`（`id` は省略可能）で追加します。`WeightedGraph` は `Cost = ll` の別名です。
BFS / LCA / HLD は通常の `vector<vector<int>>` を直接受け取ります。
旧 `vector<vector<ll>>` のグラフは頂点型を `int` に変更してください。

## `template.hpp` の要点

```cpp
vector<ll> a{3, 1, 4};
cout << arr_max(a) << ' ' << arr_min(a) << ' ' << arr_sum(a) << '\n';

chmin(a[0], 2LL);
chmax(a[1], 5LL);

mint998 x = 1;
mint107 y = 1;
dynamic_mint<0>::set_mod(1000000007);
```

従来の型エイリアス、`rep`、`all`、`rall`、`infint`、`inf`、8 方向配列、
`pow2ll`、`pow10ll` は維持しています。事故を避けるため、別名の `INF` / `LINF` は
定義していません。詳細は [docs/template.md](docs/template.md) を参照してください。

## Segment Tree の例

```cpp
//@inject segtree/BinarySearch.hpp

using M = RangeSumMonoid<long long>;
atcoder::segtree<M::S, M::op, M::e> seg(vector<long long>{2, 0, 3, 1});
cout << seg.prod(0, 3) << '\n';
cout << segtree_prefix_lower_bound(seg, 4) << '\n'; // 2

//@inject segtree/BracketSequence.hpp
BracketSequence brackets("(()())");
cout << brackets.is_valid() << '\n';
```

Monoid 一覧、実際の使用例、グローバル変数と判定関数による二分探索は
[docs/segtree.md](docs/segtree.md) にまとめています。
[segtree/README.md](segtree/README.md) はヘッダを選ぶための入口です。

## 検証

- 全ヘッダ同時コンパイル: `tests/all_headers_compile.cpp`
- ランダム・境界値テスト: `tests/library_unit_test.cpp`
- Generic API / Monoid 検証: `tests/generic_api_test.cpp`
- Library Checker 保存済み結果: 新規 11 ドライバ 274 ケースと既存回帰 111 ケース、計 385 ケースで AC（今回の API 変更後は未再実行）

検証対象と拡張機能からの実行方法は [docs/testing.md](docs/testing.md) を参照してください。

## ドキュメント

- [Template / utility](docs/template.md)
- [Segment Tree](docs/segtree.md)
- [Segment Tree ヘッダ案内](segtree/README.md)
- [Graph / Tree](docs/graph-tree.md)
- [Data structure / Math / String](docs/data-structure-math.md)
- [ライブラリ一覧](docs/library-index.md)
- [テスト方法](docs/testing.md)
