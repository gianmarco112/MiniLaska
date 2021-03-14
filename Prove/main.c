#include <stdio.h>
#include <stdlib.h>
#define NPEDINE (22)
#define NROWS (7)
#define NCOLS (7)

typedef struct field field_t;



typedef enum bool{
    true=1,
    false=0
}bool_t;


enum color{
    BLACK=0,
    WHITE=1
};


enum height{
    SINGLE,TOP,BOTTOM,CENTER
};


enum is_promoted{
    YES=1,NO=0
};


typedef struct coord{
    int x,y;
}coord_t;


struct blanks{
    coord_t coord;
};





typedef struct pedina{
    enum color colore;
    struct coord coord;
    enum height altezza;
    enum is_promoted promossa;
    bool_t is_movable;
    bool_t is_obbligata;
    bool_t in_game;
}pedina_t;
/*TODO   Aggiungere tutti i controlli per la variabile in_game*/

/*Le pedine sono divise a metà tra i due colori, prima le BIANCHE e POI le NERE*/
typedef struct play{
    bool_t END_OF_PLAY;
    bool_t VICTORY_P1;
}play_t;
/*Ad ogni turno controllo la variabile della partita per capire se è finita*/

struct field{
    int rows,cols;
    struct pedina *pedine;
    struct blanks *blanks;
    unsigned char nblanks;
    struct play partita;
};


/*LISTA DELLE FUNZIONI*/
void sel_pedina(enum color colore,field_t *field);
int possible_moves(enum color colore, field_t *field,int index);
void create_pedine(field_t *field);
void create_blanks(field_t *field);
void movable(enum color colore, field_t *field);
void obbligata(field_t *field,int index, enum color colore);
int coord_to_index(field_t *field, coord_t coord);
char* coord_to_pedina (field_t *field, coord_t cor);
int altezza_pedina(field_t *field, coord_t coord);
char* form_tower(field_t *field, coord_t coord);
void endgame(field_t *field);

void start_game2(field_t *field);
/*FINE LISTA DELLE FUNZIONI*/


int min(int a,int b){
    if(a<b)
        return a;
    else
        return b;
}


void endgame(field_t *field){

}

void info(field_t *field){
    int ok,xsol,ysol,index;
    coord_t sol;
    printf("Vuoi informazioni su qualche pedina? 0=no 1=si \n");
    scanf("%d",&ok);
    if(ok){
        printf("Quale posizione?\n");
        printf("Riga: ");
        scanf("%d",&ysol);
        printf("Colonna: ");
        scanf("%d",&xsol);
        sol.x=xsol;
        sol.y=ysol;
        index=coord_to_index(field,sol);
        printf("%d\n",index);
        if(field->pedine[index].colore)
            printf("WHITE\n");
        else
            printf("BLACK\n"); 
    }
}

/*Funzione che controlla se le varie pedine si possono muovere e ne modifica il parametro*/
void movable(enum color colore, field_t *field){
    int i,j,inizio,fine,col;
    /*Resetto le impostazioni di movable e obbligata a false*/
    
    /*Imposto la fine della partita a true, e successivamente se qualche pedina si può muovere la partita non è finita*/
    field->partita.END_OF_PLAY=true;
    if(colore){
            inizio=0;
            fine=NPEDINE/2;
            col=1;
    }else{
            inizio=NPEDINE/2;
            fine=NPEDINE;
            col=-1;
    }
    for(i=inizio;i<fine;i++){
        field->pedine[i].is_movable=false;
        field->pedine[i].is_obbligata=false;
        if(field->pedine[i].altezza==SINGLE||field->pedine[i].altezza==TOP){
            obbligata(field, i,colore);
            for(j=0;j<field->nblanks;j++){
                if(field->blanks[j].coord.y==field->pedine[i].coord.y+col&&(
                    field->blanks[j].coord.x==field->pedine[i].coord.x+1|| 
                    field->blanks[j].coord.x==field->pedine[i].coord.x-1)){
                    field->pedine[i].is_movable=true;
                    field->partita.END_OF_PLAY=false;
                }
            }
        }
    }
}

