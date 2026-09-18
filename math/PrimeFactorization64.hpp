#pragma once

#include "../template.hpp"

inline uint64_t multiply_mod_u64(uint64_t left,
                                 uint64_t right,
                                 uint64_t modulus) {
    return static_cast<uint64_t>(
        static_cast<__uint128_t>(left) * right % modulus);
}

inline uint64_t power_mod_u64(uint64_t base,
                              uint64_t exponent,
                              uint64_t modulus) {
    uint64_t result = 1 % modulus;
    while (exponent != 0) {
        if (exponent & 1) result = multiply_mod_u64(result, base, modulus);
        base = multiply_mod_u64(base, base, modulus);
        exponent >>= 1;
    }
    return result;
}

inline bool is_prime_u64(uint64_t value) {
    if (value < 2) return false;
    for (const uint64_t prime : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL,
                                 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL}) {
        if (value % prime == 0) return value == prime;
    }

    uint64_t odd = value - 1;
    int shift = 0;
    while ((odd & 1) == 0) {
        odd >>= 1;
        ++shift;
    }

    for (const uint64_t base : {2ULL, 325ULL, 9375ULL, 28178ULL,
                                450775ULL, 9780504ULL, 1795265022ULL}) {
        if (base % value == 0) continue;
        uint64_t current = power_mod_u64(base % value, odd, value);
        if (current == 1 || current == value - 1) continue;
        bool composite = true;
        for (int i = 1; i < shift; ++i) {
            current = multiply_mod_u64(current, current, value);
            if (current == value - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

namespace fib_factorization_detail {

inline mt19937_64& engine() {
    static mt19937_64 value(static_cast<uint64_t>(
        chrono::steady_clock::now().time_since_epoch().count()));
    return value;
}

inline uint64_t pollard_rho(uint64_t value) {
    if (value % 2 == 0) return 2;
    if (value % 3 == 0) return 3;

    while (true) {
        uniform_int_distribution<uint64_t> distribution(1, value - 1);
        uint64_t y = distribution(engine());
        const uint64_t constant = distribution(engine());
        constexpr uint64_t block_size = 128;
        uint64_t factor = 1;
        uint64_t length = 1;
        uint64_t x = 0;
        uint64_t saved_y = 0;

        const auto next = [&](uint64_t current) {
            return static_cast<uint64_t>(
                (static_cast<__uint128_t>(
                     multiply_mod_u64(current, current, value)) +
                 constant) %
                value);
        };

        while (factor == 1) {
            x = y;
            for (uint64_t i = 0; i < length; ++i) y = next(y);
            for (uint64_t offset = 0;
                 offset < length && factor == 1;
                 offset += block_size) {
                saved_y = y;
                uint64_t product = 1;
                const uint64_t count = min(block_size, length - offset);
                for (uint64_t i = 0; i < count; ++i) {
                    y = next(y);
                    const uint64_t difference = x > y ? x - y : y - x;
                    product = multiply_mod_u64(product, difference, value);
                }
                factor = gcd(product, value);
            }
            length <<= 1;
        }

        if (factor == value) {
            do {
                saved_y = next(saved_y);
                const uint64_t difference =
                    x > saved_y ? x - saved_y : saved_y - x;
                factor = gcd(difference, value);
            } while (factor == 1);
        }
        if (factor != value) return factor;
    }
}

inline void factor_recursively(uint64_t value, vector<uint64_t>& factors) {
    if (value == 1) return;
    if (is_prime_u64(value)) {
        factors.push_back(value);
        return;
    }
    const uint64_t divisor = pollard_rho(value);
    factor_recursively(divisor, factors);
    factor_recursively(value / divisor, factors);
}

}  // namespace fib_factorization_detail

inline vector<uint64_t> factorize_u64(uint64_t value) {
    assert(value >= 1 && "factorize_u64 requires a positive integer");
    vector<uint64_t> factors;
    fib_factorization_detail::factor_recursively(value, factors);
    sort(factors.begin(), factors.end());
    return factors;
}

inline vector<pair<uint64_t, int>> factorize_u64_with_exponents(
    uint64_t value) {
    const auto factors = factorize_u64(value);
    vector<pair<uint64_t, int>> result;
    for (const uint64_t factor : factors) {
        if (result.empty() || result.back().first != factor) {
            result.push_back({factor, 1});
        } else {
            ++result.back().second;
        }
    }
    return result;
}
