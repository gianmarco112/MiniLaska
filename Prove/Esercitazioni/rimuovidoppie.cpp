#include <iostream>

using namespace std;

void rimuovidoppie(string& s){
    int i=1, j=1;
    while(j<s.length()){
        if(s[j]!=s[j-1])
            s[i++]=s[j];
        j++;
    }
    s.resize(i);
}