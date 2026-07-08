#include "../templete.hpp"

void WeightedTreeBFS(int start,const WeightedGraph &G,vll &dist){
    queue<ll> q;
    for(auto v:dist) v=inf;
    dist[start]=0;
    q.push(start);

  
    while(!q.empty()){
        ll x=q.front();
        q.pop();
        for(auto v:G[x]){
            if(dist[v.to]==inf){
                dist[v.to]=dist[x]+v.weight;
                q.push(v.to);
            }
        }
    }
}