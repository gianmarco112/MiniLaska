#include <iostream>

using namespace std;

/*
* E -> (double) | (+ES) | (*ES)
* S -> E | ES
*/

double S(char op);

double E(){

    char c;
    cin >> c; // read (
    cin >> c; // read a digit, v, or &

    double res {};

    if(c >= '0' and c <= '9'){

        cin.putback(c);
        cin >> res;

    }else{

        switch(c){

            case '+': 
                res = E();
                res += S('+'); 
            break;
            case '*': 
                res = E();
                res *= S('*'); 
            break;

        }

    }

    cin >> c; // read )

    return res;

}

double S(char op){

    // S -> E | ES

    double res = E();

    char c;
    cin >> c; // either ( or )
    cin.putback(c);

    if(c == '('){ // E is followed by S

        if(op == '+') res += S('+');
        else res *= S('*');

    }// else, E is not followed by S. Leave res as is

    return res;

}

int main(){

    cout << "insert arithmetic tree: " << flush;
    cout << E() << endl;

}
