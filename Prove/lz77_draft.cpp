#include <iostream>
#include <vector>

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::pair;
using std::max;

vector<pair<int,int>> LZ77(const string& s){
    vector<pair<int,int>> sol;
    //Scorro la stringa, se non ho mai trovato il carattere aggiungo il carattere al vettore
    int pos=0;
    while(pos<s.length()){
        //trovo la sottostringa più lunga che io abbia già trovato

        pair<int,int> best_match_so_far;

        for(int i=0;i<pos;i++){
        //confronto la posizione corrente con quelle precedenti a partire da 0
            int len = 0;
            while (pos+len<s.length() && s[pos+len]==s[i+len])len++;
            if(len>best_match_so_far.second) best_match_so_far={i,len};
            
        }
        if(best_match_so_far.second==0) best_match_so_far.first=s[pos];
        //incremento pos
        pos+=std::max(1,best_match_so_far.second);
        //pos+= best_match_so_far.second+(best_match_so_far.second==0);
        sol.push_back(best_match_so_far);
    }

    return sol;

};

string decompress(const vector<pair<int,int>>& C);


int main(){

    cout << "insert string: ";
    string s;
    cin >> s;

    auto compressed = LZ77(s);
    auto decompressed = decompress(compressed);

    cout << endl << "compressed string: ";

    for(auto x : compressed){
     
        cout << "("; 

        if(x.second == 0) cout << char(x.first);
        else cout << x.first;

        cout << ", " << x.second << ") ";

    }
        
    cout << endl << "decompressed string: " << decompressed << endl;

}

