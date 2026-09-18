# Graph / Tree

## 共通グラフ型

- 非重み付き: `Graph = vector<vector<ll>>`
- 重み付き: `WeightedGraphT<Cost>`、`WeightedEdge<Cost>{to, weight, id}`
- `WeightedGraph` は `Cost = ll`

## BFS / Dijkstra

`Graph/BFS.hpp` の `bfs(graph, source)` は `BFSResult{dist, parent}` を返します。
`restore_path(target)` で頂点列を復元できます。複数始点版は source の vector を渡します。
旧 `BFS(start, graph, dist)` も維持しています。

`Graph/Dijkstra.hpp`:

```cpp
auto result = dijkstra(graph, source);
result.dist[target];
result.reachable(target);
result.restore_vertices(target);
result.restore_edges(target);
```

多始点は `dijkstra(graph, vector<int>{...})`。辺重みは非負である必要があります。
`edge.id == -1` の場合、復元辺 ID はその頂点の隣接リスト内 index になります。

## Cycle Detection

`Graph/CycleDetection.hpp` は判定だけでなく、実際の cycle を構築します。

```cpp
GraphCycle directed = find_directed_cycle(n, edges);
GraphCycle undirected = find_undirected_cycle(n, edges);
```

`edges[i] = {u, v}` とし、有向版では `u -> v` です。結果の `vertices` と `edges` は同じ長さで、
`edges[i]` が `vertices[i]` から次の頂点へ接続します。見つからない場合は両方空です。
無向版は自己ループと多重辺にも対応します。再帰を使わないため長い path でも stack overflow しません。

## LowLink

```cpp
LowLink lowlink(n, edges);
lowlink.articulation_points();
lowlink.bridge_ids();

TwoEdgeConnectedComponents components(lowlink);
components[vertex];
components.groups();
components.bridge_tree();
```

辺 ID は入力 vector の index です。非連結・多重辺・自己ループを扱えます。

## Tree doubling

`Tree/TreeDoubling.hpp`:

```cpp
TreeDoubling tree(graph, root);
tree.kth_ancestor(v, k);
tree.lca(u, v);       // 旧名 LCA も可
tree.distance(u, v);
tree.jump(u, v, k);   // u から v へ k 辺進んだ頂点。旧名 JumpOnTree も可
```

## Heavy-Light Decomposition

`Tree/HeavyLightDecomposition.hpp` は頂点を半開区間へ写します。

```cpp
HeavyLightDecomposition hld(graph, root);
int p = hld.position(vertex);
auto [l, r] = hld.subtree(vertex);
int ancestor = hld.lca(u, v);
auto segments = hld.path_segments(u, v);
```

`HeavyLightSegment{left, right, reversed}` の順番は常に `u -> v` です。非可換モノイドでは
`reversed` に応じて区間内の向きを反転してください。可換な和などは `for_each_path` が便利です。
辺を「深い側の頂点位置」に置く場合は `edge_mode=true` を指定します。

## Rerooting

`Tree/Rerooting.hpp` の `rerooting_dp` は `merge`, `add_edge`, `add_vertex` を受け取ります。
辺は `RerootingEdge<EdgeData>` で、対称データと方向別データの両方を構築できます。

`add_edge(neighbor_dp, edge_data)` に渡る方向別データは「現在の頂点から隣接頂点へ」の値です。
`merge` は結合則を満たし、`identity` を単位元とする必要があります。計算量は `O(N)` です。

## MST / diameter / DSU

- `kruskal(n, edges)` は合計コスト、採用辺 ID、連結成分数を返します。
- 旧 `MinimumSpinningTree` は維持し、生成した森と `connected` も返します。
- `ExecuteTreeDiameter` は直径長・両端点・実際の頂点列を返します。
- `DisjointUnionSet` は ACL DSU を基盤に旧 API を維持し、`size` と連結成分数も提供します。
