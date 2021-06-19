#include <stdio.h>
#include <stdlib.h>
#define NPEDINE (22)
#define NROWS (7)
#define NCOLS (7)

typedef struct field field_t;

typedef enum bool{
    TRUE = 1,
    FALSE = 0
}bool_t;


enum color{
    BLACK = 0,
    WHITE = 1
};


enum height{
    SINGLE, TOP, BOTTOM, CENTER
};


enum is_promoted{
    YES = 1, NO = 0
};


typedef struct coord{
    int x, y;
}coord_t;

typedef struct pair_int{
    int score;
    int index;
    int indexb;
}pair_t;
typedef struct vect{
    int* v;
    int size;
    bool_t obbligata;
}vect;
typedef struct blanks{
    coord_t coord;
}blanks_t;

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
    int rows, cols;
    struct pedina* pedine;
    struct blanks* blanks;
    unsigned char nblanks;
    struct play partita;
};
/*LISTA DELLE FUNZIONI*/
void sel_pedina(enum color colore, field_t* field);
int possible_moves(enum color colore, field_t* field, int index);
void create_pedine(field_t* field);
void create_blanks(field_t* field);
void movable(enum color colore, field_t* field);
void obbligata(field_t* field, int index, enum color colore);
void promossa(field_t* field, int index);
void spostamento_pedine(field_t* field, enum color colore, int index, int indexb);
int coord_to_index(field_t* field, coord_t coord);
char* coord_to_pedina(field_t* field, coord_t cor);
int altezza_pedina(field_t* field, coord_t coord);
pair_t cpu_turn(field_t* field);
pair_t trun_cpu(field_t* field);
pair_t cpu_pedina(field_t field, int depth, enum color colore);
pair_t cpu_mossa(field_t field, int index, int depth, enum color colore);
void pedina_player(field_t* field, enum color colore);
void sel_pedina2(enum color colore, field_t* field, vect* soluzione);
pair_t mossa_cpu(field_t field, enum color colore, int index, int depth);
pair_t pedina_cpu(field_t field, enum color colore, int depth);
int n_promosse(field_t* field, enum color colore);
int n_pedine(field_t* field, enum color colore);
int mossa_player(field_t* field, enum color colore, int index);
void possible_moves2(enum color colore, field_t* field, int index, vect* soluzione);
void stampa_field(field_t* field);
void coord_to_char(field_t* field, int x, int y, char* ped);
void endgame(field_t* field);
void start_game(field_t* field);
/*FINE LISTA DELLE FUNZIONI*/

