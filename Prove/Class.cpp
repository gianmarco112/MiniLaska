#include <iostream>
#include <cmath>

class complex{
    private:
    double r;
    double i;
    
    public:
    //Costruttore
    complex():r{0.0},i{0.0}{}
    //Costruttore che inizializza in base alla chiamata
    complex(double real, double immagin):r{real},i{immagin}{}
    //Costruttore che usa modulo e angolo
    complex(double mod,double ang, bool polar);
    complex somma(const complex& a) const{
        complex res{a};
        res.r+=r;//r appartiene ad a, il complex passato come parametro
        res.i+=i;
        return res;
    }
    void pront() const{
        std::cout <<"Real part: "<< r;
        std::cout <<"Imaginary part"<< i;
    }
    void coniuga(){
        i=-i;
    }
};

complex::complex(double mod,double ang, bool polar){
        r=mod*cos(ang);
        i=mod*sin(ang);
    }

int main(){

    return 0;
}