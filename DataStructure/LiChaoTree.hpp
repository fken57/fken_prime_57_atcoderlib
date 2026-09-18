#pragma once

#include "../template.hpp"

template <class T>
class LiChaoTree {
public:
    struct Line {
        T slope = 0;
        T intercept = std::numeric_limits<T>::max();

        __int128 evaluate_wide(T x) const {
            return static_cast<__int128>(slope) * x + intercept;
        }

        T operator()(T x) const {
            return static_cast<T>(evaluate_wide(x));
        }
    };

    LiChaoTree(T minimum_x, T maximum_x)
        : minimum_x_(minimum_x), maximum_x_(maximum_x) {
        assert(minimum_x_ <= maximum_x_);
    }

    void add_line(T slope, T intercept) {
        add_line(root_, minimum_x_, maximum_x_, {slope, intercept});
    }

    void add_segment(T slope,
                     T intercept,
                     T segment_left,
                     T segment_right) {
        if (segment_right < minimum_x_ || maximum_x_ < segment_left) return;
        segment_left = max(segment_left, minimum_x_);
        segment_right = min(segment_right, maximum_x_);
        add_segment(root_, minimum_x_, maximum_x_, segment_left, segment_right,
                    {slope, intercept});
    }

    T query(T x) const {
        assert(minimum_x_ <= x && x <= maximum_x_);
        const __int128 answer = query(root_.get(), minimum_x_, maximum_x_, x);
        if (answer >= wide_infinity()) return std::numeric_limits<T>::max();
        return static_cast<T>(answer);
    }

private:
    struct Node {
        Line line;
        bool has_line = false;
        unique_ptr<Node> left;
        unique_ptr<Node> right;
    };

    static constexpr __int128 wide_infinity() {
        return static_cast<__int128>(std::numeric_limits<T>::max());
    }

    static bool better(const Line& left, const Line& right, T x) {
        return left.evaluate_wide(x) < right.evaluate_wide(x);
    }

    static T midpoint(T left, T right) {
        return static_cast<T>(
            static_cast<__int128>(left) +
            (static_cast<__int128>(right) - left) / 2);
    }

    void add_line(unique_ptr<Node>& node, T left, T right, Line line) {
        if (!node) node = make_unique<Node>();
        if (!node->has_line) {
            node->line = line;
            node->has_line = true;
            return;
        }

        const T middle = midpoint(left, right);
        const bool left_better = better(line, node->line, left);
        const bool middle_better = better(line, node->line, middle);
        if (middle_better) swap(line, node->line);
        if (left == right) return;

        if (left_better != middle_better) {
            add_line(node->left, left, middle, line);
        } else {
            add_line(node->right, middle + 1, right, line);
        }
    }

    void add_segment(unique_ptr<Node>& node,
                     T left,
                     T right,
                     T query_left,
                     T query_right,
                     const Line& line) {
        if (query_right < left || right < query_left) return;
        if (query_left <= left && right <= query_right) {
            add_line(node, left, right, line);
            return;
        }
        if (!node) node = make_unique<Node>();
        const T middle = midpoint(left, right);
        add_segment(node->left, left, middle, query_left, query_right, line);
        add_segment(node->right, middle + 1, right, query_left, query_right,
                    line);
    }

    __int128 query(const Node* node, T left, T right, T x) const {
        if (node == nullptr) return wide_infinity();
        __int128 result =
            node->has_line ? node->line.evaluate_wide(x) : wide_infinity();
        if (left == right) return result;
        const T middle = midpoint(left, right);
        if (x <= middle) {
            result = min(result, query(node->left.get(), left, middle, x));
        } else {
            result = min(result,
                         query(node->right.get(), middle + 1, right, x));
        }
        return result;
    }

    T minimum_x_;
    T maximum_x_;
    unique_ptr<Node> root_;
};
