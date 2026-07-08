#include "../templete.hpp"

vll Compress(vll A){
    ll Asize=A.size();
    vll PressA=A;
    sort(PressA.begin(),PressA.end());
    PressA.erase(unique(PressA.begin(),PressA.end()),PressA.end());
    vll cnt;
    rep(i,0,Asize){
        ll x=lower_bound(PressA.begin(),PressA.end(),A[i])-PressA.begin();
        cnt.push_back(x);
    }
    return cnt;
}