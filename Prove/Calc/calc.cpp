#include <iostream>

using std::cout;
using std::endl;
using std::cin;

class Token{
public:


  Token(){
      char c;
      cin>>c;
      if (c >= '0' and c<= '9'){
          cin.putback(c);
          cin>>val;
          type='n';
      }else{
          type = c;
      }
  }; //eats the next object from cin and builds token


  bool is_numeric() const{
      return type=='n';
  }; //true if and only if it is a number
  bool is_final() const{
      return type=='=';
  }; //is it the token ‘=’?
  double get_value() const{
      return val;
  }; //to be used only if is_numeric()==true
  char get_type() const{
      return type;
  }; //to be used only if is_numeric()==false
private: // your choice

    double val{};
    char type {}; //'n' if token is a number
};

int main(){
    Token t;
    return 0;
}