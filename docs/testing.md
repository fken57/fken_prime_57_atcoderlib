# Testing

## ローカルテスト

リポジトリ root で実行します。

```sh
g++ -std=gnu++17 -O2 -Wall -Wextra -Wshadow -I. \
  tests/all_headers_compile.cpp -o /tmp/fib_all_headers

g++ -std=gnu++17 -O2 -Wall -Wextra -Wshadow -I. \
  tests/library_unit_test.cpp -o /tmp/fib_library_unit
/tmp/fib_library_unit
```

`library_unit_test.cpp` は固定 seed のランダムテストで、HLD の順序、方向別 rerooting、
LowLink、cycle 構築、Dijkstra、Li Chao Tree、Rolling Hash、Potential DSU、64-bit
factorization などを素朴解と比較します。

## Library Checker 拡張

`*_category__problem.test.cpp` は先頭に次の metadata を持ちます。

```cpp
// library-checker-judge test case
// problem: graph/cycle_detection
// library: Graph/CycleDetection.hpp
```

VS Code の C++ Dependency Injector / Library Checker 画面からテストを選択すると、
Bundler が対象ヘッダと依存先を展開し、公式 generator と checker で検証できます。
再検証時はヘッダ変更を反映するため依存を再展開してください。

## 現在の公式検証

| Library Checker problem | Driver | Result |
|---|---|---:|
| `graph/cycle_detection` | `Graph/CycleDetection...test.cpp` | AC 35/35 |
| `graph/cycle_detection_undirected` | `Graph/CycleDetection...test.cpp` | AC 27/27 |
| `graph/two_edge_connected_components` | `Graph/LowLink...test.cpp` | AC 21/21 |
| `graph/shortest_path` | `Graph/Dijkstra...test.cpp` | AC 29/29 |
| `tree/vertex_add_path_sum` | `Tree/HeavyLightDecomposition...test.cpp` | AC 19/19 |
| `data_structure/line_add_get_min` | `DataStructure/LiChaoTree...test.cpp` | AC 15/15 |
| `data_structure/unionfind_with_potential` | `DataStructure/PotentialDSU...test.cpp` | AC 18/18 |
| `data_structure/ordered_set` | `DataStructure/OrderStatisticTree...test.cpp` | AC 37/37 |
| `number_theory/primality_test` | `math/PrimeFactorization64...test.cpp` | AC 12/12 |
| `number_theory/factorize` | `math/PrimeFactorization64...test.cpp` | AC 31/31 |
| `enumerative_combinatorics/binomial_coefficient` | `math/ArbitraryModCombination...test.cpp` | AC 30/30 |

新規ドライバは合計 274 ケースです。さらに更新した既存の DSU、Kruskal、木の直径、
Tree Doubling（jump/LCA）も 111 ケースで再検証し、合計 385 ケースで AC しています。

Rerooting、Rolling Hash、Combination、Segment Tree モノイドは
直接対応する問題だけでは API 全体を覆えないため、ローカルの素朴解比較でも補完しています。
