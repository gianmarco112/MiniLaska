/**
 * @file Minilaska.c
 * @author Gianmarco Perini (883492@stud.unive.it)
 * @brief Mini Laska con interfaccia da terminale
 * @version 0.1
 * @date 16-03-2021
 *
 * @copyright Copyright (c) 2021
 *
 */

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

typedef struct triple_int{
    int score;
    int index;
    int indexb;
}triple_t;
typedef struct vect{
    int* v;
    int size;
    bool_t is_obbligata;
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
triple_t cpu_turn(field_t* field);
triple_t turn_cpu(field_t* field);
triple_t cpu_pedina(field_t field, int depth, enum color colore);
triple_t cpu_mossa(field_t field, int index, int depth, enum color colore);
void pedina_player(field_t* field, enum color colore);
void sel_pedina2(enum color colore, field_t* field, vect* soluzione);
triple_t mossa_cpu(field_t field, enum color colore, int index, int depth);
triple_t pedina_cpu(field_t field, enum color colore, int depth);
int n_promosse(field_t* field, enum color colore);
int n_pedine(field_t* field, enum color colore);
int mossa_player(field_t* field, enum color colore, int index);
void possible_moves2(enum color colore, field_t* field, int index, vect* soluzione);
void stampa_field(field_t* field);
void coord_to_char(field_t* field, int x, int y, char* ped);
void endgame(field_t* field);
void start_game(field_t* field);
/*FINE LISTA DELLE FUNZIONI*/

/**
 * @brief Funzione che restituisce il minimo tra due numeri
 *
 * @param a
 * @param b
 * @return int
 */
int min(int a, int b){ if (a < b)return a;else return b; }

void endgame(field_t* field){

}

/**
 * @brief Per scopi di debug stampa posizione, il parametro movable e altezza di tutte le pedine
 *
 * @param field
 */
void print_pedine(field_t* field){
    int i;
    for (i = 0;i < NPEDINE;i++){
        printf("Riga %d Colonna %d Movable %d Altezza ", field->pedine[i].coord.y, field->pedine[i].coord.x, field->pedine[i].is_movable);
        if (field->pedine[i].altezza == SINGLE){
            printf("SINGLE ");
        } else if (field->pedine[i].altezza == TOP){
            printf("TOP ");
        } else if (field->pedine[i].altezza == BOTTOM){
            printf("BOTTOM ");
        } else if (field->pedine[i].altezza == CENTER){
            printf("CENTER ");
        }
        if (field->pedine[i].in_game){
            printf("IN GAME\n");
        } else{
            printf("NO\n");
        }
    }
}
/**
 * @brief Per scopi di debug date delle coordinate mi restituisce l'indice della pedina in quel punto e il colore
 *
 * @param field
 */
void info(field_t* field){
    int ok, xsol, ysol, index;
    coord_t sol;
    printf("Vuoi informazioni su qualche pedina? 0=no 1=si \n");
    scanf("%d", &ok);
    if (ok){
        printf("Quale posizione?\n");
        printf("Riga: ");
        scanf("%d", &ysol);
        printf("Colonna: ");
        scanf("%d", &xsol);
        sol.x = xsol;
        sol.y = ysol;
        index = coord_to_index(field, sol);
        printf("%d\n", index);
        if (field->pedine[index].colore)
            printf("WHITE\n");
        else
            printf("BLACK\n");
    }
}


/**
 * @brief Funzione che controlla se le varie pedine si possono muovere e ne modifica il parametro
 *        Function responsable for checking each pawn if it can move and it updates the corresponding parameter
 * @param colore
 * @param field
 */
void movable(enum color colore, field_t* field){
    int i, j, inizio, fine, col;

    /*Imposto la fine della partita a TRUE, e successivamente se qualche pedina si può muovere la partita non è finita*/
    field->partita.END_OF_PLAY = TRUE;
    if (colore){
        inizio = 0;
        fine = NPEDINE / 2;
        col = 1;
    } else{
        inizio = NPEDINE / 2;
        fine = NPEDINE;
        col = -1;
    }
    for (i = inizio;i < fine;i++){
        /*Resetto le impostazioni di movable e obbligata a FALSE*/
        field->pedine[i].is_movable = FALSE;
        field->pedine[i].is_obbligata = FALSE;
        if (field->pedine[i].altezza == SINGLE || field->pedine[i].altezza == TOP){
            promossa(field, i);
            obbligata(field, i, colore);
            for (j = 0;j < field->nblanks;j++){
                if (field->blanks[j].coord.y == field->pedine[i].coord.y + col && (
                    field->blanks[j].coord.x == field->pedine[i].coord.x + 1 ||
                    field->blanks[j].coord.x == field->pedine[i].coord.x - 1)){
                    field->pedine[i].is_movable = TRUE;
                    field->partita.END_OF_PLAY = FALSE;
                }
            }
            if (field->pedine[i].promossa == YES){
                for (j = 0;j < field->nblanks;j++){
                    if (field->blanks[j].coord.y == field->pedine[i].coord.y - col && (
                        field->blanks[j].coord.x == field->pedine[i].coord.x + 1 ||
                        field->blanks[j].coord.x == field->pedine[i].coord.x - 1)){
                        field->pedine[i].is_movable = TRUE;
                        field->partita.END_OF_PLAY = FALSE;
                    }
                }
            }
        }

    }
    /*if (!field->partita.END_OF_PLAY){
        bool_t ok = FALSE;
        for (i = 0;i < NPEDINE;i++){
            if (field->pedine->is_movable){
                ok = TRUE;
            }
        }
        field->partita.END_OF_PLAY = ok;
    }*/
}

/**
 * @brief Funzione che data una pedina mi restituisce se essa è obbligata a mangiare o meno
 *        Function that checks if a pawn must move and eat the opponent and updates the corresponding parameter
 * @param field
 * @param index
 * @param colore
 */

void obbligata(field_t* field, int index, enum color colore){
    int i, j, inizio, fine, col;
    if (!colore){
        inizio = 0;
        fine = NPEDINE / 2;
        col = -1;
    } else{
        inizio = NPEDINE / 2;
        fine = NPEDINE;
        col = 1;
    }
    for (i = inizio;i < fine;i++){
        /*Controllo solo le pedine che posso mangiare perchè sono o il TOP della torre o pedina singola*/
        if ((field->pedine[i].altezza == SINGLE || field->pedine[i].altezza == TOP) &&
            field->pedine[i].colore != field->pedine[index].colore){
            /*Controllo quale o quali pedina/e devo mangiare*/
            if (field->pedine[i].coord.y == field->pedine[index].coord.y + col/*Deve essere nella riga successiva*/){
                if (field->pedine[i].coord.x == field->pedine[index].coord.x + 1){/*Cerco in una diagonale*/
                    /*Cerco gli spazi di destinazione della mia pedina*/
                    for (j = 0;j < field->nblanks;j++){
                        if (field->blanks[j].coord.y == field->pedine[index].coord.y + col + col &&
                        (field->blanks[j].coord.x == field->pedine[index].coord.x + 2)){
                            field->pedine[index].is_obbligata = TRUE;
                            field->pedine[index].is_movable = TRUE;
                            field->partita.END_OF_PLAY = FALSE;
                        }
                    }
                } else if (field->pedine[i].coord.x == field->pedine[index].coord.x - 1){/*Cerco nell'altra diagonale*/
                    /*Cerco gli spazi di destinazione della mia pedina*/
                    for (j = 0;j < field->nblanks;j++){
                        if (field->blanks[j].coord.y == field->pedine[index].coord.y + col + col &&
                        (field->blanks[j].coord.x == field->pedine[index].coord.x - 2)){
                            field->pedine[index].is_obbligata = TRUE;
                            field->pedine[index].is_movable = TRUE;
                            field->partita.END_OF_PLAY = FALSE;
                        }
                    }
                }
            }if (field->pedine[index].promossa){
                if (field->pedine[i].coord.y == field->pedine[index].coord.y - col/*Deve essere nella riga successiva*/){
                    if (field->pedine[i].coord.x == field->pedine[index].coord.x + 1){/*Cerco in una diagonale*/
                        /*Cerco gli spazi di destinazione della mia pedina*/
                        for (j = 0;j < field->nblanks;j++){
                            if (field->blanks[j].coord.y == field->pedine[index].coord.y - col - col &&
                            (field->blanks[j].coord.x == field->pedine[index].coord.x + 2)){
                                field->pedine[index].is_obbligata = TRUE;
                                field->pedine[index].is_movable = TRUE;
                                field->partita.END_OF_PLAY = FALSE;
                            }
                        }
                    } else if (field->pedine[i].coord.x == field->pedine[index].coord.x - 1){/*Cerco nell'altra diagonale*/
                        /*Cerco gli spazi di destinazione della mia pedina*/
                        for (j = 0;j < field->nblanks;j++){
                            if (field->blanks[j].coord.y == field->pedine[index].coord.y - col - col &&
                            (field->blanks[j].coord.x == field->pedine[index].coord.x - 2)){
                                field->pedine[index].is_obbligata = TRUE;
                                field->pedine[index].is_movable = TRUE;
                                field->partita.END_OF_PLAY = FALSE;
                            }
                        }
                    }
                }
            }

        } else if (!(field->pedine[i].altezza == SINGLE || field->pedine[i].altezza == TOP)){
            /*Nel caso non sia la pedina top o singola resetto il suo stato di promozione*/
            field->pedine[i].promossa = NO;
        }
    }

}
/**
 * @brief Funzione che controlla se una pedina ha raggiunto il lato opposto del campo e deve essere promossa, ne modifica quindi il parametro
 *        Funciotn that checks if a pawn has reached the opposite first line and if so, it updates the corresponding parameter
 * @param field
 * @param index
 */
void promossa(field_t* field, int index){
    if (field->pedine[index].colore && field->pedine[index].coord.y == NROWS){
        field->pedine[index].promossa = YES;
    } else if ((!field->pedine[index].colore) && field->pedine[index].coord.y == 1){
        field->pedine[index].promossa = YES;
    } else{
        field->pedine[index].promossa = NO;
    }
}

/**
 * @brief Funzione che riempie un vettore di tutte le possibili pedine che si possono spostare del colore passato per parametro
 *
 * @param colore
 * @param field
 * @param soluzione
 */
void sel_pedina2(enum color colore, field_t* field, vect* soluzione){
    int i;
    int inizio, fine;
    int c = 0;
    movable(colore, field);
    soluzione->is_obbligata = FALSE;
    if (colore){/*Bianco*/
        inizio = 0;
        fine = NPEDINE / 2;
    } else{/*Nero*/
        inizio = NPEDINE / 2;
        fine = NPEDINE;
    }
    /*Prima controllo se ho la possibilità di mangiare una pedina*/
    for (i = inizio;i < fine;i++){
        if (field->pedine[i].is_obbligata){
            soluzione->v[c] = i;
            c++;
            soluzione->is_obbligata = TRUE;
        }
    }
        /*Se non ho mosse obbligate controllo se posso muovere la pedina*/
    if (!soluzione->is_obbligata){
        for (i = inizio;i < fine;i++){
            if (field->pedine[i].is_movable){
                soluzione->v[c] = i;
                c++;

            }
        }
    }
    if (c == 0)
        printf("Errore size pedine\n");

    soluzione->size = c;


}

/**
 * @brief Prende in input il colore del giocatore, il campo di gioco e l'indice della pedina da muovere e print le possibili mosse di quella pedina
 * Viene chiamata dalla funzione sel_pedina
 *        Funcion called next to sel_pedina responsable for displaying the possible moves that the pawn corresponding to the index is capable of
 * @param colore
 * @param field
 * @param index
 * @return int
 */

void possible_moves2(enum color colore, field_t* field, int index, vect* soluzione){
    int i, c = 0;

    int col;

    if (colore){/*Bianco*/
        col = 1;
    } else{/*Nero*/
        col = -1;
    }
    if (field->partita.END_OF_PLAY)printf("END\n");
    soluzione->size = 0;
    soluzione->is_obbligata = FALSE;
    soluzione->v[0] = -1;
    /*Se la pedina che devo muovere is_obbligata*/
    if (field->pedine[index].is_obbligata){

        int j, inizio, fine;
        soluzione->is_obbligata = TRUE;
        if (!colore){/*Nero*/
            inizio = 0;
            fine = NPEDINE / 2;
        } else{/*Bianco*/
            inizio = NPEDINE / 2;
            fine = NPEDINE;
        }

        for (i = inizio;i < fine;i++){
            /*Controllo solo le pedine che posso mangiare perchè sono o il TOP della torre o pedina singola*/
            if ((field->pedine[i].altezza == SINGLE || field->pedine[i].altezza == TOP) &&
            field->pedine[i].colore != field->pedine[index].colore){
                /*Controllo quale o quali pedina/e devo mangiare*/
                if (field->pedine[i].coord.y == field->pedine[index].coord.y + col/*Deve essere nella riga successiva*/){
                    if (field->pedine[i].coord.x == field->pedine[index].coord.x + 1){/*Cerco in una diagonale*/
                        /*Cerco lo spazio di destinazione della mia pedina*/
                        for (j = 0;j < field->nblanks;j++){
                            if (field->blanks[j].coord.y == field->pedine[index].coord.y + col + col &&
                            (field->blanks[j].coord.x == field->pedine[index].coord.x + 2)){
                                soluzione->v[c] = j;
                                c++;
                                soluzione->size++;
                            }
                        }
                    } else if (field->pedine[i].coord.x == field->pedine[index].coord.x - 1){/*Cerco nell'altra diagonale*/
                        /*Cerco lo spazio di destinazione della mia pedina*/
                        for (j = 0;j < field->nblanks;j++){
                            if (field->blanks[j].coord.y == field->pedine[index].coord.y + col + col &&
                            (field->blanks[j].coord.x == field->pedine[index].coord.x - 2)){
                                soluzione->v[c] = j;
                                c++;
                                soluzione->size++;
                            }
                        }
                    }
                }
                if (field->pedine[index].promossa){
                    if (field->pedine[i].coord.y == field->pedine[index].coord.y - col/*Deve essere nella riga successiva*/){
                        if (field->pedine[i].coord.x == field->pedine[index].coord.x + 1){/*Cerco in una diagonale*/
                            /*Cerco gli spazi di destinazione della mia pedina*/
                            for (j = 0;j < field->nblanks;j++){
                                if (field->blanks[j].coord.y == field->pedine[index].coord.y - col - col &&
                                (field->blanks[j].coord.x == field->pedine[index].coord.x + 2)){
                                    soluzione->v[c] = j;
                                    c++;
                                    soluzione->size++;
                                }
                            }
                        } else if (field->pedine[i].coord.x == field->pedine[index].coord.x - 1){/*Cerco nell'altra diagonale*/
                            /*Cerco gli spazi di destinazione della mia pedina*/
                            for (j = 0;j < field->nblanks;j++){
                                if (field->blanks[j].coord.y == field->pedine[index].coord.y - col - col &&
                                (field->blanks[j].coord.x == field->pedine[index].coord.x - 2)){
                                    soluzione->v[c] = j;
                                    c++;
                                    soluzione->size++;
                                }
                            }
                        }
                    }
                }
            }
        }

        /*Se non ho mosse obbligatorie controllo le mosse di una casella in diagonale*/
    } else{
        for (i = 0;i < field->nblanks;i++){
            if (field->blanks[i].coord.y == field->pedine[index].coord.y + col && (
                field->blanks[i].coord.x == field->pedine[index].coord.x + 1 ||
                field->blanks[i].coord.x == field->pedine[index].coord.x - 1)){
                soluzione->v[c] = i;
                c++;
                soluzione->size++;
            }
            if (field->pedine[index].promossa){
                if (field->blanks[i].coord.y == field->pedine[index].coord.y - col && (
                    field->blanks[i].coord.x == field->pedine[index].coord.x + 1 ||
                    field->blanks[i].coord.x == field->pedine[index].coord.x - 1)){
                    soluzione->v[c] = i;
                    c++;
                    soluzione->size++;
                }
            }
        }
    }
    if (c == 0 || soluzione->v[0] == -1){
        printf("Errore size possible moves\n");
        printf("index = %d\n", index);
        printf("movable %d\n", field->pedine[index].is_movable);
    }


}
/**
 * @brief Funzione responsabile per la visualizzazione e della selezione delle pedine per il player, chiama successivamente la mossa_player e infine esegue lo spostamento delle pedine
 *
 * @param field
 * @param colore
 */
void pedina_player(field_t* field, enum color colore){
    int i, index = NPEDINE + 1, isol = -1, indexb;
    vect selezione;
    selezione.size = 0;
    selezione.is_obbligata = FALSE;
    selezione.v = (int*) malloc(sizeof(int) * NPEDINE);
    movable(colore, field);
    sel_pedina2(colore, field, &selezione);
    for (i = 0;i < selezione.size;i++){
        if (selezione.is_obbligata){
            printf("%d: Riga %d Colonna %d Obbligata a mangiare\n", i + 1, field->pedine[selezione.v[i]].coord.y, field->pedine[selezione.v[i]].coord.x);
        } else{
            printf("%d: Riga %d Colonna %d \n", i + 1, field->pedine[selezione.v[i]].coord.y, field->pedine[selezione.v[i]].coord.x);
        }
    }
    while (index == NPEDINE + 1){
        printf("Quale pedina vuoi muovere: ");
        scanf("%d", &isol);
        index = selezione.v[isol - 1];
        printf("\n");
        if (isol > selezione.size + 1 || isol < 1){
            printf("errore nell'inserimento \n");
            index = NPEDINE + 1;
        }

        if (!field->pedine[index].is_movable){
            printf("errore nell'inserimento \n");
            index = NPEDINE + 1;
        }
    }
    indexb = mossa_player(field, colore, index);
    spostamento_pedine(field, colore, index, indexb);
    free(selezione.v);
}
/**
 * @brief Nuova funzione per la scelta della mossa del player data la pedina da muovere
 *
 * @param field
 * @param colore
 * @param index
 * @return int
 */
int mossa_player(field_t* field, enum color colore, int index){
    int i = 0, indexb = -1, selezione = -1;
    vect mossa;
    mossa.v = (int*) malloc(sizeof(int) * NPEDINE);
    possible_moves2(colore, field, index, &mossa);
    if (mossa.size == 0){
        printf("error size mossa player\n");
    }
    for (i = 0;i < mossa.size;i++){
        printf("%d: Riga %d Colonna %d\n", i + 1, field->blanks[mossa.v[i]].coord.y, field->blanks[mossa.v[i]].coord.x);
    }

    printf("Dove vuoi muovere la pedina? ");
    while (selezione == -1 || selezione - 1 > mossa.size)
        scanf("%d", &selezione);
    printf("\n");
    indexb = mossa.v[selezione - 1];
    free(mossa.v);
    return indexb;
}
/**
 * @brief Funzione che serve per invocare il turno della cpu e ritorna la mossa da eseguire
 *
 * @param field
 * @return triple_t
 */
triple_t turn_cpu(field_t* field){
    field_t  campo = *field;

    pedina_t* copiapedine = malloc(sizeof(pedina_t) * NPEDINE);
    blanks_t* copiablanks = malloc(sizeof(blanks_t) * field->nblanks);
    int i, z;
    triple_t sol;
    sol.index = -3;
    sol.indexb = -3;
    sol.score = 0;
    for (i = 0;i < NPEDINE;i++){
        copiapedine[i] = field->pedine[i];
        if (i < field->nblanks)
            copiablanks[i] = field->blanks[i];
    }
    sol = pedina_cpu(campo, BLACK, 6);
    for (z = 0;z < NPEDINE;z++){
        field->pedine[z] = copiapedine[z];
        if (z < field->nblanks)
            field->blanks[z] = copiablanks[z];
    }

    return sol;
}
/**
 * @brief Funzione che permette al computer di scorrere tutte le possibili pedine da muovere,
 *ritorna la mossa in base allo score di tutte le possibilità
 *
 * @param field
 * @param colore
 * @param depth
 * @return triple_t
 */
triple_t pedina_cpu(field_t field, enum color colore, int depth){
    int i, k = 0;
    int massimo = 0, indicemassimo = 0;

    triple_t retval;
    triple_t* max = malloc(sizeof(triple_t) * NPEDINE);
    vect pedine;
    pedine.size = 0;
    pedine.v = (int*) malloc(sizeof(int) * NPEDINE);
    movable(colore, &field);
    /*Se la movable non trova pedine che si possono muovere imposta lo stato di END_OF_PLAY su true*/
    if (field.partita.END_OF_PLAY){
        triple_t res;
        res.score = -3;
        res.indexb = -1;
        res.index = -1;
        return res;
    }

    if (depth == 0){
        triple_t res;
        res.score = 0;
        res.indexb = -1;
        res.index = -1;
        return res;
    }
    sel_pedina2(colore, &field, &pedine);
    if (pedine.size == 0){
        triple_t res;
        printf("Errore size pedine\n");
        res.score = 0;
        res.indexb = -1;
        res.index = -1;
        return res;
    }
    for (i = 0;i < pedine.size;i++){
        max[k].index = -3;
        max[k].indexb = -3;
        max[k].score = -3;

        max[k] = mossa_cpu(field, colore, pedine.v[i], depth);
        max[k].index = pedine.v[i];
        k++;
    }
    if (k == 0){
        triple_t res;
        if (!field.partita.END_OF_PLAY)printf("Errore end of play\n");
        res.score = -3;
        res.indexb = -1;
        res.index = -1;
        return res;

    }
    massimo = max[0].score;
    for (i = 0;i < k;i++){
        if (max[i].score > massimo){
            massimo = max[i].score;
            indicemassimo = i;
        }
    }
    retval = max[indicemassimo];
    free(max);
    free(pedine.v);
    return retval;

}
/**
 * @brief Funzione che data una pedina valuta tutte le possibili mosse di una pedina e in base al risultato ritorna quella migliore
 *
 * @param field
 * @param colore
 * @param index
 * @param depth
 * @return triple_t
 */
triple_t mossa_cpu(field_t field, enum color colore, int index, int depth){
    int i = 0, z, countersol = 0, npedine1, npedineAvv1, npedine2, npedineAvv2, npromosse1, npromosse2, npromosseAvv1, npromosseAvv2;
    triple_t sol[NPEDINE];
    triple_t res;
    triple_t retval;
    pedina_t* copiapedine = (pedina_t*) malloc(sizeof(pedina_t) * NPEDINE);
    blanks_t* copiablanks = (blanks_t*) malloc(sizeof(blanks_t) * field.nblanks);
    int nblanks = field.nblanks;
    /*Creazione di mossa e allocamento di memoria dell'array*/
    vect mossa;
    mossa.size = 0;
    mossa.v = (int*) malloc(sizeof(int) * NPEDINE);
    /*Evento impossibile in quanto dovrebbe fermarsi prima*/
    if (depth == 0 || field.partita.END_OF_PLAY){
        res.score = 0;
        res.indexb = 0;
        return res;
    }
    /*Salvo lo stato attuale delle pedine e degli spazi*/
    for (i = 0;i < NPEDINE;i++){
        copiapedine[i] = field.pedine[i];
        if (i < field.nblanks)
            copiablanks[i] = field.blanks[i];
    }
    possible_moves2(colore, &field, index, &mossa);/*Vettore con tutte le possibili pedine*/
    /*Se la size è zero significa che c'è stato un errore,
    in quanto se una pedina si può muovere deve avere necessariamente delle mosse*/
    if (mossa.size == 0 && !field.partita.END_OF_PLAY){
        printf("Error size mossa cpu\n");
    }

    npedine1 = n_pedine(&field, BLACK);
    npedineAvv1 = n_pedine(&field, WHITE);
    npromosse1 = n_promosse(&field, BLACK);
    npromosseAvv1 = n_promosse(&field, WHITE);

    for (i = 0;i < mossa.size;i++){
        /*Eseguo una delle mosse*/
        spostamento_pedine(&field, colore, index, mossa.v[i]);
        /*Inizializzazione preventiva*/
        sol[countersol].index = -3;
        sol[countersol].score = -3;
        sol[countersol].indexb = -3;
        /*Chiamata del ramo successivo*/
        res = pedina_cpu(field, !colore, depth - 1);
        /*Riassegnazione dei valori*/
        sol[countersol].index = index;
        sol[countersol].score = res.score;
        sol[countersol].indexb = mossa.v[i];
        /*Controllo numero delle pedine*/
        npedine2 = n_pedine(&field, BLACK);
        npedineAvv2 = n_pedine(&field, WHITE);
        npromosse2 = n_promosse(&field, BLACK);
        npromosseAvv2 = n_promosse(&field, WHITE);
        /*Assegnamento del punteggio*/
        if (npedine1 < npedine2)sol[countersol].score++;
        else if (npedine1 > npedine2)sol[countersol].score--;
        if (npedineAvv1 > npedineAvv2)sol[countersol].score++;
        else if (npedineAvv1 < npedineAvv2)sol[countersol].score--;
        if (npromosse1 < npromosse2)sol[countersol].score++;
        else if (npromosse1 > npromosse2)sol[countersol].score--;
        if (npromosseAvv1 > npromosseAvv2)sol[countersol].score++;
        else if (npromosseAvv1 < npromosseAvv2)sol[countersol].score--;
        countersol++;
        /*Annullamento mossa*/
        field.nblanks = nblanks;
        for (z = 0;z < NPEDINE;z++){
            field.pedine[z] = copiapedine[z];
            if (z < field.nblanks)
                field.blanks[z] = copiablanks[z];
        }

    }
    /*Controllo possibili errori*/
    if (countersol < 0)printf("Errore countersol\n");
    retval = sol[0];
    /*Scelta del percorso migliore*/
    while (countersol > 0){
        if (retval.score < sol[countersol].score)
            retval = sol[countersol];
        countersol--;
    }
    /*free degli array creati*/
    free(copiablanks);
    free(copiapedine);
    free(mossa.v);
    return retval;

}



/**
 * @brief Funzione che si occupa di gestire lo spostamento delle pedine dopo la selezione della mossa
 *        Funcion responsable for swapping pawns after the user/computer selection
 * @param field
 * @param colore
 * @param index
 * @param indexb
 */
void spostamento_pedine(field_t* field, enum color colore, int index, int indexb){
    coord_t mossa;
    mossa = field->blanks[indexb].coord;
    /*Ritorna l'indice dello spazio bianco di destinazione dal quale estraggo le cordinate*/

    /*Se è una mossa obbligata*/
    if (field->pedine[index].is_obbligata){

        int indexM = -1;
        int i;

        for (i = 0;i < NPEDINE;i++){
            /*Ricavo l'indice della pedina da mangiare,
            prendo il minore tra le cordinate e ci aggiungo uno per trovare la pedina di mezzo*/
            if (field->pedine[i].coord.y == min(field->pedine[index].coord.y, field->blanks[indexb].coord.y) + 1 &&
            field->pedine[i].coord.x == min(field->pedine[index].coord.x, field->blanks[indexb].coord.x) + 1 &&
            (field->pedine[i].altezza == TOP || field->pedine[i].altezza == SINGLE))
                indexM = i;
        }
        if (indexM == -1){
            printf("Error pedina mangiata\n");
        }
        /*Sposto lo spazio*/
        field->blanks[indexb].coord = field->pedine[index].coord;

        /*Caso in cui la pedina mangiata sia singola*/
        if (field->pedine[indexM].altezza == SINGLE){
            /*ne creo uno in più*/
            field->blanks[field->nblanks].coord = field->pedine[indexM].coord;
            /*Incremento il numero di spazi*/
            field->nblanks++;
            field->pedine[indexM].altezza = BOTTOM;

        /*Nel caso in cui rubo una torre libero le pedine sotto*/
        } else if (field->pedine[indexM].altezza == TOP){
            bool_t doppia = FALSE;
            /*Cerco se esiste una pedina al centro ovvero se la torre era da tre pedine*/
            for (i = 0;i < NPEDINE;i++){
                if (i != indexM &&
                field->pedine[i].coord.x == field->pedine[indexM].coord.x &&
                field->pedine[i].coord.y == field->pedine[indexM].coord.y){
                    if (field->pedine[i].altezza == CENTER){/*LA PEDINA AL CENTRO diventa TOP e la BOTTOM resta BOTTOM*/
                        field->pedine[i].altezza = TOP;
                        doppia = TRUE;

                    }
                }
            }
            if (!doppia){/*Caso in cui la torre sia da due (TOP e BOTTOM)*/
                for (i = 0;i < NPEDINE;i++){
                    if (i != indexM &&
                    field->pedine[i].coord.x == field->pedine[indexM].coord.x &&
                    field->pedine[i].coord.y == field->pedine[indexM].coord.y){
                        if (field->pedine[i].altezza == BOTTOM){
                            field->pedine[i].altezza = SINGLE;
                        }
                    }
                }
            }
            field->pedine[indexM].altezza = BOTTOM;
        }


        /*Nel caso la pedina che mangia sia una torre*/
        if (field->pedine[index].altezza == TOP){
            /*Cerco le pedine sottostanti*/
            int indicebottom = -1;
            coord_t cordind = field->pedine[index].coord;
            for (i = 0;i < NPEDINE;i++){
                if (i != index && field->pedine[i].coord.x == cordind.x &&
                field->pedine[i].coord.y == cordind.y){
                    /*Sposto le pedine che compongono la torre*/
                    field->pedine[i].coord = mossa;
                    if (field->pedine[i].altezza == CENTER){

                        if (indicebottom != -1){
                            field->pedine[indicebottom].altezza = BOTTOM;
                        }
                        break;
                    } else if (field->pedine[i].altezza == BOTTOM){

                        indicebottom = i;
                        field->pedine[i].altezza = CENTER;
                    }
                }
            }


            if (indicebottom != -1 && field->pedine[indicebottom].altezza != BOTTOM){
                field->pedine[indexM].altezza = BOTTOM;
                field->pedine[indexM].coord = mossa;
            } else{
                field->pedine[indexM].in_game = FALSE;
                field->pedine[indexM].coord.x = -1;
                field->pedine[indexM].coord.y = -1;
            }


        }

        if (field->pedine[indexM].in_game){
            field->pedine[indexM].coord = mossa;
        }

        if (field->pedine[index].in_game){
            field->pedine[index].coord = mossa;
            field->pedine[index].altezza = TOP;
        }

    } else{
        int i;

       /*Metto lo spazio nelle coordinate della pedina di partenza*/
        field->blanks[indexb].coord = field->pedine[index].coord;


        /*Sposto tutte le pedine*/
        for (i = 0; i < NPEDINE; i++){
            if (i != index && field->pedine[i].coord.x == field->pedine[index].coord.x &&
                field->pedine[i].coord.y == field->pedine[index].coord.y){
                field->pedine[i].coord = mossa;
            }
        }
        field->pedine[index].coord = mossa;

        promossa(field, index);

    }
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


/**
 * @brief Date le cordinate restituisce l'indice della pedina in quel punto, torna -1 se non ci sono pedine
 *        Given a set of coordinates gives back the index of the pawn that sits in that spot, otherwise it gives back -1 if none
 * @param field
 * @param coord
 * @return int
 */
int coord_to_index(field_t* field, coord_t coord){
    int i, index = -1;
    for (i = 0;i < NPEDINE;i++){
        if (field->pedine[i].coord.y == coord.y &&
            field->pedine[i].coord.x == coord.x &&
            (field->pedine[i].altezza == SINGLE ||
                field->pedine[index].altezza == TOP)){
            index = i;
            break;
        }
    }
    return index;
}




/*se il player sbaglia pedina o vuole cambiarla*/
void ops_sel(){

}
/**
 * @brief Date delle cordinate mi restituisce un char che serve per la print del campo di gioco
 *
 * @param field
 * @param x
 * @param y
 * @param ped
 */
void coord_to_char(field_t* field, int x, int y, char* ped){
    int i;
    for (i = 0;i < field->nblanks;i++){
        if (field->blanks[i].coord.x == x &&
            field->blanks[i].coord.y == y){
            ped[0] = ' ';
            ped[1] = ' ';
            ped[2] = ' ';

        }
    }
    for (i = 0; i < NPEDINE; i++){
        if (field->pedine[i].coord.x == x &&
            field->pedine[i].coord.y == y){
            if (field->pedine[i].altezza == SINGLE){
                ped[0] = ' ';
                ped[2] = ' ';
                if (field->pedine[i].colore){
                    if (field->pedine[i].promossa)
                        ped[1] = 'B';
                    else
                        ped[1] = 'b';
                } else{
                    if (field->pedine[i].promossa)
                        ped[1] = 'N';
                    else
                        ped[1] = 'n';
                }
                return;
            } else if (field->pedine[i].altezza == TOP){
                if (ped[1] == '|')ped[1] = ' ';/*Nel caso in cui non abbia già trovato una pedina al centro*/
                if (field->pedine[i].colore){
                    ped[0] = 'b';
                } else{
                    ped[0] = 'n';
                }
            } else if (field->pedine[i].altezza == CENTER){
                if (field->pedine[i].colore){
                    ped[1] = 'b';
                } else{
                    ped[1] = 'n';
                }
            } else{
                if (field->pedine[i].colore){
                    ped[2] = 'b';
                } else{
                    ped[2] = 'n';
                }
            }
        }
    }
}


/**
 * @brief Funzione che calcola il numero di pedine movibili del giocatore in base al colore passato
 *
 * @param field
 * @param colore
 * @return int
 */
int n_pedine(field_t* field, enum color colore){
    int inizio, fine, i;
    int num = 0;
    if (colore){
        inizio = 0;
        fine = NPEDINE / 2;
    } else{
        inizio = NPEDINE / 2;
        fine = NPEDINE;
    }
    for (i = inizio;i < fine;i++){
        if ((field->pedine[i].altezza == SINGLE || field->pedine[i].altezza == TOP) &&
        field->pedine[i].in_game)
            num++;
    }
    return num;
}
/**
 * @brief Funzione che calcola il numero di pedine movibili e promosse del giocatore in base al colore passato
 *
 * @param field
 * @param colore
 * @return int
 */
int n_promosse(field_t* field, enum color colore){
    int inizio, fine, i;
    int num = 0;
    if (colore){
        inizio = 0;
        fine = NPEDINE / 2;
    } else{
        inizio = NPEDINE / 2;
        fine = NPEDINE;
    }
    for (i = inizio;i < fine;i++){
        if ((field->pedine[i].altezza == SINGLE || field->pedine[i].altezza == TOP) &&
        field->pedine[i].in_game && field->pedine[i].promossa == YES)
            num++;
    }
    return num;
}


void fixbugs(field_t* field){
    int i, j, k, contBottom = 0;
    coord_t coordTOP, coordBOTTOM;
    for (i = 0; i < NPEDINE; i++){
        if (field->pedine[i].altezza == TOP){
            coordTOP = field->pedine[i].coord;
            for (j = 0; j < NPEDINE && contBottom == 0; j++){
                if (j != i && field->pedine[j].coord.x == coordTOP.x && field->pedine[j].coord.y == coordTOP.y && field->pedine[j].altezza == BOTTOM){
                    contBottom++;
                }
            }
            if (!contBottom){/*Se non ha trovato una bottom*/

                for (j = 0; j < NPEDINE; j++){/*Cerco una bottom senza top*/
                    int control2 = 0;
                    if (field->pedine[j].altezza == BOTTOM){
                        coordBOTTOM = field->pedine[i].coord;
                        for (k = 0; k < NPEDINE; k++){
                            if (j != k && field->pedine[k].coord.x == coordBOTTOM.x && field->pedine[k].coord.y == coordBOTTOM.y){
                                control2++;
                                break;
                            }
                        }
                        if (!control2){
                            field->pedine[j].coord = coordTOP;
                            printf("funziona\n");
                            return;
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief Funzione di stampa del campo
 *
 * @param field
 */
void stampa_field(field_t* field){
    int i, j;
    /* int x=1, y=1; */
    char* ped = malloc(3 * sizeof(char));
    ped[0] = '|';
    ped[1] = '|';
    ped[2] = '|';
    printf("  |  1  |  2  |  3  |  4  |  5  |  6  |  7  | \n");
    printf("  ------------------------------------------- \n");
    for (i = 0; i < NROWS; i++){
        /*Stampo il numero di riga e la spaziatura*/
        printf("  |");
        for (j = 0; j < NCOLS; j++){
            coord_to_char(field, j + 1, i + 1, ped);
            if (ped[1] == '|'){
                printf("||||||");
            } else{
                printf("  %c  |", ped[0]);
            }
            ped[0] = '|';
            ped[1] = '|';
            ped[2] = '|';
        }
        printf("\n");
        printf("%d |", i + 1);
        for (j = 0; j < NCOLS; j++){
            coord_to_char(field, j + 1, i + 1, ped);
            if (ped[1] == '|'){
                printf("||||||");
            } else{
                printf("  %c  |", ped[1]);
            }
            ped[0] = '|';
            ped[1] = '|';
            ped[2] = '|';
        }

        printf("\n");
        printf("  |");
        for (j = 0; j < NCOLS; j++){
            coord_to_char(field, j + 1, i + 1, ped);
            if (ped[1] == '|'){
                printf("||||||");
            } else{
                printf("  %c  |", ped[2]);
            }
            ped[0] = '|';
            ped[1] = '|';
            ped[2] = '|';
        }
        printf("\n");

    }
    printf("  ------------------------------------------- \n");
    printf("\n");
}

/**
 * @brief Create a pedine object
 * Funzione usata all'inizio del game per creare le pedine dei due giocatori, accetta in input un field
 * @param field
 */
void create_pedine(field_t* field){
    int i, j, c = 0;
    field->pedine = (struct pedina*) malloc(sizeof(struct pedina) * NPEDINE);

    for (i = 0;i < NPEDINE;i++){
        field->pedine[i].altezza = SINGLE;
        field->pedine[i].in_game = TRUE;
        field->pedine[i].promossa = NO;
    }
    for (i = 0;i < NPEDINE / 2;i++){
        field->pedine[i].colore = WHITE;
    }
    for (i = NPEDINE / 2;i < NPEDINE;i++){
        field->pedine[i].colore = BLACK;
    }
    for (i = 0;i < field->rows / 2;i++){
        if (i % 2 == 0)
            j = 0;
        else
            j = 1;
        for (;j < 7;j += 2){
            if (c == NPEDINE / 2)
                return;
            field->pedine[c].coord.y = i + 1;
            field->pedine[c].coord.x = j + 1;

            c++;
            field->pedine[NPEDINE - c].coord.y = NROWS - i;
            field->pedine[NPEDINE - c].coord.x = j + 1;

        }
    }
}

/**
 * @brief Create a blanks object
 *          Funzione che serve a creare un array con le posizioni degli spazi vuoti
 * @param field
 */
void create_blanks(field_t* field){
    int c = 0, k = 2;
    field->blanks = (struct blanks*) malloc(sizeof(struct blanks) * NPEDINE);
    while (c <= 2){
        field->blanks[c].coord.y = 4;
        field->blanks[c].coord.x = k;
        k += 2;
        c++;
    }
}

/**
 * @brief funzione da eseguire sempre prima di chiudere il programma
 *
 * @param field
 */
void free_pedine(field_t* field){
    free(field->pedine);
    free(field->blanks);
}

/**
 * @brief Funzione di partenza
 *
 * @param field
 */
void start_game(field_t* field){

    field->cols = NCOLS;
    field->rows = NROWS;
    create_pedine(field);
    field->nblanks = 3;
    create_blanks(field);
    field->partita.END_OF_PLAY = FALSE;
    field->partita.VICTORY_P1 = FALSE;

}
int main(){

    int selezione = 0;
    field_t field;
    start_game(&field);
    printf("1: 1vsCPU\n2: 1vs1 \n");
    scanf("%d", &selezione);

    if (selezione == 1){
        while (!field.partita.END_OF_PLAY){
            triple_t mossacpu;
            stampa_field(&field);
            movable(BLACK, &field);
            if (field.partita.END_OF_PLAY){
                printf("Vince il giocatore\n");
                break;
            }
            mossacpu = turn_cpu(&field);
            printf("Score %d Index %d Indexb %d \n", mossacpu.score, mossacpu.index, mossacpu.indexb);
            spostamento_pedine(&field, BLACK, mossacpu.index, mossacpu.indexb);
            stampa_field(&field);
            movable(WHITE, &field);
            if (field.partita.END_OF_PLAY){
                printf("Vince il computer\n");
                break;
            }
            pedina_player(&field, WHITE);
        }
    }
    if (selezione == 2){
        while (!field.partita.END_OF_PLAY){
            print_pedine(&field);
            stampa_field(&field);
            movable(BLACK, &field);
            if (field.partita.END_OF_PLAY)
                break;
            pedina_player(&field, BLACK);
            print_pedine(&field);
            stampa_field(&field);
            movable(WHITE, &field);
            if (field.partita.END_OF_PLAY)
                break;
            pedina_player(&field, WHITE);
        }
    }
    printf("Grazie di aver giocato\n");
    free_pedine(&field);
    return 0;
}
