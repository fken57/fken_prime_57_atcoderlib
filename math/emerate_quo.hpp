#include "../templete.hpp"


/*@param 商列挙lz形式で<要素、個数>を昇順で返します*/
vector<pair<ll,ll>> emerateQuo(ll N){
    vector<pair<ll,ll>> cnt;
    ll up=0;
    for(ll i=1;i*i<=N;i++){
        cnt.push_back({i,(N/i)-(N/(i+1))});
        up=N/(i+1);
    }
    rep(i,1,up+1){
        cnt.push_back({N/i,1});
    }
    sort(cnt.begin(),cnt.end());
    return cnt;
}