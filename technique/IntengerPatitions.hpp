#include "../templete.hpp"
//分割数の列挙(N=50くらいまでOK)
vector<vector<int>> intPartition;

void IntegerPartitions(int remain,int max_summand,vector<int> summands){
    if(remain==0){
        intPartition.push_back(summands);
    }
    else{
        for(int i=max_summand;i>0;i--){
            if(remain >= i){
                summands.push_back(i);
                IntegerPartitions(remain-i,i,summands);
                summands.pop_back();
            }
        }
    }
}