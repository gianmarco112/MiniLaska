#include <bits/stdc++.h>

using namespace std;
int minlen(const vector<int>& lengths){
    int min_len= numeric_limits<int>::max();
    for(auto x:lengths)
        min_len=x<=0?
                min_len : 
                min(min_len,x);
    return min_len;

}
int len(const vector<int>& lenghts){
    int res=0;
    for(auto x:lenghts) res+= (x>0);
    return res;
}
vector<int> rofOffcut(vector<int> lengths){
    vector<int> res;
    while(len(lengths)>0){
        res.push_back(len(lengths));
        auto minimum = minlen(lengths);
        for(int & x:lengths) x-= minimum;
    }
    return res;
}