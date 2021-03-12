#include <stdio.h>
#include <stdlib.h>
#define NPEDINE (22)
#define NROWS (7)
#define NCOLS (7)

typedef struct field field_t;
typedef enum bool bool_t;


enum bool{
    true=1,
    false=0,
};


enum color{
    BLACK=0,
    WHITE=1
};


enum height{
    SINGLE,TOP,BOTTOM,CENTER
};


enum is_promoted{
    YES=1,
    NO=0
};


typedef struct coord{
    int x,y;
}coord_t;


struct blanks{
    coord_t coord;
};


struct field{
    int rows,cols;
    struct pedina *pedine;
    struct blanks *blanks;
    unsigned char nblanks;
};


struct pedina{
    enum color colore;
    struct coord coord;
    enum height altezza;
    enum is_promoted promossa;
    bool_t is_movable;
    bool_t is_obligata;
    bool_t in_game;
}pedina_t;
/*TODO   Aggiungere tutti i controlli per la variabile in_game*/

/*Le pedine sono divise a metà tra i due colori, prima le BIANCHE e POI le NERE*/
struct partita{
    bool_t END_OF_PLAY;
    bool_t VICTORY_P1;
}partita_t;
/*Ad ogni turno controllo la variabile della partita per capire se è finita*/




/*LISTA DELLE FUNZIONI*/
void sel_pedina(enum color colore,field_t *field);
coord_t possible_moves(enum color colore, field_t *field,int index);
void create_pedine(field_t *field);
void create_blanks(field_t *field);
void movable(enum color colore, field_t *field);
void obligata(enum color colore,field_t *field,int index);
/*FINE LISTA DELLE FUNZIONI*/

void endgame(field_t *field){

}
/*Funzione che controlla se le varie pedine si possono muovere e ne modifica il parametro*/
void movable(enum color colore, field_t *field){
    int i,j;
    for(i=0;i<NPEDINE;i++){
        field->pedine[i].is_movable=false;
        field->pedine[i].is_obligata=false;
    }
    if(colore){
        for(i=0;i<NPEDINE/2;i++){
            if(field->pedine[i].altezza==SINGLE||field->pedine[i].altezza==TOP){
                for(j=0;j<field->nblanks;j++){
                    if(field->pedine[i].coord.y+1==field->blanks[j].coord.y){ 
                        if(field->pedine[i].coord.x+1==field->blanks[j].coord.x || 
                        field->pedine[i].coord.x-1==field->blanks[j].coord.x){
                            field->pedine[i].is_movable=true;
                            obligata(colore, field, i);
                        }
                    }
                }
            }
        }
    }else{
        for(i=NPEDINE/2;i<NPEDINE;i++){
            if(field->pedine[i].altezza==SINGLE||field->pedine[i].altezza==TOP){
                for(j=0;j<field->nblanks;j++){
                    if(field->pedine[i].coord.y-1==field->blanks[j].coord.y){ 
                        if(field->pedine[i].coord.x-1==field->blanks[j].coord.x || 
                        field->pedine[i].coord.x+1==field->blanks[j].coord.x){
                            field->pedine[i].is_movable=true;
                            obligata(colore,field,i);
                        }
                    }
                }
            }
        }
    }
}

void obligata(enum color colore,field_t *field,int index){
    int i,j;
    for(i=0;i<NPEDINE;i++){
        /*Controllo solo le pedine che posso muovere*/
        if(field->pedine[i].altezza==SINGLE||field->pedine[i].altezza==TOP){
            /*Controllo quale o quali pedina/e devo mangiare*/
            if(field->pedine[i].coord.y == field->pedine[index].coord.y+1&&
            (field->pedine[i].coord.x==field->pedine[index].coord.x+1||
            field->pedine[i].coord.x==field->pedine[index].coord.x-1)){
                /*Cerco gli spazi di destinazione della mia pedina*/
                for(j=0;j<field->nblanks;j++){
                    if(field->blanks[j].coord.y==field->pedine[i].coord.y+1&&
                    (field->blanks[j].coord.x==field->pedine[i].coord.x+1||
                    field->blanks[j].coord.x==field->pedine[i].coord.x-1)){
                       field->pedine[index].is_obligata=true;
                    }
                }
            }
        }
    }
}


