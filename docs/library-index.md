# Library index

| Header | Responsibility | Notes |
|---|---|---|
| `template.hpp` | 基本型、定数、utility、標準 main | `input.hpp` / `print.hpp` 非依存 |
| `debug.hpp` | LOCAL debug 表示 | 非 LOCAL は no-op |
| `input.hpp` | generic input | 任意利用 |
| `print.hpp` | generic output | 任意利用 |
| `Graph/BFS.hpp` | 単一/多始点 BFS、経路復元 | 旧 `BFS` 互換 |
| `Graph/Dijkstra.hpp` | 非負重み最短路、経路復元 | generic cost |
| `Graph/CycleDetection.hpp` | 有向/無向 cycle 構築 | 頂点・辺 ID を返す |
| `Graph/LowLink.hpp` | 関節点、橋、二重辺連結成分 | iterative |
| `Tree/TreeDoubling.hpp` | LCA、祖先、距離、jump | dynamic table size |
| `Tree/HeavyLightDecomposition.hpp` | パス/部分木の区間化 | 順序・辺 mode 対応 |
| `Tree/Rerooting.hpp` | 全方位木 DP | generic DP/edge data |
| `Tree/TreeDiameter.hpp` | 重み付き/非重み付き木の直径 | path 復元 |
| `Tree/MST.hpp` | Kruskal | 辺 ID 復元、旧 API 互換 |
| `Tree/DisjointUnionSet.hpp` | Union-Find | ACL 基盤、旧 API 互換 |
| `Tree/WeightedTreeBFS.hpp` | 重み付き木の距離 | legacy helper |
| `DataStructure/PotentialDSU.hpp` | 加法ポテンシャル DSU | `ll`, modint など |
| `DataStructure/OrderStatisticTree.hpp` | indexed set/map | GNU PBDS、任意利用 |
| `DataStructure/LiChaoTree.hpp` | 直線/区間直線の最小値 | dynamic integer domain |
| `segtree/Monoids.hpp` | 頻用モノイド | ACL segtree を再利用 |
| `segtree/BinarySearch.hpp` | segtree 上の単調探索 | range sum 専用 helper あり |
| `String/RollingHash.hpp` | substring hash / LCP | mod `2^61-1` |
| `math/Combination.hpp` | `C`, `P`, `H` | 選択可能な prime modint |
| `math/ArbitraryModCombination.hpp` | 任意 mod の `C` | 複合数・巨大な `n` に対応 |
| `math/Sieve.hpp` | 線形篩と乗法的関数 | 旧綴り互換あり |
| `math/PrimeFactorization64.hpp` | 64-bit 素数判定/素因数分解 | Miller–Rabin/Pollard–Rho |
| `math/ModArithmetic.hpp` | 正規化剰余、mod power | CRT の共通依存 |
| `math/Equation/ChineseRem.hpp` | 中国剰余 | `ModArithmetic`, `extGCD` を再利用 |
| `math/Equation/Equation.hpp` | 一次不定方程式 | legacy API |
| `math/matrixMult.hpp` | 行列積 | legacy |
| `math/matrixPow.hpp` | 抽象演算の行列累乗 | legacy |
| `math/emerate_quo.hpp` | 商が同じ区間の列挙 | legacy name |
| `technique/Compress.hpp` | 座標圧縮結果 | legacy |
| `technique/BaseConverter.hpp` | 2～16 進変換 | legacy |
| `technique/IntengerPatitions.hpp` | 整数分割列挙 | legacy name |

`Tree/AVLTree/AVLSet.hpp` は平衡化されておらず API も旧実装のままなので、新規コードでは
使用せず `std::set` または `DataStructure/OrderStatisticTree.hpp` を利用してください。