pair_t cpu_mossa(field_t field, int index, int depth, enum color colore){
    pair_t res;
    pair_t sol[NPEDINE];
    pair_t retval;
    int i, z, countersol = 0, npedine1, npedineAvv1, npedine2, npedineAvv2, npromosse1, npromosse2, npromosseAvv1, npromosseAvv2, posy, inizio, fine;
    pedina_t* copiapedine = (pedina_t*) malloc(sizeof(pedina_t)*NPEDINE);
    blanks_t* copiablanks = (blanks_t*) malloc(sizeof(blanks_t)*field.nblanks);
    int nblanks = field.nblanks;
    res.score = 0;
    /* printf("\n");
    printf("%d",depth);
    printf("\n"); */
    if (depth==0){
        res.score = 0;
        res.indexb = 0;
        return res;
    }
    for (i = 0;i<NPEDINE;i++){
        copiapedine[i] = field.pedine[i];
        if (i<field.nblanks)
            copiablanks[i] = field.blanks[i];
    }
    npedine1 = n_pedine(&field, BLACK);
    npedineAvv1 = n_pedine(&field, WHITE);
    npromosse1 = n_promosse(&field, BLACK);
    npromosseAvv1 = n_promosse(&field, WHITE);
    if (!colore){
        inizio = 0;
        fine = NPEDINE/2;
        posy = -1;
    } else{
        inizio = NPEDINE/2;
        fine = NPEDINE;
        posy = 1;
    }
    if (field.pedine[index].is_obbligata){
        int j;


        for (i = inizio;i<fine;i++){
            /*Controllo solo le pedine che posso mangiare perchè sono o il TOP della torre o pedina singola*/
            if ((field.pedine[i].altezza==SINGLE||field.pedine[i].altezza==TOP)&&field.pedine[i].colore!=field.pedine[index].colore){
                /*Controllo quale o quali pedina/e devo mangiare*/
                if (field.pedine[i].coord.y==field.pedine[index].coord.y+posy/*Deve essere nella riga successiva*/){
                    if (field.pedine[i].coord.x==field.pedine[index].coord.x+1){/*Cerco in una diagonale*/
                        /*Cerco gli spazi di destinazione della mia pedina*/
                        for (j = 0;j<field.nblanks;j++){
                            if (field.blanks[j].coord.y==field.pedine[index].coord.y+posy+posy&&
                            (field.blanks[j].coord.x==field.pedine[index].coord.x+2)){
                            /*TODO                        */
                                enum color turno;
                                spostamento_pedine(&field, colore, index, j);/*Una delle possibili mosse*/
                                if (!colore)turno = WHITE;
                                else turno = BLACK;
                                /* printf("\n");
                                printf(" mossa %d \n",turno);
                                printf("\n"); */
                                res = cpu_pedina(field, depth-1, turno);
                                sol[countersol].index = index;
                                sol[countersol].score = res.score;
                                sol[countersol].indexb = j;
                                npedine2 = n_pedine(&field, BLACK);
                                npedineAvv2 = n_pedine(&field, WHITE);
                                npromosse2 = n_promosse(&field, BLACK);
                                npromosseAvv2 = n_promosse(&field, WHITE);
                                /*Assegnamento del punteggio*/
                                if (npedine1<npedine2)sol[countersol].score++;
                                else if (npedine1>npedine2)sol[countersol].score--;
                                if (npedineAvv1>npedineAvv2)sol[countersol].score++;
                                else if (npedineAvv1<npedineAvv2)sol[countersol].score--;
                                if (npromosse1<npromosse2)sol[countersol].score++;
                                else if (npromosse1>npromosse2)sol[countersol].score--;
                                if (npromosseAvv1>npromosseAvv2)sol[countersol].score++;
                                else if (npromosseAvv1<npromosseAvv2)sol[countersol].score--;
                                countersol++;
                                field.nblanks = nblanks;
                                for (z = 0;z<NPEDINE;z++){
                                    field.pedine[z] = copiapedine[z];
                                    if (z<field.nblanks)
                                        field.blanks[z] = copiablanks[z];
                                }

                            }
                        }
                    } else if (field.pedine[i].coord.x==field.pedine[index].coord.x-1){/*Cerco nell'altra diagonale*/
                        /*Cerco gli spazi di destinazione della mia pedina*/
                        for (j = 0;j<field.nblanks;j++){
                            if (field.blanks[j].coord.y==field.pedine[index].coord.y+posy+posy&&
                            (field.blanks[j].coord.x==field.pedine[index].coord.x-2)){
                            /*TODO              */
                                enum color turno;

                                spostamento_pedine(&field, colore, index, j);/*Una delle possibili mosse*/
                                if (!colore)turno = WHITE;
                                else turno = BLACK;
                                /* printf("\n");
                                printf(" mossa %d \n",turno);
                                printf("\n"); */
                                res = cpu_pedina(field, depth-1, turno);
                                sol[countersol].index = index;
                                sol[countersol].score = res.score;
                                sol[countersol].indexb = j;
                                npedine2 = n_pedine(&field, BLACK);
                                npedineAvv2 = n_pedine(&field, WHITE);
                                npromosse2 = n_promosse(&field, BLACK);
                                npromosseAvv2 = n_promosse(&field, WHITE);
                                /*Assegnamento del punteggio*/
                                if (npedine1<npedine2)sol[countersol].score++;
                                else if (npedine1>npedine2)sol[countersol].score--;
                                if (npedineAvv1>npedineAvv2)sol[countersol].score++;
                                else if (npedineAvv1<npedineAvv2)sol[countersol].score--;
                                if (npromosse1<npromosse2)sol[countersol].score++;
                                else if (npromosse1>npromosse2)sol[countersol].score--;
                                if (npromosseAvv1>npromosseAvv2)sol[countersol].score++;
                                else if (npromosseAvv1<npromosseAvv2)sol[countersol].score--;
                                countersol++;
                                field.nblanks = nblanks;
                                for (z = 0;z<NPEDINE;z++){
                                    field.pedine[z] = copiapedine[z];
                                    if (z<field.nblanks)
                                        field.blanks[z] = copiablanks[z];
                                }

                            }
                        }
                    }
                } else if (field.pedine[index].promossa){
                    if (field.pedine[i].coord.y==field.pedine[index].coord.y-posy/*Deve essere nella riga successiva*/){
                        if (field.pedine[i].coord.x==field.pedine[index].coord.x+1){/*Cerco in una diagonale*/
                            /*Cerco gli spazi di destinazione della mia pedina*/
                            for (j = 0;j<field.nblanks;j++){
                                if (field.blanks[j].coord.y==field.pedine[index].coord.y-posy-posy&&
                                (field.blanks[j].coord.x==field.pedine[index].coord.x+2)){
                                /*TODO                        */
                                    enum color turno;
                                    spostamento_pedine(&field, colore, index, j);/*Una delle possibili mosse*/
                                    if (!colore)turno = WHITE;
                                    else turno = BLACK;
                                    /* printf("\n");
                                    printf(" mossa %d \n",turno);
                                    printf("\n"); */
                                    res = cpu_pedina(field, depth-1, turno);
                                    sol[countersol].index = index;
                                    sol[countersol].score = res.score;
                                    sol[countersol].indexb = j;
                                    npedine2 = n_pedine(&field, BLACK);
                                    npedineAvv2 = n_pedine(&field, WHITE);
                                    npromosse2 = n_promosse(&field, BLACK);
                                    npromosseAvv2 = n_promosse(&field, WHITE);
                                    /*Assegnamento del punteggio*/
                                    if (npedine1<npedine2)sol[countersol].score++;
                                    else if (npedine1>npedine2)sol[countersol].score--;
                                    if (npedineAvv1>npedineAvv2)sol[countersol].score++;
                                    else if (npedineAvv1<npedineAvv2)sol[countersol].score--;
                                    if (npromosse1<npromosse2)sol[countersol].score++;
                                    else if (npromosse1>npromosse2)sol[countersol].score--;
                                    if (npromosseAvv1>npromosseAvv2)sol[countersol].score++;
                                    else if (npromosseAvv1<npromosseAvv2)sol[countersol].score--;
                                    countersol++;
                                    field.nblanks = nblanks;
                                    for (z = 0;z<NPEDINE;z++){
                                        field.pedine[z] = copiapedine[z];
                                        if (z<field.nblanks)
                                            field.blanks[z] = copiablanks[z];
                                    }

                                }
                            }
                        } else if (field.pedine[i].coord.x==field.pedine[index].coord.x-1){/*Cerco nell'altra diagonale*/
                            /*Cerco gli spazi di destinazione della mia pedina*/
                            for (j = 0;j<field.nblanks;j++){
                                if (field.blanks[j].coord.y==field.pedine[index].coord.y-posy-posy&&
                                (field.blanks[j].coord.x==field.pedine[index].coord.x-2)){
                                /*TODO              */
                                    enum color turno;

                                    spostamento_pedine(&field, colore, index, j);/*Una delle possibili mosse*/
                                    if (!colore)turno = WHITE;
                                    else turno = BLACK;
                                    /* printf("\n");
                                    printf(" mossa %d \n",turno);
                                    printf("\n"); */
                                    res = cpu_pedina(field, depth-1, turno);
                                    sol[countersol].index = index;
                                    sol[countersol].score = res.score;
                                    sol[countersol].indexb = j;
                                    npedine2 = n_pedine(&field, BLACK);
                                    npedineAvv2 = n_pedine(&field, WHITE);
                                    npromosse2 = n_promosse(&field, BLACK);
                                    npromosseAvv2 = n_promosse(&field, WHITE);
                                    /*Assegnamento del punteggio*/
                                    if (npedine1<npedine2)sol[countersol].score++;
                                    else if (npedine1>npedine2)sol[countersol].score--;
                                    if (npedineAvv1>npedineAvv2)sol[countersol].score++;
                                    else if (npedineAvv1<npedineAvv2)sol[countersol].score--;
                                    if (npromosse1<npromosse2)sol[countersol].score++;
                                    else if (npromosse1>npromosse2)sol[countersol].score--;
                                    if (npromosseAvv1>npromosseAvv2)sol[countersol].score++;
                                    else if (npromosseAvv1<npromosseAvv2)sol[countersol].score--;
                                    countersol++;
                                    field.nblanks = nblanks;
                                    for (z = 0;z<NPEDINE;z++){
                                        field.pedine[z] = copiapedine[z];
                                        if (z<field.nblanks)
                                            field.blanks[z] = copiablanks[z];
                                    }

                                }
                            }
                        }
                    }
                }
            }
        }

        /*Se non ho mosse obbligatorie controllo le mosse di una casella in diagonale*/
    } else{
        for (i = 0;i<field.nblanks;i++){
            if (field.blanks[i].coord.y==field.pedine[index].coord.y+posy&&(
                field.blanks[i].coord.x==field.pedine[index].coord.x+1||
                field.blanks[i].coord.x==field.pedine[index].coord.x-1)){
                /*TODO                        */
                enum color turno;
                spostamento_pedine(&field, colore, index, i);/*Una delle possibili mosse*/
                if (!colore)turno = WHITE;
                else turno = BLACK;
                /* printf("\n");
                printf(" mossa %d \n",turno);
                printf("\n"); */
                res = cpu_pedina(field, depth-1, turno);
                sol[countersol].index = index;
                sol[countersol].score = res.score;
                sol[countersol].indexb = i;
                npedine2 = n_pedine(&field, BLACK);
                npedineAvv2 = n_pedine(&field, WHITE);
                npromosse2 = n_promosse(&field, BLACK);
                npromosseAvv2 = n_promosse(&field, WHITE);
                    /*Assegnamento del punteggio*/
                if (npedine1<npedine2)sol[countersol].score++;
                else if (npedine1>npedine2)sol[countersol].score--;
                if (npedineAvv1>npedineAvv2)sol[countersol].score++;
                else if (npedineAvv1<npedineAvv2)sol[countersol].score--;
                if (npromosse1<npromosse2)sol[countersol].score++;
                else if (npromosse1>npromosse2)sol[countersol].score--;
                if (npromosseAvv1>npromosseAvv2)sol[countersol].score++;
                else if (npromosseAvv1<npromosseAvv2)sol[countersol].score--;
                countersol++;
                field.nblanks = nblanks;
                for (z = 0;z<NPEDINE;z++){
                    field.pedine[z] = copiapedine[z];
                    if (z<field.nblanks)
                        field.blanks[z] = copiablanks[z];
                }

            } else if (field.pedine[index].promossa){
                if (field.blanks[i].coord.y==field.pedine[index].coord.y-posy&&(
                    field.blanks[i].coord.x==field.pedine[index].coord.x+1||
                    field.blanks[i].coord.x==field.pedine[index].coord.x-1)){
                    /*TODO                        */
                    enum color turno;
                    spostamento_pedine(&field, colore, index, i);/*Una delle possibili mosse*/
                    if (!colore)turno = WHITE;
                    else turno = BLACK;
                    /* printf("\n");
                    printf(" mossa %d \n",turno);
                    printf("\n"); */
                    res = cpu_pedina(field, depth-1, turno);
                    sol[countersol].index = index;
                    sol[countersol].score = res.score;
                    sol[countersol].indexb = i;
                    npedine2 = n_pedine(&field, BLACK);
                    npedineAvv2 = n_pedine(&field, WHITE);
                    npromosse2 = n_promosse(&field, BLACK);
                    npromosseAvv2 = n_promosse(&field, WHITE);
                        /*Assegnamento del punteggio*/
                    if (npedine1<npedine2)sol[countersol].score++;
                    else if (npedine1>npedine2)sol[countersol].score--;
                    if (npedineAvv1>npedineAvv2)sol[countersol].score++;
                    else if (npedineAvv1<npedineAvv2)sol[countersol].score--;
                    if (npromosse1<npromosse2)sol[countersol].score++;
                    else if (npromosse1>npromosse2)sol[countersol].score--;
                    if (npromosseAvv1>npromosseAvv2)sol[countersol].score++;
                    else if (npromosseAvv1<npromosseAvv2)sol[countersol].score--;
                    countersol++;
                    field.nblanks = nblanks;
                    for (z = 0;z<NPEDINE;z++){
                        field.pedine[z] = copiapedine[z];
                        if (z<field.nblanks)
                            field.blanks[z] = copiablanks[z];
                    }

                }
            }
        }
    }
    /* sol[countersol].index=index;
    sol[countersol].score=res.score;
    sol[countersol].indexb=res.indexb;
    npedine2=n_pedine(&field,BLACK);
    npedineAvv2=n_pedine(&field,WHITE);
    npromosse2=n_promosse(&field,BLACK);
    npromosseAvv2=n_promosse(&field,WHITE);
    Assegnamento del punteggio
    if(npedine1<npedine2)sol[countersol].score++;
    else if(npedine1>npedine2)sol[countersol].score--;
    if(npedineAvv1>npedineAvv2)sol[countersol].score++;
    else if(npedineAvv1<npedineAvv2)sol[countersol].score--;
    if(npromosse1<npromosse2)sol[countersol].score++;
    else if(npromosse1>npromosse2)sol[countersol].score--;
    if(npromosseAvv1>npromosseAvv2)sol[countersol].score++;
    else if(npromosseAvv1< npromosseAvv2)sol[countersol].score--; */
    if (countersol==0)printf("Errore countersol\n");
    retval = sol[0];
    while (countersol>0){
        if (retval.score<sol[countersol].score)
            retval = sol[countersol];
        countersol--;
    }
    free(copiablanks);
    free(copiapedine);
    return retval;
}
/**
 * @brief Date le cordinate mi restituisce l'altezza della pedina
 *
 * @param field
 * @param coord
 * @return int
 */
