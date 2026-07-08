// library-checker-judge test case
// problem: tree/tree_diameter
// library: Tree/TreeDiameter.hpp


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


// Injecting ../templete.hpp <- _fib/Graph/BFS.hpp

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
// Injecting Graph/BFS.hpp <- _fib/Tree/TreeDiameter.hpp
// Skipping already injected ../templete.hpp

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
// Injecting WeightedTreeBFS.hpp <- _fib/Tree/TreeDiameter.hpp

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
// Injecting Tree/TreeDiameter.hpp <- _fib/Tree/TreeDiameter_tree__tree_diameter.test.cpp

// Skipping already injected Tree/TreeDiameter.hpp

void solve(){
    ll N;
    cin >> N;
    Graph G(N);
    WeightedGraph GW(N);
    rep(i,0,N-1){
        ll u,v,w;
        cin >> u >> v >> w;
        G[u].push_back(v);
        G[v].push_back(u);
        GW[u].push_back({v,w});
        GW[v].push_back({u,w});
    }

    // 両方のライブラリを呼び出してテスト（内部で inf 初期化されていればどちらも壊れません）
    TreeDiameter a = ExecuteTreeDiameter(G);
    TreeDiameter b = ExecuteTreeDiameter(GW);

    // 今回の問題（Library Checker）は重み付きの直径とパスを要求しているため、b を使用
    ll start_node = b.edge.first;
    ll end_node = b.edge.second;

    // パス復元用の探索（GW を使用）
    vll parent(N, -1);
    vll dist(N, inf);
    
    queue<ll> q;
    dist[start_node] = 0;
    q.push(start_node);

    while(!q.empty()){
        ll x = q.front();
        q.pop();
        
        if (x == end_node) break;

        for(auto& edge : GW[x]){
            if(dist[edge.to] == inf){
                dist[edge.to] = dist[x] + edge.weight;
                parent[edge.to] = x;
                q.push(edge.to);
            }
        }
    }

    // パスの復元
    vll path;
    for (ll v = end_node; v != -1; v = parent[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());

    // 出力
    cout << b.dir << "\n";
    cout << path.size();
    for (ll v : path) {
        cout << " " << v;
    }
    cout << "\n";
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
