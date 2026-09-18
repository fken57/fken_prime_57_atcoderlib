#pragma once

#include "../template.hpp"

template <class Integer>
constexpr Integer mod(Integer value, Integer modulus) {
    assert(modulus > 0);
    value %= modulus;
    if (value < 0) value += modulus;
    return value;
}

template <class Integer, class Exponent>
constexpr Integer modpow(Integer base, Exponent exponent, Integer modulus) {
    assert(modulus > 0);
    assert(exponent >= 0);
    base = mod(base, modulus);
    Integer result = Integer{1} % modulus;
    while (exponent > 0) {
        if (exponent & 1) {
            result = static_cast<Integer>(
                static_cast<__int128_t>(result) * base % modulus);
        }
        base = static_cast<Integer>(
            static_cast<__int128_t>(base) * base % modulus);
        exponent >>= 1;
    }
    return result;
}