int altezza_pedina(field_t* field, coord_t coord){
    int i, index = 0;
    for (i = 0;i<NPEDINE;i++){
        if (field->pedine[i].coord.y==coord.y&&
            field->pedine[i].coord.x==coord.x){
            index++;

        }
    }
    return index;
}
/**
 * @brief
 *
 * @param field
 * @param depth
 * @param colore
 * @return pair_t
 */
pair_t cpu_pedina(field_t field, int depth, enum color colore){

    int i, k = 0;
    int inizio, fine, massimo = 0, indicemassimo = 0;
    /* stampa_field(&field);
    printf("\n");
    printf("%d",depth);
    printf("\n"); */
    bool_t control = FALSE;
    pair_t retval;
    pair_t* max = malloc(sizeof(pair_t)*NPEDINE);
    if (depth==0){
        pair_t res;
        res.score = 0;
        res.indexb = -1;
        res.index = -1;
        return res;
    }
    if (!colore){/*Colore nero*/
        inizio = NPEDINE/2;
        fine = NPEDINE;
    } else{
        inizio = 0;
        fine = NPEDINE/2;
    }
    movable(colore, &field);
    /*Prima controllo se ho la possibilità di mangiare una pedina*/
    for (i = inizio;i<fine;i++){
        if (field.pedine[i].is_obbligata){
            max[k] = cpu_mossa(field, i, depth, colore);
            max[k].index = i;
            k++;
            control = TRUE;
        }
    }
    /*Se non ho mosse obbligate controllo se posso muovere la pedina*/
    if (!control){
        for (i = inizio;i<fine;i++){
            if (field.pedine[i].is_movable){
                max[k] = cpu_mossa(field, i, depth, colore);
                max[k].index = i;
                k++;
            }
        }
    }
    if (k==0){
        pair_t res;
        printf("Errore max\n");
        res.score = -3;
        res.indexb = -1;
        res.index = -1;
        return res;

    }
    massimo = max[0].score;
    for (i = 0;i<k;i++){
        if (max[i].score>massimo){
            massimo = max[i].score;
            indicemassimo = i;
        }
    }
    retval = max[indicemassimo];
    return retval;


}
pair_t cpu_turn(field_t* field){
    field_t  campo = *field;

    pedina_t* copiapedine = malloc(sizeof(pedina_t)*NPEDINE);
    blanks_t* copiablanks = malloc(sizeof(blanks_t)*field->nblanks);
    int i, z;
    pair_t sol;
    for (i = 0;i<NPEDINE;i++){
        copiapedine[i] = field->pedine[i];
        if (i<field->nblanks)
            copiablanks[i] = field->blanks[i];
    }
    sol = cpu_pedina(campo, 10, BLACK);
    for (z = 0;z<NPEDINE;z++){
        field->pedine[z] = copiapedine[z];
        if (z<field->nblanks)
            field->blanks[z] = copiablanks[z];
    }

    return sol;
}
int possible_moves(enum color colore, field_t* field, int index){
    int i, c = 0, selezione = -1, indexb;
    /*c è l'indice dell'array di coordinate, selezione è l'indice dello stesso array che però rappresenta la selezione dell'utente*/
    int sol[20];
    /*Ho un array di coordinate delle possibili mosse che vengono proposte all'utente*/

    int col;
    if (colore){
        col = 1;
    } else{
        col = -1;
    }
/*Se la pedina che devo muovere is_obbligata*/
    if (field->pedine[index].is_obbligata){
        int i, j, inizio, fine;
        if (!colore){
            inizio = 0;
            fine = NPEDINE/2;
        } else{
            inizio = NPEDINE/2;
            fine = NPEDINE;
        }
        for (i = inizio;i<fine;i++){
            /*Controllo solo le pedine che posso mangiare perchè sono o il TOP della torre o pedina singola*/
            if ((field->pedine[i].altezza==SINGLE||field->pedine[i].altezza==TOP)&&field->pedine[i].colore!=field->pedine[index].colore){
                /*Controllo quale o quali pedina/e devo mangiare*/
                if (field->pedine[i].coord.y==field->pedine[index].coord.y+col/*Deve essere nella riga successiva*/){
                    if (field->pedine[i].coord.x==field->pedine[index].coord.x+1){/*Cerco in una diagonale*/
                        /*Cerco gli spazi di destinazione della mia pedina*/
                        for (j = 0;j<field->nblanks;j++){
                            if (field->blanks[j].coord.y==field->pedine[index].coord.y+col+col&&
                            (field->blanks[j].coord.x==field->pedine[index].coord.x+2)){
                                sol[c] = j;
                                c++;
                                printf("%d: Riga %d Colonna %d\n", c, field->blanks[j].coord.y, field->blanks[j].coord.x);
                            }
                        }
                    } else if (field->pedine[i].coord.x==field->pedine[index].coord.x-1){/*Cerco nell'altra diagonale*/
                        /*Cerco gli spazi di destinazione della mia pedina*/
                        for (j = 0;j<field->nblanks;j++){
                            if (field->blanks[j].coord.y==field->pedine[index].coord.y+col+col&&
                            (field->blanks[j].coord.x==field->pedine[index].coord.x-2)){
                                sol[c] = j;
                                c++;
                                printf("%d: Riga %d Colonna %d\n", c, field->blanks[j].coord.y, field->blanks[j].coord.x);
                            }
                        }
                    }
                }
            }
        }

        /*Se non ho mosse obbligatorie controllo le mosse di una casella in diagonale*/
    } else{
        for (i = 0;i<field->nblanks;i++){
            if (field->blanks[i].coord.y==field->pedine[index].coord.y+col&&(
                field->blanks[i].coord.x==field->pedine[index].coord.x+1||
                field->blanks[i].coord.x==field->pedine[index].coord.x-1)){
                sol[c] = i;
                c++;
                printf("%d: Riga %d Colonna %d\n", c, field->blanks[i].coord.y, field->blanks[i].coord.x);
            }
        }
    }

    if (c==0){
        printf("error \n");
    }
    printf("Dove vuoi muovere la pedina? ");
    while (selezione==-1||selezione-1>c)
        scanf("%d", &selezione);
    printf("\n");
    indexb = sol[selezione-1];
    return indexb;
}

