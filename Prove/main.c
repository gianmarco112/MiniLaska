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
    SINGLE,TOP,BOTTOM
};
enum is_promoted{
    YES=1,
    NO=0
};
typedef struct coord{
    int x,y;
}coord_t;
struct blanks{
    struct coord coord;
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
    bool_t in_game;
}pedina_t;
/*Le pedine sono divise a metà tra i due colori, prima le BIANCHE e POI le NERE*/
struct partita{
    bool_t END_OF_PLAY;
    bool_t VICTORY_P1;
}partita_t;
/*Ad ogni turno controllo la variabile della partita per capire se è finita*/

/*function responsable of looking for movable pedine and it prints the list of available ones*/
coord_t sel_pedina(enum color colore,field_t *field){
    int i,xsol=-1,ysol=-1;
    coord_t sol;
    if(colore){
        for(i=0;i<NPEDINE/2;i++){
            if(field->pedine[i].is_movable)
                printf("%d%d \n",field->pedine[i].coord.x,field->pedine[i].coord.y);
        }
    }else{
        for(i=NPEDINE/2;i<NPEDINE;i++){
            if(field->pedine[i].is_movable)
                printf("%d%d \n",field->pedine[i].coord.x,field->pedine[i].coord.y);
        }
    }
    printf("Inserisci le coordinate della pedina");

    while(xsol==-1||ysol==-1){
        scanf("%d%d",&xsol,&ysol);
    }
    sol.x=xsol;
    sol.y=ysol;
    return sol;
}

/*prende in input il colore del giocatore, il campo di gioco e l'indice della pedina e print le possibili mosse di quella pedina*/
void possible_moves(enum color colore, field_t *field,int index){
    int i,c=0;
    coord_t sol[20];
    for(i=0;i<field->nblanks;i++){
        if(field->blanks[i].coord.y == field->pedine[index].coord.y+1) {
            if (field->blanks[i].coord.x == field->pedine[index].coord.x + 1) {
                sol[c]=field->blanks[i].coord;
                c++;
                printf("%d%d\n", field->blanks[i].coord.x, field->blanks[i].coord.y);
            }else if (field->blanks[i].coord.x == field->pedine[index].coord.x - 1) {
                sol[c]=field->blanks[i].coord;
                c++;
                printf("%d%d\n", field->blanks[i].coord.x, field->blanks[i].coord.y);
            }
        }
    }

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
    field->blanks=malloc(sizeof(struct blanks)*field->nblanks);
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
void start_game(){
    field_t field;
    field.cols=NCOLS;
    field.rows=NROWS;
    create_pedine(&field);
    field.nblanks=3;
    create_blanks(&field);
}


int main() {
    print_fieldTEST();
    return 0;
}