/*function responsable of looking for movable pedine and it prints the list of available ones*/
void sel_pedina(enum color colore,field_t *field){
    int i,xsol=-1,ysol=-1, index=NPEDINE+1,indexb;
    bool_t control=false;
    coord_t mossa;
    if(colore){
        /*Prima controllo se ho la possibilità di mangiare una pedina*/
        for(i=0;i<NPEDINE/2;i++){
            if(field->pedine[i].is_obligata){
                printf("%d%d \n",field->pedine[i].coord.x,field->pedine[i].coord.y);
                control=true;
            }
        }  
        /*Se non ho mosse obbligate controllo se posso muovere la pedina*/
        if(!control)
        for(i=0;i<NPEDINE/2;i++){
            if(field->pedine[i].is_movable)
                printf("%d%d \n",field->pedine[i].coord.x,field->pedine[i].coord.y);
        }
    }else{
        /*Prima controllo se ho la possibilità di mangiare una pedina*/
        for(i=NPEDINE/2;i<NPEDINE;i++){
            if(field->pedine[i].is_obligata){
                printf("%d%d \n",field->pedine[i].coord.x,field->pedine[i].coord.y);
                control=true;
            }
        }
        /*Se non ho mosse obbligate controllo se posso muovere la pedina*/
        if(!control)
        for(i=NPEDINE/2;i<NPEDINE;i++){
            if(field->pedine[i].is_movable)
                printf("%d%d \n",field->pedine[i].coord.x,field->pedine[i].coord.y);
        }
    }
    printf("Inserisci le coordinate della pedina");

    while((xsol==-1||ysol==-1)){
        scanf("%d%d",&xsol,&ysol);
    }
    /*Trovo l'indice della pedina da spostare*/
    for(i=0;i<NPEDINE;i++){
        if(field->pedine[i].coord.x==xsol&&field->pedine[i].coord.y==ysol){
            index=i;
            break;
        }
    }
    /*Controllo l'input*/
    if(!field->pedine[index].is_obligata||!field->pedine[index].is_movable){
        printf("errore nell'inserimento");
        return sel_pedina(colore,field);
    }
    /*Procedo con la decisione della mossa*/
    if(index!=NPEDINE+1)
    mossa=possible_moves(colore, field, index);
    /*Trovo l'indice dello spazio da spostare*/
        for(i=0;i<field->nblanks;i++){
            if(field->blanks[i].coord.x==mossa.x&&field->blanks[i].coord.y==mossa.y){
                indexb=i;
                break;
            }
        }

    if(field->pedine[index].is_obligata){
        /*Sposto lo spazio nella posizione della mia pedina, ne creo uno su quella mangiata, sposto le due pedine, assegno le altezze*/
        int indexM=index;/*CAMBIARE INDEX CON LA POSIZIONE DELLA PEDINA MANGIATA*/
        int i,j=0;
        int control=0;/**/
        /*Sposto lo spazio e ne creo uno in più*/
        field->blanks[indexb].coord=field->pedine[index].coord;
        field->blanks[field->nblanks].coord=field->pedine[indexM].coord;
        field->nblanks++;
        if(field->pedine[index].altezza!=SINGLE){
            for(i=0;i<NPEDINE;i++){
                if(field->pedine[i].coord.x==field->pedine[index].coord.x&&
                field->pedine[i].coord.y==field->pedine[index].coord.y){
                    field->pedine[i].coord=mossa;
                    if(field->pedine[i].altezza==CENTER)
                        control++;
                    else if(field->pedine[i].altezza==BOTTOM){
                        control+=10;
                        j+=i;
                    }
                }
            }
        }
        field->pedine[index].altezza=TOP;
        if(control==10){
            field->pedine[j].altezza=CENTER;
        }else if(!control){
            field->pedine[indexM].coord=mossa;
            field->pedine[indexM].altezza=BOTTOM;
        }else{
            field->pedine[indexM].in_game=false;
            field->pedine[indexM].coord.x=-1;
            field->pedine[indexM].coord.y=-1;
        }
    }else{
        /*Scambio la pedina e lo spazio*/
        
        field->blanks[indexb].coord=field->pedine[index].coord;
        if(field->pedine[index].altezza==SINGLE){
            field->pedine[index].coord=mossa;
        }else{/*Assumo che sia TOP*/
            for(i=0;i<NPEDINE;i++){
                if(field->pedine[i].coord.x==field->pedine[index].coord.x&&
                field->pedine[i].coord.y==field->pedine[index].coord.y){
                    field->pedine[index].coord=mossa;
                }
            }
        }
    }
}


