// library-checker-judge test case
// problem: graph/minimum_spanning_tree
// library: Tree/MST.hpp

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


// Injecting ../templete.hpp <- _fib/Tree/DisjointUnionSet.hpp

struct DisjointUnionSet{
    //WIP
    int dsuSize;
    int TotalConective;

    struct Node{
        int joinGroup;
        int NodeSize;
        int Nodeindex;
        Node* to;
        vector<Node*> from;
        Node* leaderNode;
    };

    vector<Node> DsuList;

    DisjointUnionSet(ll N){
        dsuSize=N;
        DsuList.assign(N,{
            0,
            1,
            0,
            nullptr,
            {},
            nullptr
        });

        rep(i,0,N){
            DsuList[i].NodeSize=1;
            DsuList[i].joinGroup=i;
            DsuList[i].Nodeindex=i;
            DsuList[i].leaderNode = &DsuList[i];
        }
    }

    /*Merge O(logN)*/
    void Merge(ll u,ll v){
        if(u<0 || u>dsuSize-1 || v<0 || v>dsuSize-1){
            assert("Invalid Index");
        }

        if(u==v) return;

        ll uindex=DsuList[u].joinGroup;
        ll vindex=DsuList[v].joinGroup;

        if(uindex==vindex) return;

        ll x=DsuList[u].leaderNode -> NodeSize;
        ll y=DsuList[v].leaderNode -> NodeSize; 

        if(x < y) swap(u,v);

        //uがマージを受ける側、vがマージをする側

        auto toNode = DsuList[u].leaderNode;
        auto fromNode = DsuList[v].leaderNode;

        fromNode -> to = toNode;
        toNode -> from.push_back(fromNode);

        ll nodeCount=0;

        queue<Node*> q;
        q.push(fromNode);

        while(!q.empty()){
            auto cur = q.front();q.pop();
            nodeCount++;
            cur ->joinGroup = toNode -> joinGroup;
            cur ->leaderNode = toNode;

            for(auto next:cur -> from){
                q.push(next);
            }
        }

        toNode -> NodeSize += nodeCount;
    }

    bool Same(ll u,ll v){
        if(v<0 || v>dsuSize-1 || u<0 || u>dsuSize-1){
            assert("Invalid Index");
        }
        if(u==v){
            return true;
        }
        ll x=DsuList[v].joinGroup;
        ll y=DsuList[u].joinGroup;

        if(x==y){
            return true;
        }
        else{
            return false;
        }
    }

    int leader(ll u){
        if(u < 0 || u > dsuSize-1){
            assert("Invalid Index");
        }
        return DsuList[u].leaderNode -> Nodeindex;
    }

    vector<int> ReturnTree(ll v){
        auto nowNode= DsuList[v].leaderNode;

        queue<Node*> q;
        q.push(nowNode);

        vector<int> cnt;

        while(!q.empty()){
            auto cur = q.front();q.pop();
            int x=cur -> Nodeindex; 

            cnt.push_back(x);


            for(auto next:cur -> from){
                q.push(next);
            }
        }
        
        return cnt;
    }
};
// Injecting DisjointUnionSet.hpp <- _fib/Tree/MST.hpp

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
// Injecting Tree/MST.hpp <- _fib/Tree/MST_graph__minimum_spanning_tree.test.cpp

// Skipping already injected Tree/MST.hpp

void solve(){
    ll N,M;
    cin >> N >> M;
    vll U(M),V(M),C(M);
    rep(i,0,M){
        cin >> U[i] >> V[i] >> C[i];
    }
    auto res = MinimumSpinningTree(N,U,V,C);
    cout << res.cost << '\n';
    for(auto v:res.index) cout << v << " ";
    cout << '\n';
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
