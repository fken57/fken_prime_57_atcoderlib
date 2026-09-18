#pragma once

#include "../template.hpp"

// Factorial-based combinations for a prime modulus.
// The largest precomputed n must be smaller than Mint::mod().
template <class Mint>
class Combination {
public:
    Combination() : fact_(1, Mint(1)), ifact_(1, Mint(1)) {}
    explicit Combination(int n) : Combination() { ensure(n); }

    void ensure(int n) {
        assert(n >= 0);
        if (n < static_cast<int>(fact_.size())) return;
        assert(static_cast<unsigned long long>(n) <
                   static_cast<unsigned long long>(Mint::mod()) &&
               "factorial combination requires n < mod");

        const int old_size = static_cast<int>(fact_.size());
        fact_.resize(n + 1);
        ifact_.resize(n + 1);
        for (int i = old_size; i <= n; ++i) fact_[i] = fact_[i - 1] * i;
        ifact_[n] = fact_[n].inv();
        for (int i = n; i >= old_size; --i) ifact_[i - 1] = ifact_[i] * i;
    }

    Mint factorial(int n) {
        if (n < 0) return Mint(0);
        ensure(n);
        return fact_[n];
    }

    Mint inverse_factorial(int n) {
        if (n < 0) return Mint(0);
        ensure(n);
        return ifact_[n];
    }

    Mint C(long long n, long long k) {
        if (n < 0 || k < 0 || k > n) return Mint(0);
        assert(n <= std::numeric_limits<int>::max());
        ensure(static_cast<int>(n));
        return fact_[n] * ifact_[k] * ifact_[n - k];
    }

    Mint P(long long n, long long k) {
        if (n < 0 || k < 0 || k > n) return Mint(0);
        assert(n <= std::numeric_limits<int>::max());
        ensure(static_cast<int>(n));
        return fact_[n] * ifact_[n - k];
    }

    Mint H(long long n, long long k) {
        if (n < 0 || k < 0) return Mint(0);
        if (n == 0) return Mint(k == 0 ? 1 : 0);
        return C(n + k - 1, k);
    }

private:
    vector<Mint> fact_;
    vector<Mint> ifact_;
};

// Backward-compatible interface. It follows the current `mint` alias.
inline Combination<mint> fib_default_combination;

inline void COMinit(int max_n = 1009999) {
    fib_default_combination.ensure(max_n);
}

inline mint COM(long long n, long long k) {
    return fib_default_combination.C(n, k);
}

inline mint PERM(long long n, long long k) {
    return fib_default_combination.P(n, k);
}

inline mint HOM(long long n, long long k) {
    return fib_default_combination.H(n, k);
}
