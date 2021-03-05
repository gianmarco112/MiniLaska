#include <iostream>
void error(std::string s){
    std::cout<<s<<std::endl;
    exit(1);
}
int main(){
    int lval=0, rval=0;
    char op;
    std::cout<< "inser expression (+ or -):"<<std::endl;
    std::cin>>lval >> op >> rval;
    if(!std::cin){
        error("invalid operand");
    }
    if(op=='+'){
        lval+=rval;
    }else if(op=='-'){
        lval-=rval;
    }
    std::cout<< lval<<std::endl;
}