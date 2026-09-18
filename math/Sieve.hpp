#pragma once

#include "../template.hpp"

class Eratosthenes {
private:
    int maximum_;

    static int checked_maximum(int maximum) {
        assert(maximum >= 0);
        return maximum;
    }

public:
    vector<bool> isprime;
    vector<int> minfactor;
    vector<int> mobius;
    vector<int> phi;
    vector<int> primes;

    explicit Eratosthenes(int maximum)
        : maximum_(checked_maximum(maximum)),
          isprime(maximum_ + 1, false),
          minfactor(maximum_ + 1, 0),
          mobius(maximum_ + 1, 0),
          phi(maximum_ + 1, 0) {
        if (maximum_ >= 1) {
            minfactor[1] = 1;
            mobius[1] = 1;
            phi[1] = 1;
        }

        for (int value = 2; value <= maximum_; ++value) {
            if (minfactor[value] == 0) {
                minfactor[value] = value;
                isprime[value] = true;
                mobius[value] = -1;
                phi[value] = value - 1;
                primes.push_back(value);
            }
            for (const int prime : primes) {
                const long long composite = 1LL * value * prime;
                if (composite > maximum_) break;
                minfactor[composite] = prime;
                if (value % prime == 0) {
                    mobius[composite] = 0;
                    phi[composite] = phi[value] * prime;
                    break;
                }
                mobius[composite] = -mobius[value];
                phi[composite] = phi[value] * (prime - 1);
            }
        }
    }

    int maximum() const { return maximum_; }

    vector<pair<ll, ll>> factorize(ll value) const {
        assert(1 <= value && value <= maximum_);
        vector<pair<ll, ll>> result;
        while (value > 1) {
            const int prime = minfactor[value];
            int exponent = 0;
            do {
                value /= prime;
                ++exponent;
            } while (value > 1 && minfactor[value] == prime);
            result.push_back({prime, exponent});
        }
        return result;
    }

    vector<ll> divisors(ll value) const {
        vector<ll> result{1};
        for (const auto& [prime, exponent] : factorize(value)) {
            const int old_size = static_cast<int>(result.size());
            ll power = 1;
            for (int count = 1; count <= exponent; ++count) {
                power *= prime;
                for (int i = 0; i < old_size; ++i) {
                    result.push_back(result[i] * power);
                }
            }
        }
        sort(result.begin(), result.end());
        return result;
    }

};

template <class T>
void fast_zeta(vector<T>& values) {
    if (values.size() <= 2) return;
    Eratosthenes sieve(static_cast<int>(values.size()) - 1);
    const int n = static_cast<int>(values.size()) - 1;
    for (const int prime : sieve.primes) {
        for (int value = n / prime; value >= 1; --value) {
            values[value] += values[value * prime];
        }
    }
}

template <class T>
void fast_mobius(vector<T>& values) {
    if (values.size() <= 2) return;
    Eratosthenes sieve(static_cast<int>(values.size()) - 1);
    const int n = static_cast<int>(values.size()) - 1;
    for (const int prime : sieve.primes) {
        for (int value = 1; value * prime <= n; ++value) {
            values[value] -= values[value * prime];
        }
    }
}

inline vll euler_phi_table(int maximum) {
    const Eratosthenes sieve(maximum);
    return vll(sieve.phi.begin(), sieve.phi.end());
}

inline vll Eular_func(ll maximum) {
    assert(0 <= maximum && maximum <= numeric_limits<int>::max());
    return euler_phi_table(static_cast<int>(maximum));
}
