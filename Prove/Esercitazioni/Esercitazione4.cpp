void duplica(Lista& l)
{   
    if(l){//Se l non è nullptr
        Lista dup=new Cella{l->info,l->next};//Creo la cella duplicata
        l->next=dup;//Sposto il puntatore della cella sulla cella duplicata
        duplica(dup->next);//Chiamo la funzione con la cella dopo quella che ho appena duplicato
    }
}
void elimina_dup(Lista& l)
{   
    Lista helper;
    if(!l||l->next==nullptr)
        return;
    else{
    
        while(l->next!=nullptr&&l->info==l->next->info){
            helper=l->next;
            l->next=l->next->next;
            delete helper;
        }
        if(l->next!=nullptr)elimina_dup(l->next);
    }
}
