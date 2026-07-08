// library-checker-judge test case
// problem: tree/lca
// library: Tree/TreeDoubling.hpp

//#define //_GLIBCXX_DEBUG
//#define //_GLIBCXX_DEBUG
//#define //_GLIBCXX_DEBUG
//#define //_GLIBCXX_DEBUG
//#define //_GLIBCXX_DEBUG
//#define //_GLIBCXX_DEBUG
//#define //_GLIBCXX_DEBUG
//#define //_GLIBCXX_DEBUG
//#define //_GLIBCXX_DEBUG
//#define //_GLIBCXX_DEBUG
//#define //_GLIBCXX_DEBUG
//#define //_GLIBCXX_DEBUG
//#define //_GLIBCXX_DEBUG
#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
/*long long型*/
using ll = long long;
using vll=vector<ll>;
using vvll=vector<vll>;
using vvvll=vector<vvll>;
/*ull*/
using ull = unsigned long long;
using vull=vector<ull>;
using vvull=vector<vull>;
using vvvull=vector<vvull>;
/*Graphの型*/
using Graph=vvll;
/*文字列型*/
using vch=vector<char>;
using vvch=vector<vector<char>>;
using vs=vector<string>;
/*pair型(ll,ll)*/
using Pll=pair<ll,ll>;
using vPll=vector<Pll>;
/*tuple型(ll,ll,ll)*/
using tup=tuple<ll,ll,ll>;
/*bool型*/
using bl=bool;
using vbl=vector<bool>;
using vvbl=vector<vbl>;
using vvvbl=vector<vvbl>;
/*long double*/
using ld=long double;
using vld=vector<ld>;
using vvld=vector<vld>;
using vvvld=vector<vvld>;
/*mint*/
using mint = atcoder::modint998244353;
using vmint= vector<mint>;
using vvmint = vector<vmint>;
using vvvmint = vector<vvmint>;
/*inf*/
const int infint = 1073741823;
const ll inf = 1LL << 60;
/*WeightedGraph*/
struct WeightedGraphEdge{
    ll to;
    ll weight;
};

using WeightedGraph = vector<vector<WeightedGraphEdge>>;
/*chmin,max*/
template <class T> inline bool chmax(T& a,T b){if (a<b){a=b;return 1;}return 0;}
template <class T> inline bool chmin(T& a,T b){if (a>b){a=b;return 1;}return 0;}
/*rep*/
#define rep(i,x,lim) for(ll i = (x);i < (ll)(lim);i++)
/*mod big prime*/
const ll mod1000000007=(1e+9)+7;
const ll mod998244353=998244353;
/*8方向ライブラリ(Grid対応)*/
ll dx[8]={1,-1,0,0,1,1,-1,-1};
ll dy[8]={0,0,1,-1,1,-1,1,-1};
/*2べき、10べき(64bit)*/
const vector<ull> pow2ll{1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608,16777216,33554432,67108864,134217728,268435456,536870912,1073741824,2147483648,4294967296,8589934592,17179869184,34359738368,68719476736,137438953472,274877906944,549755813888,1099511627776,2199023255552,4398046511104,8796093022208,17592186044416,35184372088832,70368744177664,140737488355328,281474976710656,562949953421312,1125899906842624,2251799813685248,4503599627370496,9007199254740992,18014398509481984,36028797018963968,72057594037927936,144115188075855872,288230376151711744,576460752303423488,1152921504606846976,2305843009213693952,4611686018427387904, 9223372036854775808ull};
const vector<ull> pow10ll{1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000,10000000000,100000000000,1000000000000,10000000000000,100000000000000,1000000000000000,10000000000000000,100000000000000000,1000000000000000000, 10000000000000000000ull};
/*inline 剰余*/
inline long long mod(long long a,long long m){
    return (a % m + m) %m;
}
/*余りが任意の時の高速で求める*/
int modpow(ll x,ll n,ll m){
    if(n==0) return 1%m;
    x=((x%m)+m)%m;
    if(n%2==0){
        ll r=modpow(x,n/2,m);
        return r*r%m;
    }
    else{
        ll r=modpow(x,n/2,m);
        return r*r%m*x%m;
    }
}


ll op(ll a,ll b){return max(a,b);}
ll opmin(ll a,ll b){return min(a,b);}
ll e(){return 0;}

/*pair unordered set*/
template<class T> size_t HashCombine(const size_t seed,const T &v){
    return seed^(std::hash<T>()(v)+0x9e3779b9+(seed<<6)+(seed>>2));
}
/* pair set*/
template<class T,class S> struct std::hash<std::pair<T,S>>{
    size_t operator()(const std::pair<T,S> &keyval) const noexcept {
        return HashCombine(std::hash<T>()(keyval.first), keyval.second);
    }
};

/*二項係数lib*/
/*@brief Cominitを呼び出して二項係数を高速で計算する(10^7程度まで)*/
const int MAX = 1010000;
mint fac[MAX], finv[MAX], inv[MAX];

void COMinit() {
    const int MOD = mint::mod();
    fac[0] = fac[1] = 1;
    finv[0] = finv[1] = 1;
    inv[1] = 1;
    for (int i = 2; i < MAX; i++){
        fac[i] = fac[i - 1] * i;
        inv[i] = MOD - inv[MOD%i] * (MOD / i);
        finv[i] = finv[i - 1] * inv[i];
    }
}


mint COM(int n, int k){
    if (n < k) return 0;
    if (n < 0 || k < 0) return 0;
    return fac[n] * finv[k] * finv[n - k];
}


// Injecting ../templete.hpp <- _fib/Tree/TreeDoubling.hpp

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
// Injecting Tree/TreeDoubling.hpp <- _fib/Tree/TreeDoubling_tree__lca.test.cpp

// Skipping already injected Tree/TreeDoubling.hpp

void solve(){
    ll N,Q;
    cin >> N >> Q;

    Graph G(N);

    rep(i,1,N){
        ll x;
        cin >> x;
        G[i].push_back(x);
        G[x].push_back(i);
    }

    TreeDoubling TD(G);

    while(Q--){
        ll u,v;
        cin >> u >> v;
        cout << TD.LCA(u,v) << '\n';
    }
}

int main(){
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);
    ll T=1;
    //cin >> T;
    while(T--){
        solve();
    }
}
