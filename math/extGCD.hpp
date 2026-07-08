#include "../templete.hpp"

/*@param ax+by=gcd(x,y)を求める*/
/*@result gcd(x,y)*/
ll extGCD(ll a,ll b,ll &x,ll &y){
    ll d=a;

    if(b != 0){
        d = extGCD(b,a%b,y,x);
        y -= (a/b)*x;
    }
    else{
        x=1;
        y=0;
    }

    if(d < 0){
        d = -d;
    }

    return d;
}