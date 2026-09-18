#pragma once

#include "../template.hpp"

namespace fib_arbitrary_combination_detail {

inline long long multiply_mod(long long left,
                              long long right,
                              int modulus) {
    return static_cast<long long>(
        static_cast<__int128>(left) * right % modulus);
}

inline long long power_mod(long long base,
                           long long exponent,
                           int modulus) {
    long long result = 1 % modulus;
    while (exponent > 0) {
        if (exponent & 1) result = multiply_mod(result, base, modulus);
        base = multiply_mod(base, base, modulus);
        exponent >>= 1;
    }
    return result;
}

class PrimePowerCombination {
public:
    PrimePowerCombination(int prime, int exponent)
        : prime_(prime), exponent_(exponent), modulus_(1) {
        assert(prime_ >= 2 && exponent_ >= 1);
        for (int i = 0; i < exponent_; ++i) {
            assert(1LL * modulus_ * prime_ <=
                   numeric_limits<int>::max());
            modulus_ *= prime_;
        }

        factorial_.resize(modulus_);
        inverse_factorial_.resize(modulus_);
        factorial_[0] = 1;
        for (int value = 1; value < modulus_; ++value) {
            factorial_[value] = factorial_[value - 1];
            if (value % prime_ != 0) {
                factorial_[value] = static_cast<int>(multiply_mod(
                    factorial_[value], value, modulus_));
            }
        }

        inverse_factorial_[modulus_ - 1] = static_cast<int>(
            atcoder::inv_mod(factorial_[modulus_ - 1], modulus_));
        for (int value = modulus_ - 1; value >= 1; --value) {
            inverse_factorial_[value - 1] = inverse_factorial_[value];
            if (value % prime_ != 0) {
                inverse_factorial_[value - 1] =
                    static_cast<int>(multiply_mod(
                        inverse_factorial_[value], value, modulus_));
            }
        }
        period_product_ =
            prime_ == 2 && exponent_ >= 3 ? 1 : modulus_ - 1;
    }

    int modulus() const { return modulus_; }

    long long C(long long n, long long k) const {
        if (n < 0 || k < 0 || k > n) return 0;
        long long remainder = n - k;
        int prime_exponent = 0;
        int period_exponent = 0;
        int digit = 0;
        long long result = 1;

        while (n > 0) {
            result = multiply_mod(result, factorial_[n % modulus_],
                                  modulus_);
            result = multiply_mod(result, inverse_factorial_[k % modulus_],
                                  modulus_);
            result = multiply_mod(
                result, inverse_factorial_[remainder % modulus_], modulus_);

            n /= prime_;
            k /= prime_;
            remainder /= prime_;
            const int carry = static_cast<int>(n - k - remainder);
            prime_exponent += carry;
            if (prime_exponent >= exponent_) return 0;
            if (++digit >= exponent_) period_exponent += carry;
        }

        result = multiply_mod(
            result,
            power_mod(period_product_, period_exponent, modulus_),
            modulus_);
        result = multiply_mod(
            result, power_mod(prime_, prime_exponent, modulus_), modulus_);
        return result;
    }

private:
    int prime_;
    int exponent_;
    int modulus_;
    int period_product_;
    vector<int> factorial_;
    vector<int> inverse_factorial_;
};

}  // namespace fib_arbitrary_combination_detail

// Binomial coefficient modulo any positive 32-bit modulus, including a
// composite modulus. Construction takes O(modulus) total memory and time;
// each query takes O(number of prime factors * log(n)).
class ArbitraryModCombination {
public:
    explicit ArbitraryModCombination(int modulus) : modulus_(modulus) {
        assert(modulus_ >= 1);
        int remaining = modulus_;
        for (int prime = 2; 1LL * prime * prime <= remaining; ++prime) {
            if (remaining % prime != 0) continue;
            int exponent = 0;
            int prime_power = 1;
            do {
                remaining /= prime;
                ++exponent;
                prime_power *= prime;
            } while (remaining % prime == 0);
            component_moduli_.push_back(prime_power);
            components_.emplace_back(prime, exponent);
        }
        if (remaining > 1) {
            component_moduli_.push_back(remaining);
            components_.emplace_back(remaining, 1);
        }
    }

    int modulus() const { return modulus_; }

    long long C(long long n, long long k) const {
        if (n < 0 || k < 0 || k > n || modulus_ == 1) return 0;
        vector<long long> remainders;
        vector<long long> moduli;
        remainders.reserve(components_.size());
        moduli.reserve(components_.size());
        for (int index = 0; index < static_cast<int>(components_.size());
             ++index) {
            remainders.push_back(components_[index].C(n, k));
            moduli.push_back(component_moduli_[index]);
        }
        return atcoder::crt(remainders, moduli).first;
    }

private:
    int modulus_;
    vector<int> component_moduli_;
    vector<fib_arbitrary_combination_detail::PrimePowerCombination>
        components_;
};
