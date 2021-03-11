#include <bits/stdc++.h>

using namespace std;
string featuredProduct(vector<string> products){
    sort(products.begin(), products.end());

    vector<pair<int,string>> freqs;

    int freq=0;
    string& prev=products[0];
    for(string& x: products){
        if(x != prev) freqs.push_back({freq,prev});
        freq=x!=prev?1:freq+1;
        prev=x;
    }
    freqs.push_back({freq,prev});
    sort(freqs.begin(),freqs.end());
    return freqs[freqs.size()-1].second;
}