#include "../segtree/Monoids.hpp"
#include "../segtree/BinarySearch.hpp"
#include "../segtree/BracketSequence.hpp"
#include "../Graph/BFS.hpp"
#include "../Graph/Dijkstra.hpp"
#include "../Tree/TreeDoubling.hpp"
#include "../Tree/TreeDiameter.hpp"
#include "../Tree/HeavyLightDecomposition.hpp"

// No FIB_NO_MAIN: library headers must work with the caller's main.
template <class M>
void check_monoid(const std::vector<typename M::S>& values) {
    atcoder::segtree<typename M::S, M::op, M::e> seg(values);
    assert(M::op(M::e(), M::e()) == M::e());
    for (const auto& a : values) {
        assert(M::op(M::e(), a) == a);
        assert(M::op(a, M::e()) == a);
        for (const auto& b : values) {
            for (const auto& c : values) {
                assert(M::op(M::op(a, b), c) == M::op(a, M::op(b, c)));
            }
        }
    }
    for (int left = 0; left <= static_cast<int>(values.size()); ++left) {
        auto expected = M::e();
        assert(seg.prod(left, left) == expected);
        for (int right = left; right < static_cast<int>(values.size()); ++right) {
            expected = M::op(expected, values[right]);
            assert(seg.prod(left, right + 1) == expected);
        }
    }
}

void test_monoids() {
    check_monoid<RangeMinMonoid<int>>({-3, 2, INT_MAX});
    check_monoid<RangeMaxMonoid<int>>({INT_MIN, -3, 2});
    check_monoid<RangeSumMonoid<long long>>({3, -2, 5});
    check_monoid<RangeProductMonoid<int>>({2, 0, -3});
    check_monoid<RangeGcdMonoid<int>>({6, 0, 15});
    check_monoid<RangeLcmMonoid<int>>({6, 0, 15});
    check_monoid<RangeXorMonoid<unsigned>>({1, 3, 6});
    check_monoid<RangeAndMonoid<unsigned>>({1, 3, 6});
    check_monoid<RangeOrMonoid<unsigned>>({1, 3, 6});
    check_monoid<RangeMinMaxMonoid<int>>({{-3, -3}, {2, 2}, {5, 5}});
    check_monoid<RangeMinCountMonoid<int>>({{INT_MAX, 1}, {-3, 1}, {-3, 1}});
    check_monoid<RangeMaxCountMonoid<int>>({{INT_MIN, 1}, {3, 1}, {3, 1}});
    check_monoid<AffineCompositionMonoid<mint998>>({{2, 3}, {5, 7}, {0, 1}});
    check_monoid<BracketBalanceMonoid>({{1, 0}, {-1, -1}, {0, 0}});

    assert(RangeProductMonoid<int>::op(3, 4) == 12);
    assert(RangeGcdMonoid<int>::op(6, 15) == 3);
    assert(RangeLcmMonoid<int>::op(6, 15) == 30);
    assert(RangeXorMonoid<unsigned>::op(3, 6) == 5);
    assert(RangeAndMonoid<unsigned>::op(3, 6) == 2);
    assert(RangeOrMonoid<unsigned>::op(3, 6) == 7);
    using Counts = RangeMinCountMonoid<int>;
    assert((Counts::op({-3, 2}, {-3, 1}) == Counts::S{-3, 3}));
    using MaxCounts = RangeMaxCountMonoid<int>;
    assert((MaxCounts::op({3, 2}, {3, 1}) == MaxCounts::S{3, 3}));

    using M = RangeSumMonoid<long long>;
    atcoder::segtree<M::S, M::op, M::e> seg(std::vector<long long>{2, 0, 3, 1});
    assert(seg.prod(0, 3) == 5);
    assert(segtree_prefix_lower_bound(seg, 4) == 2); // int literal with ll sum
    assert(segtree_prefix_lower_bound(seg, 7) == 4);
    assert(segtree_kth(seg, 0) == 0);
    seg.set(0, 5);
    assert(seg.prod(0, 3) == 8);

    using F = AffineCompositionMonoid<mint998>;
    atcoder::segtree<F::S, F::op, F::e> functions(std::vector<F::S>{{2, 3}, {5, 7}});
    assert(F::apply(functions.prod(0, 2), 4).val() == 62); // 5*(2*4+3)+7
    assert(F::apply(functions.prod(0, 0), 4).val() == 4);
    BracketSequence brackets("(()())");
    assert(brackets.is_valid());
}

void test_graphs() {
    static_assert(std::is_same_v<Graph, std::vector<std::vector<int>>>);
    std::vector<std::vector<int>> graph{{1}, {0, 2}, {1}, {}};
    const auto result = bfs(graph, 0);
    assert(result.dist[2] == 2 && result.dist[3] == inf);
    assert((result.restore_path(2) == std::vector<int>{0, 1, 2}));
    graph.pop_back();
    TreeDoubling doubling(graph);
    HeavyLightDecomposition hld(graph);
    assert(doubling.lca(1, 2) == 1 && hld.lca(1, 2) == 1);
    assert(ExecuteTreeDiameter(graph).dir == 2);

    // Ordinary nested vectors with a cost-parametric edge struct.
    std::vector<std::vector<WeightedEdge<double>>> weighted{
        {{1, 1.5, 0}}, {{0, 1.5, 0}, {2, 2.25, 1}}, {{1, 2.25, 1}}};
    const auto shortest = dijkstra(weighted, 0);
    assert(shortest.dist[2] == 3.75);
    assert((shortest.restore_edges(2) == std::vector<int>{0, 1}));
    std::vector<double> dist;
    WeightedTreeBFS(0, weighted, dist);
    assert(dist[2] == 3.75);
    const auto diameter = ExecuteTreeDiameter(weighted);
    static_assert(std::is_same_v<decltype(diameter.dir), double>);
    assert(diameter.dir == 3.75);
}

int main() {
    test_monoids();
    test_graphs();
}
