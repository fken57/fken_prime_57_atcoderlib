#include "DisjointUnionSet.hpp"

struct MSTData{
    ll cost;
    WeightedGraph graph;
    vll index;
}; 
/*@param 連結な0index-ed辺リストに関して最小全域木を返す*/
MSTData MinimumSpinningTree(ll N,vll V,vll U,vll W){
    if(V.size()!=U.size()||U.size()!=W.size()){
        assert("Invalid Input");
    }

    ll M=V.size();

    DisjointUnionSet Tr(N);

    vector<tuple<ll,ll,ll,ll>> Tup(M);;
    rep(i,0,M) Tup[i]={W[i],V[i],U[i],i};

    sort(Tup.begin(),Tup.end());

    WeightedGraph G(N);

    vll res;

    ll mincost=0;

    rep(i,0,M){
        ll cost=get<0>(Tup[i]);
        ll from=get<1>(Tup[i]);    
        ll to=get<2>(Tup[i]);
        ll ind=get<3>(Tup[i]);

        if(Tr.Same(from,to)){
            continue;
        }

        Tr.Merge(from,to);
        mincost += cost;
        G[from].push_back({to,cost});
        G[to].push_back({from,cost});

        res.push_back(ind);
    }

    return {mincost,G,res};
}