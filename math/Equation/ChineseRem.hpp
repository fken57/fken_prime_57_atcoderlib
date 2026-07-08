#include "../../templete.hpp";
#include "../extGCD.hpp";

struct ChineseRemAns{
    bool isSolvable;
    ll r;
    ll Complexmod;
};

ChineseRemAns ChineseRem(const vector<ll> &b,const vector<ll> &m){
    ll r=0;ll M=1;
    for(int i=0; i<(int)b.size();++i){
        ll s,t;
        ll d=extGCD(M,m[i],s,t);
        if(mod(b[i]-r,d)!=0){
            return{
                false,
                0,
                0
            };
        }
        ll temp=(b[i]-r)/d*s%(m[i]/d);
        ll lcmCRT = m[i] * (M/d);

        r=mod(r+M*temp,lcmCRT);
        M=lcmCRT;
    }
    return{
        true,
        r,
        M
    };
}