/*Funzione che data una pedina mi restituisce se essa è obbligata a mangiare o meno*/
void obbligata(field_t *field,int index, enum color colore){
    int i,j,inizio,fine,col;
    if(!colore){
            inizio=0;
            fine=NPEDINE/2;
            col=-1;
        }else{
            inizio=NPEDINE/2;
            fine=NPEDINE;
            col=1;
        }
    for(i=inizio;i<fine;i++){
        /*Controllo solo le pedine che posso mangiare perchè sono o il TOP della torre o pedina singola*/
        if(field->pedine[i].altezza==SINGLE||field->pedine[i].altezza==TOP){
            /*Controllo quale o quali pedina/e devo mangiare*/
            if(field->pedine[i].coord.y == field->pedine[index].coord.y+col/*Deve essere nella riga successiva*/){
                if(field->pedine[i].coord.x==field->pedine[index].coord.x+1){/*Cerco in una diagonale*/
                    /*Cerco gli spazi di destinazione della mia pedina*/
                    for(j=0;j<field->nblanks;j++){
                        if(field->blanks[j].coord.y==field->pedine[index].coord.y+col+col &&
                        (field->blanks[j].coord.x==field->pedine[index].coord.x+2)){
                        field->pedine[index].is_obbligata=true;
                        field->pedine[index].is_movable=true;
                        field->partita.END_OF_PLAY=false;
                        }
                    }
                }else if(field->pedine[i].coord.x==field->pedine[index].coord.x-1){/*Cerco nell'altra diagonale*/
                    /*Cerco gli spazi di destinazione della mia pedina*/
                    for(j=0;j<field->nblanks;j++){
                        if(field->blanks[j].coord.y==field->pedine[index].coord.y+col+col &&
                        (field->blanks[j].coord.x==field->pedine[index].coord.x-2)){
                        field->pedine[index].is_obbligata=true;
                        field->pedine[index].is_movable=true;
                        field->partita.END_OF_PLAY=false;
                        }
                    }
                }
            }
        }
    }
    
}


