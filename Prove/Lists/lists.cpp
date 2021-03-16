#include <iostream>
//Lista di elementi di tipo T è
//-La lista vuota (codifichiamo con nullptr)
//-Un elemento di tipo T seguito da una lista di elementi di tipo T
struct Cella{
    int info;
    struct Cella* next;
};
typedef struct Cella* Lista;
void prepend(Lista& lista,int info){
    Lista nuovo =new Cella;//Creo nodo
    nuovo->info=info;//Copio info da inserire
    nuovo->next=lista;//Metto il puntatore alla lista successiva
    lista=nuovo;//Metto il puntatore della lista sul nuovo nodo
}
//Aggiunta in coda
void append(Lista& l, int info){
  if(!l)
    l=new Cella{info, nullptr};
  else
    append(l->next, info);
}
void stampa_lista(Lista l){
    if(l){
        std::cout<<l->info<<std::endl;
        stampa_lista(l->next);
    }
}
void distruggi_lista_iter(Lista& l){
  Lista supp;
  while(l){
    supp=l;
    l=l->next;
    delete supp;
  }
}
void distruggi_lista(Lista& l){
  if(l){
    distruggi_lista(l->next);
    delete l;
  }
}
void distruggi_lista_c(Lista *l){
  if(*l){
    distruggi_lista_c(&(*l)->next);
    delete l;
  }
}
int massimo_lista(Lista l){
  if(l->next==nullptr)
    return l->info;
  else{
    int max=massimo_lista(l->next);
    if(l->info>max)
      return l->info;
    else
      return max;
  }
}
int main(){
    Lista l=nullptr;
    append(l, 14);
    append(l,10);
    append(l, 9);
    stampa_lista(l);
    distruggi_lista(l);
    return 0;
}
