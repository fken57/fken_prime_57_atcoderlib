#pragma once

#include "../template.hpp"

class RollingHash {
public:
    using Hash = uint64_t;
    static constexpr Hash modulus = (Hash(1) << 61) - 1;

    RollingHash() : prefix_(1, 0) {}
    explicit RollingHash(string_view text) { assign(text); }

    void assign(string_view text) {
        ensure_powers(text.size());
        prefix_.assign(text.size() + 1, 0);
        for (size_t i = 0; i < text.size(); ++i) {
            prefix_[i + 1] = add(multiply(prefix_[i], base()),
                                 static_cast<unsigned char>(text[i]) + 1);
        }
    }

    int size() const { return static_cast<int>(prefix_.size()) - 1; }

    Hash get(int left, int right) const {
        assert(0 <= left && left <= right && right <= size());
        return subtract(prefix_[right],
                        multiply(prefix_[left], powers_[right - left]));
    }

    static Hash concat(Hash left, Hash right, int right_length) {
        assert(right_length >= 0);
        ensure_powers(right_length);
        return add(multiply(left, powers_[right_length]), right);
    }

    int lcp(const RollingHash& other,
            int left1,
            int right1,
            int left2,
            int right2) const {
        assert(0 <= left1 && left1 <= right1 && right1 <= size());
        assert(0 <= left2 && left2 <= right2 && right2 <= other.size());
        int low = 0;
        int high = min(right1 - left1, right2 - left2) + 1;
        while (high - low > 1) {
            const int middle = (low + high) / 2;
            if (get(left1, left1 + middle) ==
                other.get(left2, left2 + middle)) {
                low = middle;
            } else {
                high = middle;
            }
        }
        return low;
    }

private:
    static Hash add(Hash left, Hash right) {
        Hash result = left + right;
        if (result >= modulus) result -= modulus;
        return result;
    }

    static Hash subtract(Hash left, Hash right) {
        return left >= right ? left - right : left + modulus - right;
    }

    static Hash multiply(Hash left, Hash right) {
        const __uint128_t product =
            static_cast<__uint128_t>(left) * right;
        Hash result = static_cast<Hash>(product >> 61) +
                      static_cast<Hash>(product & modulus);
        if (result >= modulus) result -= modulus;
        return result;
    }

    static Hash base() {
        static const Hash value = [] {
            const Hash low = Hash(1) << 20;
            const Hash high = modulus - low;
            const Hash seed = static_cast<Hash>(
                chrono::steady_clock::now().time_since_epoch().count());
            mt19937_64 engine(seed ^ random_device{}());
            uniform_int_distribution<Hash> distribution(low, high);
            return distribution(engine);
        }();
        return value;
    }

    static void ensure_powers(size_t size) {
        if (powers_.size() > size) return;
        const size_t old_size = powers_.size();
        powers_.resize(size + 1);
        for (size_t i = old_size; i <= size; ++i) {
            powers_[i] = multiply(powers_[i - 1], base());
        }
    }

    inline static vector<Hash> powers_{1};
    vector<Hash> prefix_;
};