/*function responsable of looking for movable pedine or mangiate obbligatorie and it prints the list of available ones*/
void sel_pedina(enum color colore,field_t *field){
    int i,xsol=-1,ysol=-1, index=NPEDINE+1,indexb;
    int inizio,fine;
    bool_t control=false;
    coord_t mossa;

    if(colore){
        inizio=0;
        fine=NPEDINE/2;
    }else{
        inizio=NPEDINE/2;
        fine=NPEDINE;
    }
        /*Prima controllo se ho la possibilità di mangiare una pedina*/
        for(i=inizio;i<fine;i++){
            if(field->pedine[i].is_obbligata){
                printf("RC %d%d Obbligata\n",field->pedine[i].coord.y,field->pedine[i].coord.x);
                control=true;
            }
        }  
        /*Se non ho mosse obbligate controllo se posso muovere la pedina*/
        if(!control)
        for(i=inizio;i<fine;i++){
            if(field->pedine[i].is_movable)
                printf("RC %d%d \n",field->pedine[i].coord.y,field->pedine[i].coord.x);
        }
    
    printf("Inserisci le coordinate della pedina: \n");
        printf("Riga: ");
        scanf("%d",&ysol);
        printf("Colonna: ");
        scanf("%d",&xsol);
    
    
    /*Trovo l'indice della pedina da spostare*/
    for(i=0;i<NPEDINE;i++){
        if(!(field->pedine[i].coord.x-xsol)&&!(field->pedine[i].coord.y-ysol)){
            index=i;
            break;
        }
    }
    /*Controllo l'input*/
    if(!field->pedine[index].is_obbligata&&!field->pedine[index].is_movable){
        printf("errore nell'inserimento \n");
        sel_pedina(colore,field);
    }
    /*Procedo con la decisione della mossa*/
    if(index!=NPEDINE+1){
        indexb=possible_moves(colore, field, index);
        mossa=field->blanks[indexb].coord;
    /*Mi ritorna l'indice dello spazio bianco di destinazione dal quale estraggo le cordinate*/    
    }
    /*Se è una mossa obbligata*/
    if(field->pedine[index].is_obbligata){
        
        int indexM=0;
        int i,j=0,k=0;
        int control=0;/**/

        for(i=0;i<NPEDINE;i++){
            /*Ricavo l'indice della pedina da mangiare, 
            prendo il minore tra le cordinate e ci aggiungo uno per trovare la pedina di mezzo*/
            if(field->pedine[i].coord.y==min(field->pedine[index].coord.y,field->blanks[indexb].coord.y)+1&&
            field->pedine[i].coord.x==min(field->pedine[index].coord.x,field->blanks[indexb].coord.x)+1&&
            (field->pedine[i].altezza==TOP||field->pedine[i].altezza==TOP))
            indexM=i;
        }
        /*Sposto lo spazio*/ 
        field->blanks[indexb].coord=field->pedine[index].coord;
        /*Caso in cui la pedina mangiata sia singola*/
        if(field->pedine[indexM].altezza==SINGLE){
        /*ne creo uno in più*/
        field->blanks[field->nblanks].coord=field->pedine[indexM].coord;
        /*Incremento il numero di spazi*/
        field->nblanks++;
        /*Nel caso in cui rubo una torre libero le pedine sotto*/
        }else if(field->pedine[indexM].altezza==TOP){
			/*Cerco se esiste una pedina al centro ovvero se la torre era da tre pedine*/
            for(i=0;i<NPEDINE;i++){
                if(i!=indexM && 
                field->pedine[i].coord.x==field->pedine[indexM].coord.x&&
                field->pedine[i].coord.y==field->pedine[indexM].coord.y){
                    if(field->pedine[i].altezza==CENTER){
						field->pedine[i].altezza=TOP;
                    }
                }
            }
        }
		
        
        /*Nel caso la pedina che mangia sia una torre*/
        if(field->pedine[index].altezza==TOP){
            /*Cerco le pedine sottostanti*/
            for(i=0;i<NPEDINE;i++){
                if(field->pedine[i].coord.x==field->pedine[index].coord.x&&
                field->pedine[i].coord.y==field->pedine[index].coord.y){
                    /*Sposto le pedine che compongono la torre*/
                    field->pedine[i].coord=mossa;
                    if(field->pedine[i].altezza==CENTER){
                        control++;
                        k=i;
                    }else if(field->pedine[i].altezza==BOTTOM){
                        control+=10;
                        j=i;
                    }
                }
            }
        }
        field->pedine[index].altezza=TOP;
        field->pedine[index].coord=mossa;
        if(control==10){
            field->pedine[j].coord=mossa;
            field->pedine[j].altezza=CENTER;
        }else if(!control){
            field->pedine[indexM].altezza=BOTTOM;
        }else{
            field->pedine[indexM].in_game=false;
            field->pedine[indexM].coord.x=-1;
            field->pedine[indexM].coord.y=-1;
            field->pedine[j].coord=mossa;
            field->pedine[k].coord=mossa;
        }
        if(field->pedine[indexM].in_game)
            field->pedine[indexM].coord=mossa;
    }
    else{
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
int possible_moves(enum color colore, field_t *field,int index){
    int i,j,c=0,selezione=-1,indexb;
    /*c è l'indice dell'array di coordinate, selezione è l'indice dello stesso array che però rappresenta la selezione dell'utente*/
    int sol[20];
    /*Ho un array di coordinate delle possibili mosse che vengono proposte all'utente*/
    /*Se la pedina che devo muovere is_obbligata*/
    int col;
        if(colore){
            col=1;
        }else{
            col=-1;
        }
    if(field->pedine[index].is_obbligata){
        /*for(i=0;i<NPEDINE;i++){
            /*Controllo quale o quali pedina/e devo mangiare
            if(i!=index&&field->pedine[i].coord.y == field->pedine[index].coord.y+col&&
                field->pedine[i].colore!=field->pedine[index].colore&&
                (field->pedine[i].altezza=TOP||field->pedine[i].altezza==SINGLE)){
                if(field->pedine[i].coord.x==field->pedine[index].coord.x+1){
                    /*Cerco gli spazi di destinazione della mia pedina
                    for(j=0;j<field->nblanks;j++){
                        if(field->blanks[j].coord.y==field->pedine[index].coord.y+col+col &&
                        (field->blanks[j].coord.x==field->pedine[index].coord.x+2)){
                        sol[c]=j;
                        c++;
                        printf("%d: %d%d\n",c,field->blanks[j].coord.x,field->blanks[j].coord.y);
                        }
                    }
                }else if(field->pedine[i].coord.x==field->pedine[index].coord.x-1){
                    /*Cerco gli spazi di destinazione della mia pedina
                    for(j=0;j<field->nblanks;j++){
                        if(field->blanks[j].coord.y==field->pedine[index].coord.y+col+col &&
                        (field->blanks[j].coord.x==field->pedine[index].coord.x-2)){
                        sol[c]=j;
                        c++;
                        printf("%d: %d%d\n",c,field->blanks[j].coord.x,field->blanks[j].coord.y);
                        }
                    }
                }
            }
        }*/
	int i,j,inizio,fine,col;
    if(!colore){
            inizio=0;
            fine=NPEDINE/2;
            col=-1;
        }else{
            inizio=NPEDINE/2;
            fine=NPEDINE;
            col=1;
        }
    for(i=inizio;i<fine;i++){
        /*Controllo solo le pedine che posso mangiare perchè sono o il TOP della torre o pedina singola*/
        if(field->pedine[i].altezza==SINGLE||field->pedine[i].altezza==TOP){
            /*Controllo quale o quali pedina/e devo mangiare*/
            if(field->pedine[i].coord.y == field->pedine[index].coord.y+col/*Deve essere nella riga successiva*/){
                if(field->pedine[i].coord.x==field->pedine[index].coord.x+1){/*Cerco in una diagonale*/
                    /*Cerco gli spazi di destinazione della mia pedina*/
                    for(j=0;j<field->nblanks;j++){
                        if(field->blanks[j].coord.y==field->pedine[index].coord.y+col+col &&
                        (field->blanks[j].coord.x==field->pedine[index].coord.x+2)){
                        sol[c]=j;
                        c++;
                        printf("%d: %d%d\n",c,field->blanks[j].coord.x,field->blanks[j].coord.y);
                        }
                    }
                }else if(field->pedine[i].coord.x==field->pedine[index].coord.x-1){/*Cerco nell'altra diagonale*/
                    /*Cerco gli spazi di destinazione della mia pedina*/
                    for(j=0;j<field->nblanks;j++){
                        if(field->blanks[j].coord.y==field->pedine[index].coord.y+col+col &&
                        (field->blanks[j].coord.x==field->pedine[index].coord.x-2)){
                        sol[c]=j;
                        c++;
                        printf("%d: %d%d\n",c,field->blanks[j].coord.x,field->blanks[j].coord.y);
                        }
                    }
                }
            }
        }
    }
    
    /*Se non ho mosse obbligatorie controllo le mosse di una casella in diagonale*/
    }else{
        for(i=0;i<field->nblanks;i++){
            if(field->blanks[i].coord.y==field->pedine[index].coord.y+col&&(
                    field->blanks[i].coord.x==field->pedine[index].coord.x+1|| 
                    field->blanks[i].coord.x==field->pedine[index].coord.x-1)) {
                    sol[c]=i;
                    c++;
                    printf("%d: %d%d\n", c ,field->blanks[i].coord.y, field->blanks[i].coord.x);
                }
            }
        }
    
    if(c==0){
        printf("error \n");
    }
    printf("Dove vuoi muovere la pedina?\n(seleziona il numero corrispondente alla casella di destinazione): ");
    while(selezione==-1||selezione>c)   
        scanf("%d",&selezione);
    printf("\n");
    indexb=sol[selezione-1];
    return indexb;
}


/*Date le cordinate mi restituisce un char corrispondente alla struttura della pedina in quela casella*/
char* coord_to_pedina (field_t *field, coord_t cor){
    char *sol;
    int index=coord_to_index(field,cor);
    if(field->pedine[index].altezza==SINGLE)
        sol[1]=field->pedine[index].colore;
    else if(field->pedine[index].altezza==TOP){
        sol=form_tower(field, cor);
    }
    return sol;
}
/*Date le cordinate restituisce l'indice della pedina in quel punto, torna -1 se non ci sono pedine*/
int coord_to_index(field_t *field, coord_t coord){
    int i,index=-1;
    for(i=0;i<NPEDINE;i++){
        if(field->pedine[i].coord.y == coord.y&&
            field->pedine[i].coord.x==coord.x&&
            (field->pedine[i].altezza==SINGLE||
            field->pedine[index].altezza==TOP)){
                index=i;
                break;
            }
    }
    return index;
}
/*Date le cordinate mi restituisce l'altezza della pedina*/
int altezza_pedina(field_t *field, coord_t coord){
    int i,index=0;
    for(i=0;i<NPEDINE;i++){
        if(field->pedine[i].coord.y == coord.y&&
            field->pedine[i].coord.x==coord.x){
                index++;
                
            }
    }
    return index;
}
/*Mi restituisce un char con la forma della torre*/
char* form_tower(field_t *field, coord_t coord){
    int i;
    char sol[3];
    for(i=0;i<NPEDINE;i++){
        if(field->pedine[i].coord.y == coord.y&&
            field->pedine[i].coord.x==coord.x){
                if(field->pedine[i].altezza==TOP){
                    sol[0]=field->pedine[i].colore;
                }else if(field->pedine[i].altezza==CENTER){
                    sol[1]=field->pedine[i].colore;
                }else if(field->pedine[i].altezza==BOTTOM){
                    sol[2]=field->pedine[i].colore;
                }
            }
    }
    return sol;
}


/*se il player sbaglia pedina o vuole cambiarla*/
void ops_sel() {

}


void print_field(field_t *field){
     int i,j;
     char c=' ';
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

            
            printf(" %c |",c);
            
        }
        printf("\n");
    }
}

