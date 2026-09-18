#define FIB_NO_MAIN

#include "../DataStructure/LiChaoTree.hpp"
#include "../DataStructure/OrderStatisticTree.hpp"
#include "../DataStructure/PotentialDSU.hpp"
#include "../Graph/CycleDetection.hpp"
#include "../Graph/Dijkstra.hpp"
#include "../Graph/LowLink.hpp"
#include "../String/RollingHash.hpp"
#include "../Tree/HeavyLightDecomposition.hpp"
#include "../Tree/Rerooting.hpp"
#include "../math/Combination.hpp"
#include "../math/ArbitraryModCombination.hpp"
#include "../math/PrimeFactorization64.hpp"
#include "../math/Sieve.hpp"
#include "../segtree/BinarySearch.hpp"

namespace {

mt19937 random_engine(123456789);

int random_int(int left, int right) {
    return uniform_int_distribution<int>(left, right)(random_engine);
}

void test_template_and_segtree() {
    vector<ll> values{3, -2, 7, 4};
    assert(arr_max(values) == 7);
    assert(arr_min(values) == -2);
    assert(arr_sum(values) == 12);
    assert(arr_max((vector<int>{1, 9, 2})) == 9);
    assert(pow2ll[63] == (1ULL << 63));
    assert(pow10ll[19] == 10000000000000000000ULL);
    assert(dx.size() == 8 && dy.size() == 8);

    RangeMinSegTree<ll> minimum(values);
    RangeMaxSegTree<ll> maximum(values);
    RangeSumSegTree<ll> sum(values);
    assert(minimum.prod(1, 4) == -2);
    assert(maximum.prod(0, 3) == 7);
    assert(sum.prod(0, 4) == 12);

    RangeSumSegTree<int> frequency(vector<int>{0, 2, 0, 3});
    assert(segtree_prefix_lower_bound(frequency, 1) == 1);
    assert(segtree_prefix_lower_bound(frequency, 3) == 3);
    assert(segtree_prefix_lower_bound(frequency, 6) == 4);
    assert(segtree_kth(frequency, 0) == 1);
    assert(segtree_kth(frequency, 1) == 1);
    assert(segtree_kth(frequency, 2) == 3);
    assert(segtree_left_boundary(frequency, 4,
                                 [](int value) { return value <= 3; }) == 2);

    const string brackets = "(()())()";
    vector<BracketValue> bracket_values;
    for (const char bracket : brackets) {
        bracket_values.push_back(BracketMonoid::from_char(bracket));
    }
    BracketSegTree bracket_tree(bracket_values);
    assert(BracketMonoid::is_valid(bracket_tree.all_prod()));
    assert(BracketMonoid::is_valid(bracket_tree.prod(1, 3)));
    assert(!BracketMonoid::is_valid(bracket_tree.prod(0, 3)));

    vector<BracketValue> invalid_values;
    for (const char bracket : string("())")) {
        invalid_values.push_back(BracketMonoid::from_char(bracket));
    }
    BracketSegTree invalid_brackets(invalid_values);
    assert(segtree_first_break(
               invalid_brackets, 0,
               [](BracketValue value) {
                   return BracketMonoid::is_prefix_valid(value);
               }) == 2);
}

void test_combination_and_sieve() {
    using mint13 = static_mint<13>;
    Combination<mint13> combination;
    assert(combination.C(10, 3).val() == 3);
    assert(combination.P(5, 2).val() == 7);
    assert(combination.H(3, 4).val() == 2);

    dynamic_mint<11>::set_mod(17);
    Combination<dynamic_mint<11>> dynamic_combination;
    assert(dynamic_combination.C(8, 4).val() == 2);

    COMinit(10);
    assert(COM(5, 2).val() == 10);

    for (int modulus = 1; modulus <= 100; ++modulus) {
        const ArbitraryModCombination arbitrary(modulus);
        vector<vector<long long>> pascal(101, vector<long long>(101));
        pascal[0][0] = 1 % modulus;
        for (int n = 1; n <= 100; ++n) {
            pascal[n][0] = pascal[n][n] = 1 % modulus;
            for (int k = 1; k < n; ++k) {
                pascal[n][k] =
                    (pascal[n - 1][k - 1] + pascal[n - 1][k]) % modulus;
            }
        }
        for (int n = 0; n <= 100; ++n) {
            for (int k = 0; k <= n; ++k) {
                assert(arbitrary.C(n, k) == pascal[n][k]);
            }
        }
    }

    Eratosthenes sieve(1000);
    assert(sieve.isprime[997]);
    assert(!sieve.isprime[999]);
    assert((sieve.factorize(756) == vector<pair<ll, ll>>{{2, 2}, {3, 3}, {7, 1}}));
    assert((sieve.divisors(12) == vector<ll>{1, 2, 3, 4, 6, 12}));
    assert(sieve.phi[36] == 12 && sieve.mobius[30] == -1);
}

void test_order_statistics_and_potential_dsu() {
    OrderedSet<int> set;
    for (const int value : {8, 1, 5, 3}) set.insert(value);
    assert(*set.find_by_order(0) == 1);
    assert(*set.find_by_order(2) == 5);
    assert(set.order_of_key(5) == 2);

    OrderedMap<int, string> map;
    map[10] = "ten";
    map[3] = "three";
    assert(map.find_by_order(1)->second == "ten");

    PotentialDSU<ll> dsu(5);
    assert(dsu.merge(0, 1, 4));
    assert(dsu.merge(1, 2, -7));
    assert(dsu.diff(0, 2) == -3);
    assert(dsu.merge(0, 2, -3));
    assert(!dsu.merge(0, 2, 9));
    assert(!dsu.diff_if_same(0, 4));
}

void validate_directed_cycle(const vector<pair<int, int>>& edges,
                             const GraphCycle& cycle) {
    assert(cycle.vertices.size() == cycle.edges.size());
    assert(cycle);
    const int length = static_cast<int>(cycle.edges.size());
    vector<bool> used(edges.size(), false);
    for (int i = 0; i < length; ++i) {
        const int id = cycle.edges[i];
        assert(!used[id]);
        used[id] = true;
        assert(edges[id].first == cycle.vertices[i]);
        assert(edges[id].second == cycle.vertices[(i + 1) % length]);
    }
}

void validate_undirected_cycle(const vector<pair<int, int>>& edges,
                               const GraphCycle& cycle) {
    assert(cycle.vertices.size() == cycle.edges.size());
    assert(cycle);
    const int length = static_cast<int>(cycle.edges.size());
    vector<bool> used(edges.size(), false);
    for (int i = 0; i < length; ++i) {
        const int id = cycle.edges[i];
        assert(!used[id]);
        used[id] = true;
        const int left = cycle.vertices[i];
        const int right = cycle.vertices[(i + 1) % length];
        assert((edges[id] == pair<int, int>{left, right} ||
                edges[id] == pair<int, int>{right, left}));
    }
}

void test_cycle_detection() {
    vector<pair<int, int>> directed{{0, 1}, {1, 2}, {2, 3}, {3, 1}};
    validate_directed_cycle(directed, find_directed_cycle(4, directed));
    assert(!find_directed_cycle(4, {{0, 1}, {0, 2}, {2, 3}}));
    validate_directed_cycle({{0, 0}}, find_directed_cycle(1, {{0, 0}}));

    vector<pair<int, int>> undirected{{0, 1}, {1, 2}, {2, 0}};
    validate_undirected_cycle(undirected, find_undirected_cycle(3, undirected));
    vector<pair<int, int>> parallel{{0, 1}, {0, 1}};
    validate_undirected_cycle(parallel, find_undirected_cycle(2, parallel));
    validate_undirected_cycle({{0, 0}}, find_undirected_cycle(1, {{0, 0}}));
    assert(!find_undirected_cycle(4, {{0, 1}, {1, 2}, {1, 3}}));
}

vector<int> naive_path(int from,
                       int to,
                       const vector<int>& parent,
                       const vector<int>& depth) {
    vector<int> front, back;
    while (from != to) {
        if (depth[from] >= depth[to]) {
            front.push_back(from);
            from = parent[from];
        } else {
            back.push_back(to);
            to = parent[to];
        }
    }
    front.push_back(from);
    reverse(back.begin(), back.end());
    front.insert(front.end(), back.begin(), back.end());
    return front;
}

void test_hld_and_rerooting() {
    for (int trial = 0; trial < 200; ++trial) {
        const int n = random_int(1, 60);
        Graph graph(n);
        vector<int> parent(n, 0), depth(n, 0);
        vector<RerootingEdge<ll>> edges;
        vector<vector<pair<int, ll>>> weighted(n);
        for (int vertex = 1; vertex < n; ++vertex) {
            parent[vertex] = random_int(0, vertex - 1);
            depth[vertex] = depth[parent[vertex]] + 1;
            graph[vertex].push_back(parent[vertex]);
            graph[parent[vertex]].push_back(vertex);
            const ll forward = random_int(1, 20);
            const ll backward = random_int(1, 20);
            edges.emplace_back(parent[vertex], vertex, forward, backward);
            weighted[parent[vertex]].push_back({vertex, forward});
            weighted[vertex].push_back({parent[vertex], backward});
        }

        HeavyLightDecomposition hld(graph);
        for (int query = 0; query < 100; ++query) {
            const int from = random_int(0, n - 1);
            const int to = random_int(0, n - 1);
            const auto expected = naive_path(from, to, parent, depth);
            vector<int> actual;
            for (const auto& segment : hld.path_segments(from, to)) {
                if (segment.reversed) {
                    for (int position = segment.right - 1;
                         position >= segment.left; --position) {
                        actual.push_back(hld.vertex_at(position));
                    }
                } else {
                    for (int position = segment.left;
                         position < segment.right; ++position) {
                        actual.push_back(hld.vertex_at(position));
                    }
                }
            }
            assert(actual == expected);
            assert(hld.distance(from, to) ==
                   static_cast<int>(expected.size()) - 1);

            const int ancestor = hld.lca(from, to);
            vector<int> expected_edges = expected;
            expected_edges.erase(find(expected_edges.begin(),
                                      expected_edges.end(), ancestor));
            actual.clear();
            for (const auto& segment : hld.path_segments(from, to, true)) {
                if (segment.reversed) {
                    for (int position = segment.right - 1;
                         position >= segment.left; --position) {
                        actual.push_back(hld.vertex_at(position));
                    }
                } else {
                    for (int position = segment.left;
                         position < segment.right; ++position) {
                        actual.push_back(hld.vertex_at(position));
                    }
                }
            }
            assert(actual == expected_edges);
        }

        struct State {
            ll sum;
            int count;
        };
        const auto answers = rerooting_dp<State>(
            n, edges, {0, 0},
            [](State left, State right) {
                return State{left.sum + right.sum,
                             left.count + right.count};
            },
            [](State state, ll cost) {
                state.sum += cost * state.count;
                return state;
            },
            [](State state, int) {
                ++state.count;
                return state;
            });
        for (int root = 0; root < n; ++root) {
            ll expected_sum = 0;
            vector<ll> distance(n, -1);
            distance[root] = 0;
            vector<int> stack{root};
            while (!stack.empty()) {
                const int vertex = stack.back();
                stack.pop_back();
                for (const auto& [to, cost] : weighted[vertex]) {
                    if (distance[to] != -1) continue;
                    distance[to] = distance[vertex] + cost;
                    stack.push_back(to);
                }
            }
            expected_sum = accumulate(distance.begin(), distance.end(), 0LL);
            assert(answers[root].sum == expected_sum);
            assert(answers[root].count == n);
        }
    }
}

int component_count(int n,
                    const vector<pair<int, int>>& edges,
                    int removed_edge = -1,
                    int removed_vertex = -1) {
    vector<vector<int>> graph(n);
    for (int id = 0; id < static_cast<int>(edges.size()); ++id) {
        if (id == removed_edge) continue;
        const auto [u, v] = edges[id];
        if (u == removed_vertex || v == removed_vertex) continue;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    int components = 0;
    vector<bool> visited(n, false);
    if (removed_vertex != -1) visited[removed_vertex] = true;
    for (int start = 0; start < n; ++start) {
        if (visited[start]) continue;
        ++components;
        vector<int> stack{start};
        visited[start] = true;
        while (!stack.empty()) {
            const int vertex = stack.back();
            stack.pop_back();
            for (const int to : graph[vertex]) {
                if (visited[to]) continue;
                visited[to] = true;
                stack.push_back(to);
            }
        }
    }
    return components;
}

void test_lowlink() {
    for (int trial = 0; trial < 1000; ++trial) {
        const int n = random_int(1, 9);
        const int m = random_int(0, 18);
        vector<pair<int, int>> edges;
        for (int i = 0; i < m; ++i) {
            edges.push_back({random_int(0, n - 1), random_int(0, n - 1)});
        }
        const LowLink lowlink(n, edges);
        const int base = component_count(n, edges);
        for (int id = 0; id < m; ++id) {
            assert(static_cast<bool>(lowlink.bridge_flags()[id]) ==
                   (component_count(n, edges, id) > base));
        }
        for (int vertex = 0; vertex < n; ++vertex) {
            assert(static_cast<bool>(lowlink.articulation_flags()[vertex]) ==
                   (component_count(n, edges, -1, vertex) > base));
        }

        const TwoEdgeConnectedComponents components(lowlink);
        for (int left = 0; left < n; ++left) {
            for (int right = 0; right < n; ++right) {
                vector<bool> visited(n, false);
                vector<int> stack{left};
                visited[left] = true;
                while (!stack.empty()) {
                    const int vertex = stack.back();
                    stack.pop_back();
                    for (const auto& [to, edge_id] : lowlink.graph()[vertex]) {
                        if (lowlink.bridge_flags()[edge_id] || visited[to]) continue;
                        visited[to] = true;
                        stack.push_back(to);
                    }
                }
                assert((components[left] == components[right]) == visited[right]);
            }
        }
    }
}

void test_dijkstra() {
    for (int trial = 0; trial < 300; ++trial) {
        const int n = random_int(1, 20);
        WeightedGraph graph(n);
        vector<vector<ll>> distance(n, vector<ll>(n, inf));
        for (int i = 0; i < n; ++i) distance[i][i] = 0;
        int edge_id = 0;
        for (int from = 0; from < n; ++from) {
            for (int to = 0; to < n; ++to) {
                if (random_int(0, 4) != 0) continue;
                const ll cost = random_int(0, 50);
                graph[from].push_back({to, cost, edge_id++});
                chmin(distance[from][to], cost);
            }
        }
        for (int middle = 0; middle < n; ++middle) {
            for (int from = 0; from < n; ++from) {
                for (int to = 0; to < n; ++to) {
                    if (distance[from][middle] == inf ||
                        distance[middle][to] == inf) continue;
                    chmin(distance[from][to],
                          distance[from][middle] + distance[middle][to]);
                }
            }
        }
        const int source = random_int(0, n - 1);
        const auto result = dijkstra(graph, source, inf);
        assert(result.dist == distance[source]);
        for (int target = 0; target < n; ++target) {
            const auto path = result.restore_vertices(target);
            assert(path.empty() == (distance[source][target] == inf));
            if (!path.empty()) {
                assert(path.front() == source && path.back() == target);
            }
        }
    }
}

void test_rolling_hash() {
    for (int trial = 0; trial < 500; ++trial) {
        string left(random_int(0, 80), 'a');
        string right(random_int(0, 80), 'a');
        for (char& value : left) value += random_int(0, 3);
        for (char& value : right) value += random_int(0, 3);
        const RollingHash left_hash(left), right_hash(right);
        for (int query = 0; query < 100; ++query) {
            int l1 = random_int(0, static_cast<int>(left.size()));
            int r1 = random_int(l1, static_cast<int>(left.size()));
            int l2 = random_int(0, static_cast<int>(right.size()));
            int r2 = random_int(l2, static_cast<int>(right.size()));
            if (r1 - l1 == r2 - l2) {
                assert((left_hash.get(l1, r1) == right_hash.get(l2, r2)) ==
                       (left.substr(l1, r1 - l1) == right.substr(l2, r2 - l2)));
            }
            int expected_lcp = 0;
            while (l1 + expected_lcp < r1 && l2 + expected_lcp < r2 &&
                   left[l1 + expected_lcp] == right[l2 + expected_lcp]) {
                ++expected_lcp;
            }
            assert(left_hash.lcp(right_hash, l1, r1, l2, r2) == expected_lcp);
        }
    }
}

void test_li_chao_tree() {
    struct SegmentLine {
        ll slope;
        ll intercept;
        int left;
        int right;
    };
    for (int trial = 0; trial < 300; ++trial) {
        LiChaoTree<ll> tree(-30, 30);
        vector<SegmentLine> lines;
        for (int operation = 0; operation < 200; ++operation) {
            if (lines.empty() || random_int(0, 2) != 0) {
                const ll slope = random_int(-30, 30);
                const ll intercept = random_int(-100, 100);
                if (random_int(0, 1) == 0) {
                    tree.add_line(slope, intercept);
                    lines.push_back({slope, intercept, -30, 30});
                } else {
                    int left = random_int(-30, 30);
                    int right = random_int(left, 30);
                    tree.add_segment(slope, intercept, left, right);
                    lines.push_back({slope, intercept, left, right});
                }
            } else {
                const int x = random_int(-30, 30);
                ll expected = numeric_limits<ll>::max();
                for (const auto& line : lines) {
                    if (line.left <= x && x <= line.right) {
                        chmin(expected, line.slope * x + line.intercept);
                    }
                }
                assert(tree.query(x) == expected);
            }
        }
    }
}

bool naive_is_prime(uint64_t value) {
    if (value < 2) return false;
    for (uint64_t divisor = 2; divisor * divisor <= value; ++divisor) {
        if (value % divisor == 0) return false;
    }
    return true;
}

void test_factorization() {
    for (uint64_t value = 1; value <= 100000; ++value) {
        assert(is_prime_u64(value) == naive_is_prime(value));
    }
    vector<uint64_t> values{
        1,
        2,
        9999999967ULL,
        1000000007ULL * 1000000009ULL,
        999381247093216751ULL,
        numeric_limits<uint64_t>::max(),
    };
    for (int i = 0; i < 200; ++i) {
        values.push_back(static_cast<uint64_t>(random_int(1, 1000000)) *
                         static_cast<uint64_t>(random_int(1, 1000000)));
    }
    for (const uint64_t value : values) {
        const auto factors = factorize_u64(value);
        __uint128_t product = 1;
        for (const uint64_t factor : factors) {
            assert(is_prime_u64(factor));
            product *= factor;
        }
        assert(product == value);
        assert(is_sorted(factors.begin(), factors.end()));
    }
}

}  // namespace

int main() {
    test_template_and_segtree();
    test_combination_and_sieve();
    test_order_statistics_and_potential_dsu();
    test_cycle_detection();
    test_hld_and_rerooting();
    test_lowlink();
    test_dijkstra();
    test_rolling_hash();
    test_li_chao_tree();
    test_factorization();
}
