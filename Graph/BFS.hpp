#pragma once

#include "../templete.hpp"

template<typename T>
void BFS(T start,const Graph &G,vll &dist){
    for(auto v:dist) v=inf;
    queue<ll> q;
    if constexpr (is_same_v<T,vll> || is_same_v<T,vector<int>>){
        for(auto v:start){
            dist[v]=0;
            q.push(v);
        }
    }
    else if constexpr (is_same_v<T,ll> || is_same_v<T,int>){
        dist[start]=0;
        q.push(start);
    }
    else{
        static_assert(is_same_v<T,ll>||is_same_v<T,vll> || is_same_v<T,vector<int>> || is_same_v<T,int>,
            "Error:T must be ll,vll,vint,int");
    }
    while(!q.empty()){
        ll x=q.front();
        q.pop();
        for(auto v:G[x]){
            if(dist[v]==inf){
                dist[v]=dist[x]+1;
                q.push(v);
            }
        }
    }
}