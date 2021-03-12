#include <iostream>
using namespace std;

int T();//Returns the number of nodes in the longest path
int S();
int T(){
    //T-> (S)
    char c{};

    cin>>c; // read (
    //check that c is ( |TODO|

    //h = max num of nodes in a path on a tree in S
    int h=S();

    cin>>c; // read )

    return h+1;

}
int S(){
    
    //S -> eps | TS

    char c{};

    cin>>c; //either ( or )
    cin.putback(c);//To put back the char that i just read so T can read it as well

    int res=0;

    if(c == '('){

        int h1=T();
        int h2=S();

        res=max(h1,h2);//Calc the max of all trees in the sequence

    }//else if(c== ')') res=0; as it was initialized before the if
        
    return res;
}
int main(){

    cout<<T()-1<<endl;
    
    return 0;
}