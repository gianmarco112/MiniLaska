#include <iostream>
#include <vector>

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::pair;
using std::max;

vector<pair<int,int>> LZ77(const string& s);

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

