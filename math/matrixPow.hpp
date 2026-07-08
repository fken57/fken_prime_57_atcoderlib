#include "../templete.hpp"

template<typename T>
struct MatrixPow{
    using F = function<T(T,T)>;

    F mul_operation;
    F add_operation;
    T mul_e;
    T add_e;
    vector<vector<T>> op_mat;
    vector<vector<T>> init_mat;

    /*@brief 抽象化行列累乗*/
    /*mul_op 乗法の演算定義*/
    /*add_op 加法の演算定義*/
    /*mul_e 乗法の単位元*/
    /*add_e 加法の単位元*/
    /*init_mat 初期行列*/
    /*op_mat 行列累乗するための行列*/
    MatrixPow(F mul_op,F add_op,T mul_e,T add_e,vector<vector<T>> init_mat, vector<vector<T>> op_mat):
        mul_operation(mul_op),add_operation(add_op),mul_e(mul_e),add_e(add_e),init_mat(init_mat),op_mat(op_mat){
        }

    vector<vector<T>> Matrix_add(vector<vector<T>> &a,vector<vector<T>> &b){
        int n=a.size();
        vector<vector<T>> res(n,vector<T>(n,add_e));
        rep(i,0,n)rep(j,0,n)res[i][j]=add_operation(a[i][j],b[i][j]);
        return res;
    }

    vector<vector<T>> Matrix_mul(vector<vector<T>> &a,vector<vector<T>> &b){
        int n=a.size();
        vector<vector<T>> res(n,vector<T>(n,add_e));
        rep(i,0,n)rep(j,0,n)rep(k,0,n)res[i][j]=add_operation(res[i][j],mul_operation(a[i][k],b[k][j]));
        return res;
    }

    /*行列累乗を求める*/
    /*nに数字を代入する(ll型)*/
    vector<vector<T>> Matrix_pow(ll n){
        vector<vector<T>> res=init_mat;
        vector<vector<T>> op_mat_copy=op_mat;

        int sz=init_mat.size();
        while(n){
            if(n&1) res=Matrix_mul(res,op_mat_copy);
            op_mat_copy=Matrix_mul(op_mat_copy,op_mat_copy);
            n>>=1;
        }
        return res;
    }
};

    /*@brief トロピカル代数の単位元*/
    auto mul_op=[](ll a,ll b){return a+b;};
    auto add_op=[](ll a,ll b){return min(a,b);};
    ll mul_e=0;
    ll add_e=inf;

    MatrixPow<ll> mp(mul_op,add_op,mul_e,add_e,init,C);

        /*@brief 一般代数の単位元*/
        auto mul_op=[](ll a,ll b){return a*b;};
        auto add_op=[](ll a,ll b){return a+b;};
        ll mul_e=1;
        ll add_e=0;
    
        MatrixPow<ll> mp(mul_op,add_op,mul_e,add_e,init,C);