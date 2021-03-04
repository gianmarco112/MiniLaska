#include<iostream>
#include<vector>

using std::vector;
using std::cin;
using std::cout;

void leggi_vettore(vector<int>& vet){
    int prev,cur;

    cin>>cur;
    do{
        prev=cur;
        cin>>cur;
        vet.push_back(prev);
    }while(prev!=cur);
    vet.pop_back();
    
}

vector<int> solo_pari(vector<int> vin){
   vector<int> vout;
   for(int elem:vin){
       if(elem%2==0)
        vout.push_back(elem);
   } 
   return vout;
}

int pos_massimo(vector<int> vet){
    int posmax=0;
    for(size_t i=0;i<vet.size();i++){
        if(vet.at(i)>vet.at(posmax)){
            posmax=i;
        }
    }
    return posmax;
}
bool is_vett_crescente(const vector<int>& vin){
    bool res=true;
    for(size_t i=0;i<vin.size()-1&&res;i++){
        if(vin.at(i+1)<vin.at(i))
            res= false;
    }
    return res;
}
//spostare il più piccolo all'inizio
void bubbles(vector<int>& vin){
    size_t soglia;
    for(soglia=0;soglia<vin.size();soglia++){
        //Tutti gli elementi da 0 a soglia-1 sono ordinati
        //Tutti gli elementi da soglia a size-1 sono più grandi di quelli dentro la soglia e sono disordinati
        for(size_t i=vin.size()-2;i>=soglia;i--){
            if(vin.at(i) > vin.at(i+1)){
                int supp = vin.at(i);
                vin.at(i) = vin.at(i+1);
                vin.at(i+1) =supp;
            }
        }
    }
}

void stampa_vett(const vector<int>& v){
    for(int elem:v){
        cout<<elem<<" ";
    }
    cout<<std::endl;
}


int main(){
    vector<int> v;
    leggi_vettore(v);
    if(is_vett_crescente(v))
        cout<<"si";
    bubbles(v);
    stampa_vett(v);

    return 0;
}