/*Viene chiamata dalla funzione sel_pedina*/
/*prende in input il colore del giocatore, il campo di gioco e l'indice della pedina da muovere e print le possibili mosse di quella pedina*/
coord_t possible_moves(enum color colore, field_t *field,int index){
    int i,j,c=0,selezione=-1;
    /*c è l'indice dell'array di coordinate, selezione è l'indice dello stesso array che però rappresenta la selezione dell'utente*/
    coord_t sol[20];
    /*Ho un array di coordinate delle possibili mosse che vengono proposte all'utente*/
    /*Se la pedina che devo muovere is_obligata*/
    if(field->pedine[index].is_obligata){
        for(i=0;i<NPEDINE;i++){
            /*Controllo quale o quali pedina/e devo mangiare*/
            if(field->pedine[i].coord.y == field->pedine[index].coord.y+1&&
            (field->pedine[i].coord.x==field->pedine[index].coord.x+1||
            field->pedine[i].coord.x==field->pedine[index].coord.x-1)){
                /*Cerco gli spazi di destinazione della mia pedina*/
                for(j=0;j<field->nblanks;j++){
                    if(field->blanks[j].coord.y==field->pedine[i].coord.y+1&&(
                        field->blanks[j].coord.x==field->pedine[i].coord.x+1||
                        field->blanks[j].coord.x==field->pedine[i].coord.x-1)){
                        sol[c]=field->blanks[j].coord;
                        c++;
                        printf("%d: %d%d",c,sol[c-1].x,sol[c-1].y);
                    }
                }
            }
        }
    
    /*Se non ho mosse obbligatorie controllo le mosse di una casella in diagonale*/
    }else{
        for(i=0;i<field->nblanks;i++){
            if(field->blanks[i].coord.y == field->pedine[index].coord.y+1) {
                if (field->blanks[i].coord.x == field->pedine[index].coord.x + 1) {
                    sol[c]=field->blanks[i].coord;
                    c++;
                    printf("%d: %d%d\n", c ,field->blanks[i].coord.x, field->blanks[i].coord.y);
                }else if (field->blanks[i].coord.x == field->pedine[index].coord.x - 1) {
                    sol[c]=field->blanks[i].coord;
                    c++;
                    printf("%d: %d%d\n", c,field->blanks[i].coord.x, field->blanks[i].coord.y);
                }
            }
        }
    }
    printf("Dove vuoi muovere la pedina?(seleziona il numero corrispondente alla casella di destinazione)");
    while(selezione==-1)   
        scanf("%d",&selezione);
    return sol[selezione];
}

/*se il player sbaglia pedina o vuole cambiarla*/
void ops_sel() {

}

void print_field(field_t *field){
}

/*Un prototipo di come dovrebbe essere la print*/
void print_fieldTEST(){
    int i,j;
    printf("    1 2 3 4 5 6 7   \n");
    for(i=0;i<7;i++){
        /*printf("  --------------\n");*/
        if(i%2==0||i==0) {
            j = 3;
            printf("%d |",i);
        }else {
            j = 4;
            printf("%d   |",i);
        }
        for(;j<7;j++){

            if(j%2==0)
            printf(" w |");
            else
            printf(" b |");
        }
        printf("\n");
    }
}
/*Funzione usata all'inizio del game per creare le pedine dei due giocatori, accetta in input un field*/
void create_pedine(field_t *field){
    int i,j,c=0;
    field->pedine=malloc(sizeof(struct pedina)*NPEDINE);

    for(i=0;i<NPEDINE;i++){
        field->pedine[i].altezza=SINGLE;
        field->pedine[i].in_game=true;
        field->pedine[i].promossa=NO;
    }
    for(i=0;i<1+NPEDINE/2;i++){
        field->pedine[i].colore=WHITE;
    }
    for(i=1+NPEDINE/2;i<NPEDINE;i++){
        field->pedine[i].colore=BLACK;
    }
    for(i=0;i<field->rows;i++){
        for(j=0;j<field->cols;j++) {
            if(i % 2 == 0)
                j = 3;
            else
                j = 4;
            if(c==NPEDINE)
                return;
            field->pedine[c].coord.x=i;
            field->pedine[c].coord.y=j;
            c++;
            field->pedine[NPEDINE-c].coord.x=NPEDINE-i;
            field->pedine[NPEDINE-c].coord.y=NPEDINE-j;
            c++;
        }
    }
}
/*Funzione che serve a creare un array con le posizioni degli spazi vuoti*/
void create_blanks(field_t *field){
    int c=0;
    field->blanks=malloc(sizeof(struct blanks)*NPEDINE);
    while(c!=2){
        field->blanks[c].coord.x=4;
        field->blanks[c].coord.y=c+1;
        c++;
    }
}
/*funzione da eseguire sempre prima di chiudere il programma*/
void free_pedine(field_t *field){
    free(field->pedine);
    free(field->blanks);
}
/*funzione di inizializzazione*/
field_t start_game(){
    field_t field;
    field.cols=NCOLS;
    field.rows=NROWS;
    create_pedine(&field);
    field.nblanks=3;
    create_blanks(&field);
    return field;
}


int main() {
    print_fieldTEST();
    return 0;
}
