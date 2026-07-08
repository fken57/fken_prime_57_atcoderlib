#include "../templete.hpp"

struct Eratosthenes{
    vector<bool> isprime;//素数であるか
    vector<int> minfactor;//最小素因数
    vector<int> mobius;//メビウス関数の列挙

    Eratosthenes(ll N):isprime(N+1,true),minfactor(N+1,-1),mobius(N+1,1){
        isprime[1]=false;
        minfactor[1]=1;
        for(ll i=2;i<=N;i++){
            if(!isprime[i]) continue;
            minfactor[i]=i;
            mobius[i]=-1;
            for(ll j=i*2;j<=N;j+=i){
                isprime[j]=false;
                if(minfactor[j]==-1) minfactor[j]=i;
                if((j/i)%i==0) mobius[j]=0;
                else mobius[j]=-mobius[j];
            }
        }
    }

    vector<pair<ll,ll>> factorize(ll n){
        vector<pair<ll,ll>> res;
        while(n>1){
            int p=minfactor[n];
            int exp=0;
            while(minfactor[n]==p){
                n/=p;
                ++exp;
            }
            res.push_back({p,exp});
        }
        return res;
    }

    vector<ll> divisors(ll n){
        vector<ll> res({1});
        auto pf=factorize(n);
        for(auto p:pf){
            ll s=res.size();
            for(ll i=0;i<s;i++){
                ll v=1;
                for(ll j=0;j<p.second;++j){
                    v*=p.first;
                    res.push_back(res[i]*v);
                }
            }
        }
        sort(res.begin(), res.end());
        return res;
    }
};

//約数系の高速ゼータ変換
//f(1)…f(N)から約数和を求める。
template<class T> void fast_zeta(vector<T> &f){
    ll N=f.size();
    Eratosthenes Er(N);
    rep(p,2,N){
        if(!Er.isprime[p]) continue;
        for(ll k=(N-1)/p;k>=1;--k){
            f[k]+=f[k*p];
        }
    }
}
//高速メビウス変換
template<class T> void fast_mobius(vector<T> &f){
    ll N=f.size();
    Eratosthenes Er(N);
    rep(p,2,N){
        if(!Er.isprime[p]) continue;
        for(ll k=1;k*p<N;++k){
            f[k]-=f[k*p];
        }
    }
}

//オイラー関数
vll Eular_func(ll N){
    vll res(N+1);
    rep(i,0,N+1) res[i]=i;
    rep(i,2,N+1){
        if(res[i]==i){
            for(ll j=i;j<=N;j+=i){
                res[j]=res[j]/i*(i-1);
            }
        }
    }
    return res;
}