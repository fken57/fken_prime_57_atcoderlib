#include "../../templete.hpp";
#include "../extGCD.hpp";

/*@param .firstがak .secondがb*/
struct LinerDiphantineEquationAns{
    bool isSolveable;
    pair<ll,ll> xans;
    pair<ll,ll> yans;
};

/*@param 一時不定方程式をsk+tの形で求めます*/
LinerDiphantineEquationAns LinerDiphantineEquation(ll a,ll b,ll m){
    ll s=0;ll t=0;
    ll g=extGCD(a,b,s,t);
    if(m%abs(g)!=0 || a==0 || b==0){
        LinerDiphantineEquationAns cnt = {false,{0,0},{0,0}};
        return cnt;
    }

    ll zoom=m/g;
    s*=zoom;t*=zoom;

    ll abgcd=gcd(a,b);

    ll xansk= abs(b)/abgcd;
    ll yansk= abs(a)/abgcd;

    return{
        true,
        {xansk,s},
        {yansk,t}
    };
}

/*@param ax≡b(modM)の解の個数を1~Nで求める*/
ll AXBModMEquation (ll a,ll b,ll M,ll N){
    if (M == 0) {
        if (a == 0) return (b == 0 ? N : 0);
        if (b % a == 0) {
            ll sol = b / a;
            return (sol >= 1 && sol <= N ? 1 : 0);
        }
        return 0;
    }

    if(a == 0){
        return (b % M == 0 ? N : 0);
    }
    LinerDiphantineEquationAns cnt = LinerDiphantineEquation(a,M,-b);

    if(!cnt.isSolveable) return 0;

    ll x=cnt.xans.first;
    ll y=cnt.xans.second;

     y=(y % x + x) % x;
    if(y==0) y=x;
    if(N< y) return 0;
    return (N-y)/x+1;
}