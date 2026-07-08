#include "Graph/BFS.hpp"
#include "WeightedTreeBFS.hpp"

struct TreeDiameter{
    ll dir;
    pair<ll,ll> edge;
};

/*@param 木の直径を求めます。*/
template<typename T>
TreeDiameter ExecuteTreeDiameter(const T &G){
    ll ed1,ed2;
    ll maxdir=-inf,maxdir2=-inf;

    if constexpr (is_same_v<T,Graph>){

        ll N=G.size();

        vll dist1(N,inf),dist2(N,inf);
        
        BFS(0,G,dist1);

        for(int i=0;i<N;i++){
            if(chmax(maxdir,dist1[i])){
                ed1=i;
            }
        }

        BFS((ll)ed1,G,dist2);

        for(int i=0;i<N;i++){
            if(chmax(maxdir2,dist2[i])){
                ed2=i;
            }
        }

        return {maxdir2,{ed1,ed2}};
    }

    else if constexpr (is_same_v<T,WeightedGraph>){
        
        ll N=G.size();

        vll dist1(N,inf),dist2(N,inf);
        
        WeightedTreeBFS(0LL,G,dist1);

        for(int i=0;i<N;i++){
            if(chmax(maxdir,dist1[i])){
                ed1=i;
            }
        }

        WeightedTreeBFS((ll)ed1,G,dist2);

        for(int i=0;i<N;i++){
            if(chmax(maxdir2,dist2[i])){
                ed2=i;
            }
        }

        return {maxdir2,{ed1,ed2}};
    }

    else{
        static_assert(is_same_v<T,Graph>||is_same_v<T,WeightedGraph>,
            "Error:T must be Graph,EdgeGraph");
    }
}