#include "../templete.hpp";

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