/*Un prototipo di come dovrebbe essere la print*/
void print_fieldTEST(){
    int i,j;
    printf("    1 2 3 4 5 6 7   \n");
    for(i=0;i<7;i++){
        /*printf("  --------------\n");*/
        if(i%2==0||i==0) {
            j = 3;
            printf("%d |",i+1);
        }else {
            j = 4;
            printf("%d   |",i+1);
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
    for(i=0;i<NPEDINE/2;i++){
        field->pedine[i].colore=WHITE;
    }
    for(i=NPEDINE/2;i<NPEDINE;i++){
        field->pedine[i].colore=BLACK;
    }
    for(i=0;i<field->rows/2;i++){
        if(i % 2 == 0)
                j = 0;
            else
                j=1;
        for(;j<7;j+=2) {
            if(c==NPEDINE/2)
                return;
            field->pedine[c].coord.y=i+1;
            field->pedine[c].coord.x=j+1;
            
            c++;
            field->pedine[NPEDINE-c].coord.y=NROWS-i;
            field->pedine[NPEDINE-c].coord.x=j+1;
            
        }
    }
}
/*Funzione che serve a creare un array con le posizioni degli spazi vuoti OK*/
void create_blanks(field_t *field){
    int c=0,k=2;
    field->blanks=malloc(sizeof(struct blanks)*NPEDINE);
    while(c<=2){
        field->blanks[c].coord.y=4;
        field->blanks[c].coord.x=k;
        k+=2;
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
    field.partita.END_OF_PLAY=false;
    field.partita.VICTORY_P1=false;
    return field;
}
void start_game2(field_t *field){
    
    field->cols=NCOLS;
    field->rows=NROWS;
    create_pedine(field);
    field->nblanks=3;
    create_blanks(field);
    field->partita.END_OF_PLAY=false;
    field->partita.VICTORY_P1=false;
    
}

void fintomain(){
    field_t field = start_game();
    while(!field.partita.END_OF_PLAY){
        print_field(&field);
        movable(WHITE,&field);
        sel_pedina(WHITE,&field);
        print_field(&field);
        movable(BLACK,&field);
        sel_pedina(BLACK,&field);
        print_field(&field);
    }
    free_pedine(&field);
    
}

int main() {
    enum color a=BLACK,b=BLACK;
    
    field_t field;
    start_game2(&field);
    /*for(i=0;i<NPEDINE;i++){
        printf("Riga %d Colonna %d \n",field.pedine[i].coord.y,field.pedine[i].coord.x);
    }*/
    
    while(!field.partita.END_OF_PLAY){
        print_field(&field);
        movable(WHITE,&field);
        sel_pedina(WHITE,&field);
        print_field(&field);
        movable(BLACK,&field);
        sel_pedina(BLACK,&field);
        /*info(&field);*/
    }
    free_pedine(&field);
    return 0;
}
