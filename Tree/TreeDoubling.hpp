#include "../templete.hpp";

struct TreeDoubling{

    Graph G;
    vector<vector<int>> parent;
    int DS=20;
    int start=0;

    vector<int> depth;

    ll N;

    /*@param TreeDoublingのコンストラクタ*/
    TreeDoubling(const Graph &g,int s=0){
        G=g;
        start=s;

        N=G.size();

        parent.assign(N, vector<int>(DS,-1));

        depth.assign(N,-1);

        if(G.size()>(1LL<<(DS-1))){
            assert(false && "Graph size exceeds res limit");
        }

        ExecuteDoubling();
    }

    /*@param ダブリングを実行する。計算量O(N log N)*/
    void ExecuteDoubling(){
        BFS(G,start);

        rep(j,1,DS){
            rep(i,0,N){
                ll x=parent[i][j-1];
                if(x==-1) continue;
                ll y=parent[x][j-1];

                parent[i][j]=y;
            }
        }
    }

    /*@param 帰りがけ順列挙を行う*/
    void BFS(const Graph &g,ll start=0){
        queue<int> q;
        q.push(start);
        depth[start]=0;

        while(!q.empty()){
            ll x=q.front();q.pop();
            for(auto v:g[x]){
                if(depth[v]!=-1) continue;
                parent[v][0]=x;
                depth[v]=depth[x]+1;
                q.push(v);
            }
        }
    }

    /*@param depth v > uとして*/
    int LCA(int v,int u){
        if(depth[v]<depth[u]) swap(v,u);

        for(int i=0;i<DS;i++){
            if((depth[v]-depth[u])>>i & 1){
                v=parent[v][i];
            }
        }

        if(u==v) return v;

        for(int i=DS-1;i>=0;i--){
            if(parent[v][i] != parent[u][i]){
                v=parent[v][i];
                u=parent[u][i];
            }
        }

        return parent[v][0];
    }

    /*u->vのk個先を求めるアルゴリズム*/
    int JumpOnTree(int u,int v,int k){
        int p=LCA(u,v);

        int udist=depth[u]-depth[p];
        int vdist=depth[v]-depth[p];

        if(udist+vdist<k){
            return -1;
        }

        int res;

        if(k <= udist){
            res=u;
            for(int i=0;i<DS;i++){
                if(k >> i & 1) res=parent[res][i];
            }

            return res;
        }
        else{
            res = v;
            for(int i=0;i<DS;i++){
                if((vdist+udist-k) >> i & 1) res=parent[res][i];
            }

            return res;
        }
    }

};