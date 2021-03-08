#include <iostream>

struct complex{
    double r;
    double i;
    complex somma(complex a){
        complex res{a};
        res.r+=r;//r appartiene ad a, il complex passato come parametro
        res.i+=i;
        return res;
    }
    void azzerati(){
        r=0.0;
        i=0.0;
    }
};
complex somma(complex a, complex b){//Posso anche fare il passaggio per reference 
    complex res{a};
    res.r+=b.r;
    res.i+=b.i;
    return res;
}
complex somma2(complex a, complex b){//devo per forza fare il passaggio per copia 
    a.r+=b.r;
    a.i+=b.i;
    return a;
}

int main(){
    /*complex a{2.0,5.0};
    complex b{.r=2.0,.i=5.0 };
    complex c{b};
    a.r=0;
    complex x,y,z;
    z=somma2(x,y);*/
    complex a{7.0,2.0}, b{1.0,-1.0};
    complex c;
    c=a.somma(b);
    c=b.somma(a);
    return 0;
}