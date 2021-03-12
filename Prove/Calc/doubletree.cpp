#include <iostream>
using namespace std;
double E();
double S(char op);

double E(){
    //E -> (double) | (/ES) | (*ES)
    char c{};
    cin>>c;//read (
    cin>>c;//read num,/,*
    double res{};
    if(c=='/'){
        res=E()/S(c);
    }else if(c=='*'){
        res=E()*S(c);
    }else{
        cin.putback(c);
        cin>>res;
    }
    
    cin>>c; //read )

    return res;
}
double S(char op){
    //S-> E | ES 
    double res=E();
    char c;
    cin>>c;
    cin.putback(c);

    if(c=='('){
        double res2=S(op);
        if(op=='/'){
            res /= res2;
        }else{//else op == '*'
            res *= res2;
        }

    }
    return res;
}
int main(){

    cout<<E()<<endl;
    return 0;
}