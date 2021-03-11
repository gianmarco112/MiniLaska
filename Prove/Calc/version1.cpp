#include <iostream>

struct myerror{
    std::string msg;
};


void evaluate(){

    double lval=0, rval=0;
    char op = 0;

    std::cout << "insert expression (we accept +-*/): " << std::endl;
    std::cout << "end expression with =" << std::endl;

    std::cin >> lval >> op;

    if(!std::cin) throw myerror {"invalid operand"};
    
    while(op != '='){

        std::cin >> rval;

        if(!std::cin) throw myerror {"invalid operand"};

        switch(op){
            case '+': lval += rval; break;
            case '-': lval -= rval; break;
            case '/': lval /= rval; break;
            case '*': lval *= rval; break;
            default: throw myerror {"invalid operand"};
        }

        std::cin >> op;

    }

    std::cout << lval << std::endl;

}



int main(){

    try{

        evaluate();
        
    }catch(myerror e){

        std::cout << "error!" << e.msg << std::endl;
    
    }

}