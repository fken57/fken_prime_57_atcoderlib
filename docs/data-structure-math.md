# Data structure / Math / String

## Indexed set / map

`DataStructure/OrderStatisticTree.hpp` は GNU PBDS を利用する任意追加ヘッダです。
標準テンプレートには含まれません。

```cpp
OrderedSet<ll> values;
values.insert(10);
values.insert(30);
values.order_of_key(30); // 30 未満の個数 = 1
*values.find_by_order(0); // 0-indexed k-th = 10

OrderedMap<ll, string> map;
map[5] = "five";
map.find_by_order(0)->second;
```

`find_by_order(k)` は範囲外なら `end()` です。GNU 拡張なので GCC/libstdc++ 環境向けです。

## Potential DSU

`PotentialDSU<T>` は加法群向けです。

```cpp
PotentialDSU<ll> dsu(n);
dsu.merge(a, b, d); // potential(b) - potential(a) = d を追加
dsu.same(a, b);
dsu.diff(a, b);     // potential(b) - potential(a)
dsu.diff_if_same(a, b); // optional<T>
```

既知の制約と矛盾する `merge` だけが `false` を返します。`ll` のほか modint も使えます。

## Li Chao Tree

`LiChaoTree<T>(minimum_x, maximum_x)` は整数閉区間上の最小値版です。

```cpp
LiChaoTree<ll> tree(-1000000000LL, 1000000000LL);
tree.add_line(a, b);             // y = ax + b
tree.add_segment(a, b, l, r);   // l <= x <= r のみ
ll answer = tree.query(x);
```

比較時は `__int128` を使います。直線が存在しない点の query は
`numeric_limits<T>::max()` を返します。

## Combination

`math/Combination.hpp` は利用する `Mint` を型で選び、必要な最大値まで遅延前計算します。

```cpp
Combination<mint107> comb(1000000);
comb.C(n, k);
comb.P(n, k);
comb.H(n, k);

dynamic_mint<2>::set_mod(1000000007);
Combination<dynamic_mint<2>> runtime_comb;
```

この階乗・逆階乗方式は modulus が素数で、前計算最大値が modulus 未満である場合を対象にします。
従来の使用感も維持しています。

```cpp
COMinit();
mint answer = COM(n, k);
```

`COM` は `mint`（既定では `mint998`）に従います。固定長の巨大な global 配列は持ちません。

複合数を含む任意 modulus と巨大な `n` には `math/ArbitraryModCombination.hpp` を使います。

```cpp
ArbitraryModCombination comb(modulus);
long long answer = comb.C(n, k);
```

素数冪ごとに計算して CRT で復元します。構築は `O(modulus)` 時間・メモリ、1 query は
modulus の異なる素因数の個数と `log n` に比例します。`modulus = 1` にも対応します。

## Sieve / 64-bit factorization

`math/Sieve.hpp` の `Eratosthenes(maximum)` は線形時間の前計算で、`isprime`, `minfactor`,
`mobius`, `phi`, `primes` を公開します。`factorize`, `divisors`, `fast_zeta`, `fast_mobius`,
`euler_phi_table` も利用できます。旧スペル `math/Eratosthnes.hpp` は互換ヘッダです。

前計算範囲を超える 64-bit 整数には `math/PrimeFactorization64.hpp` を使います。

```cpp
is_prime_u64(value);
factorize_u64(value);                 // 素因数を重複込み昇順で返す
factorize_u64_with_exponents(value);  // (prime, exponent)
```

Miller–Rabin の決定的底と Pollard–Rho を使い、入力は正整数です。

## Rolling Hash

`String/RollingHash.hpp` は法 `2^61-1` と process ごとのランダム base を使います。

```cpp
RollingHash hash(text);
auto value = hash.get(left, right);
auto joined = RollingHash::concat(left_hash, right_hash, right_length);
int length = hash.lcp(other, l1, r1, l2, r2);
```

hash 一致は確率的です。厳密比較が必要な最終候補では元文字列も確認してください。