void sel_pedina(enum color colore, field_t* field){
    int i, index = NPEDINE+1, indexb, isol = 0;
    int inizio, fine;
    bool_t control = FALSE;
    int sol[20];
    int c = 0;
    if (colore){
        inizio = 0;
        fine = NPEDINE/2;
    } else{
        inizio = NPEDINE/2;
        fine = NPEDINE;
    }
    /*Prima controllo se ho la possibilità di mangiare una pedina*/
    for (i = inizio;i<fine;i++){
        if (field->pedine[i].is_obbligata){
            sol[c] = i;
            c++;
            printf("%d: Riga %d Colonna %d Obbligata a mangiare\n", c, field->pedine[i].coord.y, field->pedine[i].coord.x);
            control = TRUE;
        }
    }
        /*Se non ho mosse obbligate controllo se posso muovere la pedina*/
    if (!control){
        for (i = inizio;i<fine;i++){
            if (field->pedine[i].is_movable){
                sol[c] = i;
                c++;
                printf("%d: Riga %d Colonna %d \n", c, field->pedine[i].coord.y, field->pedine[i].coord.x);
            }
        }
    }

    while (index==NPEDINE+1){
        printf("Quale pedina vuoi muovere: ");
        scanf("%d", &isol);
        index = sol[isol-1];
        printf("\n");
    /*Controllo l'input*/
        if (isol>c||isol<1){
            printf("errore nell'inserimento \n");
            index = NPEDINE+1;
        }

        if (!field->pedine[index].is_movable){
            printf("errore nell'inserimento \n");
            index = NPEDINE+1;
        }

    }

/*Procedo con la decisione della mossa*/

    indexb = possible_moves(colore, field, index);
    spostamento_pedine(field, colore, index, indexb);
}
/**
 * @brief Date le cordinate mi restituisce un char corrispondente alla struttura della pedina in quela casella
 *        Funcion that given a set of coordinates gives back the char vector with the list of pawns for the print funcion
 * @param field
 * @param cor
 * @return char*
 */
char* coord_to_pedina(field_t* field, coord_t cor){
    char* sol = malloc(sizeof(char) * 20);
    int index = coord_to_index(field, cor);
    if (field->pedine[index].altezza == SINGLE)
        sol[1] = field->pedine[index].colore;
    else if (field->pedine[index].altezza == TOP){

    }
    return sol;
}

int main(){
    
   /*  if (selezione==1){
        while (!field.partita.END_OF_PLAY){
            stampa_field(&field);
            movable(BLACK, &field);
            if (field.partita.END_OF_PLAY)
                break;
            mossacpu = cpu_turn(&field);
            
            spostamento_pedine(&field, BLACK, mossacpu.index, mossacpu.indexb);
            printf("Score %d Index %d Indexb %d \n", mossacpu.score, mossacpu.index, mossacpu.indexb);
            
            fixbugs(&field);

            stampa_field(&field);
            movable(WHITE, &field);
            if (field.partita.END_OF_PLAY)
                break;
            sel_pedina(WHITE, &field);
            print_pedine(&field);
            fixbugs(&field);
        }
    } */

}