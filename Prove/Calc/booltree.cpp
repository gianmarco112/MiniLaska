#include <iostream>
using namespace std;

bool E();
bool S(char op);

bool E(){
    //E -> (t) | (f) | (vES) | (&ES)
    char c{};
    cin>>c;//read (
    cin>>c;//read t,f,v,&
    bool res{};
    switch (c)
    {
    case 't':res=true;
        break;
    case 'f':res=false;
        break;
    case 'v':res= E(); res |= S(c);
        break;
    case '&':res= E(); res &= S(c);
        break;
    }
    cin>>c; //read )

    return res;
}
bool S(char op){
    //S-> E | ES 
    bool res=E();
    char c;
    cin>>c;
    cin.putback(c);

    if(c=='('){
        bool res2=S(op);
        if(op=='v'){
            res |= res2;
        }else{//else op == '&'
            res &= res2;
        }

    }
    return res;
}
int main(){

    cout<<E()<<endl;
    return 0;
}