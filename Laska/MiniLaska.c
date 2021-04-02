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
    TRUE=1,
    FALSE=0
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

typedef struct pair_int{
    int score;
    int index;
    int indexb;
}pair_t;

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
void promossa(field_t *field,int index);
void spostamento_pedine(field_t *field, enum color colore, int index, int indexb);
int coord_to_index(field_t *field, coord_t coord);
char* coord_to_pedina (field_t *field, coord_t cor);
int altezza_pedina(field_t *field, coord_t coord);
pair_t cpu_turn(field_t *field);
pair_t cpu_pedina(field_t field,int depth,enum color colore);
pair_t cpu_mossa(field_t field,int index, int depth,enum color colore);
void stampa_field(field_t *field);
void coord_to_char(field_t *field, int x, int y, char *ped);
void endgame(field_t *field);
void start_game2(field_t *field);
/*FINE LISTA DELLE FUNZIONI*/


int min(int a,int b){if(a<b)return a;else return b;}

void endgame(field_t *field){

}

/**
 * @brief Per scopi di debug stampa posizione, il parametro movable e altezza di tutte le pedine
 * 
 * @param field 
 */
void print_pedine(field_t *field){
    int i;
    for(i=0;i<NPEDINE;i++){
        printf("Riga %d Colonna %d Movable %d Altezza ",field->pedine[i].coord.y,field->pedine[i].coord.x,field->pedine[i].is_movable);
            if(field->pedine[i].altezza==SINGLE){
                printf("SINGLE ");
            }else if(field->pedine[i].altezza==TOP){
                printf("TOP ");
            }else if(field->pedine[i].altezza==BOTTOM){
                printf("BOTTOM ");
            }else if(field->pedine[i].altezza==CENTER){
                printf("CENTER ");  
            }
			if(field->pedine[i].in_game){
				printf("IN GAME\n");
			}else{
				printf("NO\n");
			}
        }
}
/**
 * @brief Per scopi di debug date delle coordinate mi restituisce l'indice della pedina in quel punto e il colore
 * 
 * @param field 
 */
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


/**
 * @brief Funzione che controlla se le varie pedine si possono muovere e ne modifica il parametro
 *        Function responsable for checking each pawn if it can move and it updates the corresponding parameter
 * @param colore 
 * @param field 
 */
void movable(enum color colore, field_t *field){
    int i,j,inizio,fine,col;
    /*Resetto le impostazioni di movable e obbligata a FALSE*/
    
    /*Imposto la fine della partita a TRUE, e successivamente se qualche pedina si può muovere la partita non è finita*/
    field->partita.END_OF_PLAY=TRUE;
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
        field->pedine[i].is_movable=FALSE;
        field->pedine[i].is_obbligata=FALSE;
        if(field->pedine[i].altezza==SINGLE||field->pedine[i].altezza==TOP){
            obbligata(field, i,colore);
            for(j=0;j<field->nblanks;j++){
                if(field->blanks[j].coord.y==field->pedine[i].coord.y+col&&(
                    field->blanks[j].coord.x==field->pedine[i].coord.x+1|| 
                    field->blanks[j].coord.x==field->pedine[i].coord.x-1)){
                    field->pedine[i].is_movable=TRUE;
                    field->partita.END_OF_PLAY=FALSE;
                }
            }
            if(field->pedine[i].promossa==YES){
               for(j=0;j<field->nblanks;j++){
                    if(field->blanks[j].coord.y==field->pedine[i].coord.y-col&&(
                        field->blanks[j].coord.x==field->pedine[i].coord.x+1|| 
                        field->blanks[j].coord.x==field->pedine[i].coord.x-1)){
                        field->pedine[i].is_movable=TRUE;
                        field->partita.END_OF_PLAY=FALSE;
                    }
                } 
            }
        }
        
    }
}

/**
 * @brief Funzione che data una pedina mi restituisce se essa è obbligata a mangiare o meno
 *        Function that checks if a pawn must move and eat the opponent and updates the corresponding parameter
 * @param field 
 * @param index 
 * @param colore 
 */

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
                        field->pedine[index].is_obbligata=TRUE;
                        field->pedine[index].is_movable=TRUE;
                        field->partita.END_OF_PLAY=FALSE;
                        }
                    }
                }else if(field->pedine[i].coord.x==field->pedine[index].coord.x-1){/*Cerco nell'altra diagonale*/
                    /*Cerco gli spazi di destinazione della mia pedina*/
                    for(j=0;j<field->nblanks;j++){
                        if(field->blanks[j].coord.y==field->pedine[index].coord.y+col+col &&
                        (field->blanks[j].coord.x==field->pedine[index].coord.x-2)){
                        field->pedine[index].is_obbligata=TRUE;
                        field->pedine[index].is_movable=TRUE;
                        field->partita.END_OF_PLAY=FALSE;
                        }
                    }
                }
            }if(field->pedine[index].promossa==YES){
                if(field->pedine[i].coord.y == field->pedine[index].coord.y-col/*Deve essere nella riga successiva*/){
                if(field->pedine[i].coord.x==field->pedine[index].coord.x+1){/*Cerco in una diagonale*/
                    /*Cerco gli spazi di destinazione della mia pedina*/
                    for(j=0;j<field->nblanks;j++){
                        if(field->blanks[j].coord.y==field->pedine[index].coord.y-col-col &&
                        (field->blanks[j].coord.x==field->pedine[index].coord.x+2)){
                        field->pedine[index].is_obbligata=TRUE;
                        field->pedine[index].is_movable=TRUE;
                        field->partita.END_OF_PLAY=FALSE;
                        }
                    }
                }else if(field->pedine[i].coord.x==field->pedine[index].coord.x-1){/*Cerco nell'altra diagonale*/
                    /*Cerco gli spazi di destinazione della mia pedina*/
                    for(j=0;j<field->nblanks;j++){
                        if(field->blanks[j].coord.y==field->pedine[index].coord.y-col-col &&
                        (field->blanks[j].coord.x==field->pedine[index].coord.x-2)){
                        field->pedine[index].is_obbligata=TRUE;
                        field->pedine[index].is_movable=TRUE;
                        field->partita.END_OF_PLAY=FALSE;
                        }
                    }
                }
            }
            }
            
        }
    }
    
}
/**
 * @brief Funzione che controlla se una pedina ha raggiunto il lato opposto del campo e deve essere promossa, ne modifica quindi il parametro
 *        Funciotn that checks if a pawn has reached the opposite first line and if so, it updates the corresponding parameter
 * @param field 
 * @param index 
 */
void promossa(field_t *field,int index){
    if(field->pedine[index].colore&&field->pedine[index].coord.y==field->rows&&!field->pedine[index].promossa){
        field->pedine[index].promossa=YES;
    }else if(!field->pedine[index].colore&&field->pedine[index].coord.y==1&&!field->pedine[index].promossa){
        field->pedine[index].promossa=YES;
    }
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
int possible_moves(enum color colore, field_t *field,int index){
    int i,c=0,selezione=-1,indexb;
    /*c è l'indice dell'array di coordinate, selezione è l'indice dello stesso array che però rappresenta la selezione dell'utente*/
    int sol[20];
    /*Ho un array di coordinate delle possibili mosse che vengono proposte all'utente*/
    
    int col;
        if(colore){
            col=1;
        }else{
            col=-1;
        }
    /*Se la pedina che devo muovere is_obbligata*/
    if(field->pedine[index].is_obbligata){
	int i,j,inizio,fine;
    if(!colore){
            inizio=0;
            fine=NPEDINE/2;
        }else{
            inizio=NPEDINE/2;
            fine=NPEDINE;
        }
    for(i=inizio;i<fine;i++){
        /*Controllo solo le pedine che posso mangiare perchè sono o il TOP della torre o pedina singola*/
        if((field->pedine[i].altezza==SINGLE||field->pedine[i].altezza==TOP)&&field->pedine[i].colore!=field->pedine[index].colore){
            /*Controllo quale o quali pedina/e devo mangiare*/
            if(field->pedine[i].coord.y == field->pedine[index].coord.y+col/*Deve essere nella riga successiva*/){
                if(field->pedine[i].coord.x==field->pedine[index].coord.x+1){/*Cerco in una diagonale*/
                    /*Cerco gli spazi di destinazione della mia pedina*/
                    for(j=0;j<field->nblanks;j++){
                        if(field->blanks[j].coord.y==field->pedine[index].coord.y+col+col &&
                        (field->blanks[j].coord.x==field->pedine[index].coord.x+2)){
                        sol[c]=j;
                        c++;
                        printf("%d: Riga %d Colonna %d\n",c,field->blanks[j].coord.y,field->blanks[j].coord.x);
                        }
                    }
                }else if(field->pedine[i].coord.x==field->pedine[index].coord.x-1){/*Cerco nell'altra diagonale*/
                    /*Cerco gli spazi di destinazione della mia pedina*/
                    for(j=0;j<field->nblanks;j++){
                        if(field->blanks[j].coord.y==field->pedine[index].coord.y+col+col &&
                        (field->blanks[j].coord.x==field->pedine[index].coord.x-2)){
                        sol[c]=j;
                        c++;
                        printf("%d: Riga %d Colonna %d\n",c,field->blanks[j].coord.y,field->blanks[j].coord.x);
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
                    printf("%d: Riga %d Colonna %d\n", c ,field->blanks[i].coord.y, field->blanks[i].coord.x);
                }
            }
        }
    
    if(c==0){
        printf("error \n");
    }
    printf("Dove vuoi muovere la pedina? ");
    while(selezione==-1||selezione-1>c)   
        scanf("%d",&selezione);
    printf("\n");
    indexb=sol[selezione-1];
    return indexb;
}


/**
 * @brief function responsable of looking for movable pedine or obliged eats and it prints the list of available ones
 * 
 * @param colore 
 * @param field 
 */
void sel_pedina(enum color colore,field_t *field){
    int i, index=NPEDINE+1,indexb,isol=0;
    int inizio,fine;
    bool_t control=FALSE;
    int sol[20];
    int c=0;
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
            sol[c]=i;
            c++;
            printf("%d: Riga %d Colonna %d Obbligata a mangiare\n",c,field->pedine[i].coord.y,field->pedine[i].coord.x);
            control=TRUE;
        }
    }  
        /*Se non ho mosse obbligate controllo se posso muovere la pedina*/
    if(!control){
        for(i=inizio;i<fine;i++){
            if(field->pedine[i].is_movable){
                sol[c]=i;
                c++;
                printf("%d: Riga %d Colonna %d \n",c,field->pedine[i].coord.y,field->pedine[i].coord.x);
            }
        }
    }
    
        while (index==NPEDINE+1){
            printf("Quale pedina vuoi muovere: ");
                scanf("%d",&isol);
                index=sol[isol-1];
                printf("\n");
            /*Controllo l'input*/
            if(isol>c||isol<1){
                printf("errore nell'inserimento \n");
                index=NPEDINE+1;
            }

            if(!field->pedine[index].is_movable){
                printf("errore nell'inserimento \n");
                index=NPEDINE+1;
            }
        
        }

    /*Procedo con la decisione della mossa*/
    
    indexb=possible_moves(colore, field, index);
    spostamento_pedine(field,colore,index,indexb);
}

/**
 * @brief Funzione che si occupa di gestire lo spostamento delle pedine dopo la selezione della mossa
 *        Funcion responsable for swapping pawns after the user/computer selection
 * @param field 
 * @param colore 
 * @param index 
 * @param indexb 
 */
void spostamento_pedine(field_t *field, enum color colore, int index, int indexb){
    bool_t debug=FALSE;
    coord_t mossa;
    mossa=field->blanks[indexb].coord;
    /*Mi ritorna l'indice dello spazio bianco di destinazione dal quale estraggo le cordinate*/    
    
    /*Se è una mossa obbligata*/
    if(field->pedine[index].is_obbligata){
        
        int indexM=-1;
        int i;
        /*int control=0,k=0,j=0;Serve per il controllo dell'altezza delle pedine*/
        for(i=0;i<NPEDINE;i++){
            /*Ricavo l'indice della pedina da mangiare, 
            prendo il minore tra le cordinate e ci aggiungo uno per trovare la pedina di mezzo*/
            if(field->pedine[i].coord.y==min(field->pedine[index].coord.y,field->blanks[indexb].coord.y)+1&&
            field->pedine[i].coord.x==min(field->pedine[index].coord.x,field->blanks[indexb].coord.x)+1&&
            (field->pedine[i].altezza==TOP||field->pedine[i].altezza==SINGLE))
            indexM=i;
        }
        if(indexM==-1){
            printf("Error pedina mangiata\n");
        }
        /*Sposto lo spazio*/ 
        field->blanks[indexb].coord=field->pedine[index].coord;

        /*Caso in cui la pedina mangiata sia singola*/
        if(field->pedine[indexM].altezza==SINGLE){
            /*ne creo uno in più*/
            field->blanks[field->nblanks].coord=field->pedine[indexM].coord;
            /*Incremento il numero di spazi*/
            field->nblanks++;
            field->pedine[indexM].altezza=BOTTOM;
            
        /*Nel caso in cui rubo una torre libero le pedine sotto*/
        }else if(field->pedine[indexM].altezza==TOP){
            bool_t doppia=FALSE;
			/*Cerco se esiste una pedina al centro ovvero se la torre era da tre pedine*/
            for(i=0;i<NPEDINE;i++){
                if(i!=indexM && 
                field->pedine[i].coord.x==field->pedine[indexM].coord.x&&
                field->pedine[i].coord.y==field->pedine[indexM].coord.y){
                    if(field->pedine[i].altezza==CENTER){/*LA PEDINA AL CENTRO diventa TOP e la BOTTOM resta BOTTOM*/
						field->pedine[i].altezza=TOP;
                        doppia=TRUE;
                        
                    }
                }
            }
            if(!doppia){/*Caso in cui la torre sia da due (TOP e BOTTOM)*/
                for(i=0;i<NPEDINE;i++){
                    if(i!=indexM && 
                    field->pedine[i].coord.x==field->pedine[indexM].coord.x&&
                    field->pedine[i].coord.y==field->pedine[indexM].coord.y){
                        if(field->pedine[i].altezza==BOTTOM){
                            field->pedine[i].altezza=SINGLE;
                        }
                    }
                }
            }
            field->pedine[indexM].altezza=BOTTOM;
        }
		
        
        /*Nel caso la pedina che mangia sia una torre*/
        if(field->pedine[index].altezza==TOP){
            /*Cerco le pedine sottostanti*/
            int indicebottom=-1;
            for(i=0;i<NPEDINE;i++){
                if(i!=index&&field->pedine[i].coord.x==field->pedine[index].coord.x&&
                field->pedine[i].coord.y==field->pedine[index].coord.y){
                    /*Sposto le pedine che compongono la torre*/
                    field->pedine[i].coord=mossa;
                    if(field->pedine[i].altezza==CENTER){
                        /* control++;
                        k=i; */
                        if(indicebottom!=-1){
                            field->pedine[indicebottom].altezza=BOTTOM;
                        }
                        break;
                    }else if(field->pedine[i].altezza==BOTTOM){
                        /* control+=10;
                        j=i; */
                        indicebottom=i;
                        field->pedine[i].altezza=CENTER;
                    }
                }
            }
            
			field->pedine[index].coord=mossa;
            if(indicebottom!=-1 && field->pedine[indicebottom].altezza!=BOTTOM){
                field->pedine[indexM].altezza=BOTTOM;
                field->pedine[indexM].coord=mossa;
            }else{
                field->pedine[indexM].in_game=FALSE;
				field->pedine[indexM].coord.x=-1;
				field->pedine[indexM].coord.y=-1;
            }
			/* if(control==10){
				field->pedine[j].altezza=CENTER;
                field->pedine[indexM].altezza=BOTTOM;
                field->pedine[indexM].coord=mossa;
			}else if(control==11){
				field->pedine[indexM].in_game=FALSE;
				field->pedine[indexM].coord.x=-1;
				field->pedine[indexM].coord.y=-1;
			}else if(control==20){
                field->pedine[j].altezza=CENTER;
                field->pedine[indexM].in_game=FALSE;
				field->pedine[indexM].coord.x=-1;
				field->pedine[indexM].coord.y=-1;
            }else{
                printf("Error 2 control=%d\n",control);
                debug=TRUE;
            } */
		}
       
        if(field->pedine[indexM].in_game){
            field->pedine[indexM].coord=mossa;
            /* field->pedine[indexM].is_movable=FALSE; */
		}
        
        if(field->pedine[index].in_game){
            field->pedine[index].coord=mossa;
            field->pedine[index].altezza=TOP;
        }
        
    }else{
         int i;
        /*Scambio la pedina e lo spazio*/
        field->blanks[indexb].coord=field->pedine[index].coord;
        /* if(field->pedine[index].altezza==SINGLE){
            field->pedine[index].coord=mossa;
        }else{ Assumo che sia TOP 
            int i;
            for(i=0;i<NPEDINE;i++){
                if(field->pedine[i].coord.x==field->pedine[index].coord.x&&
                field->pedine[i].coord.y==field->pedine[index].coord.y){
                    field->pedine[i].coord=mossa;
                }
            }
        } */
        for(i=0; i <NPEDINE; i++){
            if(field->pedine[i].coord.x==field->pedine[index].coord.x&&
                field->pedine[i].coord.y==field->pedine[index].coord.y) {
                field->pedine[i].coord.x = mossa.x;
                field->pedine[i].coord.y = mossa.y;
            }
        } 
        if(field->pedine[index].altezza==SINGLE){
            field->pedine[index].coord.x = mossa.x;
                field->pedine[index].coord.y = mossa.y;
        }else if(field->pedine[index].altezza==TOP){
            int controllo=-1;
        for(i=0; i <NPEDINE; i++){
            if(field->pedine[i].coord.x==field->pedine[index].coord.x&&
                field->pedine[i].coord.y==field->pedine[index].coord.y) {
                if(field->pedine[i].altezza==BOTTOM)controllo=i;
                field->pedine[i].coord.x = mossa.x;
                field->pedine[i].coord.y = mossa.y;
            }
            if(i==NPEDINE-1&&controllo==-1)i=0;
        } 
        while(field->pedine[controllo].coord.x==field->pedine[index].coord.x&&
                field->pedine[controllo].coord.y==field->pedine[index].coord.y){
                    field->pedine[controllo].coord.x = mossa.x;
                    field->pedine[controllo].coord.y = mossa.y;
                }

        }
        promossa(field,index);
        
    }
    
    if(debug)printf("Pedina in %d%d\n",field->pedine[index].coord.y,field->pedine[index].coord.x);
    if(debug)printf("Spazio in %d%d\n",field->blanks[indexb].coord.y,field->blanks[indexb].coord.x);
    
   if(debug)print_pedine(field);
} 


/**
 * @brief Date le cordinate mi restituisce un char corrispondente alla struttura della pedina in quela casella
 *        Funcion that given a set of coordinates gives back the char vector with the list of pawns for the print funcion
 * @param field 
 * @param cor 
 * @return char* 
 */
char* coord_to_pedina (field_t *field, coord_t cor){
    char *sol=malloc(sizeof(char)*20);
    int index=coord_to_index(field,cor);
    if(field->pedine[index].altezza==SINGLE)
        sol[1]=field->pedine[index].colore;
    else if(field->pedine[index].altezza==TOP){
       
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

/**
 * @brief Date le cordinate mi restituisce l'altezza della pedina 
 *        
 * @param field 
 * @param coord 
 * @return int 
 */
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


/*se il player sbaglia pedina o vuole cambiarla*/
void ops_sel() {

}
/**
 * @brief Date delle cordinate mi restituisce un char che serve per la print del campo di gioco
 *         
 * @param field 
 * @param x 
 * @param y 
 * @param ped 
 */
void coord_to_char(field_t *field, int x, int y, char *ped){
    int i;
    for(i=0;i<field->nblanks;i++){
        if (field->blanks[i].coord.x == x &&
            field->blanks[i].coord.y == y){
            ped[0]=' ';
            ped[1]=' ';
            ped[2]=' ';
            
        }
    }
    /* bool_t singola = FALSE; */
    /* for (i = 0; i < NPEDINE; i++){
        if (field->pedine[i].coord.x == x &&
            field->pedine[i].coord.y == y &&
            field->pedine[i].altezza == SINGLE){
            singola = TRUE;
            ped[0]=' ';
            ped[2]=' ';
            if (field->pedine[i].colore){
                if(field->pedine[i].promossa)
                ped[1] = 'B';
                else
                ped[1] = 'b';
            }else{
                if(field->pedine[i].promossa)
                ped[1] = 'N';
                else
                ped[1] = 'n';
            }
            break;
        }
    } */
    /* if (!singola){ */
        for (i = 0; i < NPEDINE; i++){
            if (field->pedine[i].coord.x == x &&
                field->pedine[i].coord.y == y){
                if(field->pedine[i].altezza == SINGLE){
                    ped[0]=' ';
                    ped[2]=' ';
                    if (field->pedine[i].colore){
                        if(field->pedine[i].promossa)
                            ped[1] = 'B';
                        else
                            ped[1] = 'b';
                    }else{
                        if(field->pedine[i].promossa)
                            ped[1] = 'N';
                        else
                            ped[1] = 'n';
                    } 
                    return;
                }else if (field->pedine[i].altezza == TOP){   
                    if(ped[1]=='|')ped[1] = ' ';/*Nel caso in cui non abbia già trovato una pedina al centro*/
                    if (field->pedine[i].colore){
                        ped[0] = 'b';
                    }else{
                        ped[0] = 'n';
                    }
                }
                else if (field->pedine[i].altezza == CENTER){
                    if (field->pedine[i].colore){
                        ped[1] = 'b';
                    }else{
                        ped[1] = 'n';
                    }
                }else{
                    if (field->pedine[i].colore){
                        ped[2] = 'b';
                    }else{
                        ped[2] = 'n';
                    }
                }
            }
        }
    /* } */
}
/*
typedef struct pair_int{
    int score;
    int indexb;
}pair_t;
*/
int n_pedine(field_t *field, enum color colore){
    int inizio,fine, i;
    int num=0;
    if(colore){
        inizio=0;
        fine=NPEDINE/2;
    }else{
        inizio=NPEDINE/2;
        fine=NPEDINE;
    }
    for(i=inizio;i<fine;i++){
        if((field->pedine[i].altezza==SINGLE||field->pedine[i].altezza==TOP)&&
        field->pedine[i].in_game)
        num++;
    }
    return num;
}
int n_promosse(field_t *field, enum color colore){
    int inizio,fine, i;
    int num=0;
    if(colore){
        inizio=0;
        fine=NPEDINE/2;
    }else{
        inizio=NPEDINE/2;
        fine=NPEDINE;
    }
    for(i=inizio;i<fine;i++){
        if((field->pedine[i].altezza==SINGLE||field->pedine[i].altezza==TOP)&&
        field->pedine[i].in_game&&field->pedine[i].promossa==YES)
        num++;
    }
    return num;
} 
pair_t cpu_mossa(field_t field,int index, int depth,enum color colore){
    pair_t res;
    pair_t sol[4];
    pair_t retval;
    int i,z,countersol=0,npedine1, npedineAvv1,npedine2,npedineAvv2,npromosse1, npromosse2,npromosseAvv1,npromosseAvv2,posy,inizio,fine;
    pedina_t* copiapedine = (pedina_t*)malloc(sizeof(pedina_t)*NPEDINE);
    blanks_t* copiablanks = (blanks_t*)malloc(sizeof(blanks_t)*field.nblanks);
    int nblanks=field.nblanks;
    res.score=0;
    /* printf("\n");
    printf("%d",depth);
    printf("\n"); */
    if (depth==0){
        res.score = 0;
        res.indexb = 0;
        return res;
    }
    for(i=0;i<NPEDINE;i++){
        copiapedine[i]=field.pedine[i];
        if(i<field.nblanks)
            copiablanks[i]=field.blanks[i];
    }
    npedine1=n_pedine(&field,BLACK);
    npedineAvv1=n_pedine(&field,WHITE);
    npromosse1=n_promosse(&field,BLACK);
    npromosseAvv1=n_promosse(&field,WHITE);
    if(!colore){
    inizio=0;
    fine=NPEDINE/2;
    posy=-1;
    }else{
    inizio=NPEDINE/2;
    fine=NPEDINE;
    posy=1;
    }
    if(field.pedine[index].is_obbligata){
	int j;
    
    
    for(i=inizio;i<fine;i++){
        /*Controllo solo le pedine che posso mangiare perchè sono o il TOP della torre o pedina singola*/
        if((field.pedine[i].altezza==SINGLE||field.pedine[i].altezza==TOP)&&field.pedine[i].colore!=field.pedine[index].colore){
            /*Controllo quale o quali pedina/e devo mangiare*/
            if(field.pedine[i].coord.y == field.pedine[index].coord.y+posy/*Deve essere nella riga successiva*/){
                if(field.pedine[i].coord.x==field.pedine[index].coord.x+1){/*Cerco in una diagonale*/
                    /*Cerco gli spazi di destinazione della mia pedina*/
                    for(j=0;j<field.nblanks;j++){
                        if(field.blanks[j].coord.y==field.pedine[index].coord.y+posy+posy &&
                        (field.blanks[j].coord.x==field.pedine[index].coord.x+2)){
                        /*TODO                        */
                        enum color turno;
                        spostamento_pedine(&field, colore, index, j);/*Una delle possibili mosse*/
                        if(!colore)turno=WHITE;
                        else turno=BLACK;
                        /* printf("\n");
                        printf(" mossa %d \n",turno);
                        printf("\n"); */
                        res=cpu_pedina(field,depth-1,turno);
                        sol[countersol].index=index;
                        sol[countersol].score=res.score;
                        sol[countersol].indexb=j;
                        npedine2=n_pedine(&field,BLACK);
                        npedineAvv2=n_pedine(&field,WHITE);
                        npromosse2=n_promosse(&field,BLACK);
                        npromosseAvv2=n_promosse(&field,WHITE);
                        /*Assegnamento del punteggio*/
                        if(npedine1<npedine2)sol[countersol].score++;
                            else if(npedine1>npedine2)sol[countersol].score--;
                        if(npedineAvv1>npedineAvv2)sol[countersol].score++;
                            else if(npedineAvv1<npedineAvv2)sol[countersol].score--;
                        if(npromosse1<npromosse2)sol[countersol].score++;
                            else if(npromosse1>npromosse2)sol[countersol].score--;
                        if(npromosseAvv1>npromosseAvv2)sol[countersol].score++;
                            else if(npromosseAvv1< npromosseAvv2)sol[countersol].score--;
                        countersol++;
                        field.nblanks=nblanks;
                        for(z=0;z<NPEDINE;z++){
                            field.pedine[z]=copiapedine[z];
                            if(z<field.nblanks)
                                field.blanks[z]=copiablanks[z];
                        }
                        
                        }
                    }
                }else if(field.pedine[i].coord.x==field.pedine[index].coord.x-1){/*Cerco nell'altra diagonale*/
                    /*Cerco gli spazi di destinazione della mia pedina*/
                    for(j=0;j<field.nblanks;j++){
                        if(field.blanks[j].coord.y==field.pedine[index].coord.y+posy+posy &&
                        (field.blanks[j].coord.x==field.pedine[index].coord.x-2)){
                        /*TODO              */
                        enum color turno;

                        spostamento_pedine(&field, colore, index, j);/*Una delle possibili mosse*/
                        if(!colore)turno=WHITE;
                        else turno=BLACK;
                        /* printf("\n");
                        printf(" mossa %d \n",turno);
                        printf("\n"); */
                        res=cpu_pedina(field,depth-1,turno);
                        sol[countersol].index=index;
                        sol[countersol].score=res.score;
                        sol[countersol].indexb=j;
                        npedine2=n_pedine(&field,BLACK);
                        npedineAvv2=n_pedine(&field,WHITE);
                        npromosse2=n_promosse(&field,BLACK);
                        npromosseAvv2=n_promosse(&field,WHITE);
                        /*Assegnamento del punteggio*/
                        if(npedine1<npedine2)sol[countersol].score++;
                            else if(npedine1>npedine2)sol[countersol].score--;
                        if(npedineAvv1>npedineAvv2)sol[countersol].score++;
                            else if(npedineAvv1<npedineAvv2)sol[countersol].score--;
                        if(npromosse1<npromosse2)sol[countersol].score++;
                            else if(npromosse1>npromosse2)sol[countersol].score--;
                        if(npromosseAvv1>npromosseAvv2)sol[countersol].score++;
                            else if(npromosseAvv1< npromosseAvv2)sol[countersol].score--;
                        countersol++;
                        field.nblanks=nblanks;
                        for(z=0;z<NPEDINE;z++){
                            field.pedine[z]=copiapedine[z];
                            if(z<field.nblanks)
                                field.blanks[z]=copiablanks[z];
                        }
                        
                        }
                    }
                }
            }else if(field.pedine[index].promossa){
                if(field.pedine[i].coord.y == field.pedine[index].coord.y-posy/*Deve essere nella riga successiva*/){
                if(field.pedine[i].coord.x==field.pedine[index].coord.x+1){/*Cerco in una diagonale*/
                    /*Cerco gli spazi di destinazione della mia pedina*/
                    for(j=0;j<field.nblanks;j++){
                        if(field.blanks[j].coord.y==field.pedine[index].coord.y-posy-posy &&
                        (field.blanks[j].coord.x==field.pedine[index].coord.x+2)){
                        /*TODO                        */
                        enum color turno;
                        spostamento_pedine(&field, colore, index, j);/*Una delle possibili mosse*/
                        if(!colore)turno=WHITE;
                        else turno=BLACK;
                        /* printf("\n");
                        printf(" mossa %d \n",turno);
                        printf("\n"); */
                        res=cpu_pedina(field,depth-1,turno);
                        sol[countersol].index=index;
                        sol[countersol].score=res.score;
                        sol[countersol].indexb=j;
                        npedine2=n_pedine(&field,BLACK);
                        npedineAvv2=n_pedine(&field,WHITE);
                        npromosse2=n_promosse(&field,BLACK);
                        npromosseAvv2=n_promosse(&field,WHITE);
                        /*Assegnamento del punteggio*/
                        if(npedine1<npedine2)sol[countersol].score++;
                            else if(npedine1>npedine2)sol[countersol].score--;
                        if(npedineAvv1>npedineAvv2)sol[countersol].score++;
                            else if(npedineAvv1<npedineAvv2)sol[countersol].score--;
                        if(npromosse1<npromosse2)sol[countersol].score++;
                            else if(npromosse1>npromosse2)sol[countersol].score--;
                        if(npromosseAvv1>npromosseAvv2)sol[countersol].score++;
                            else if(npromosseAvv1< npromosseAvv2)sol[countersol].score--;
                        countersol++;
                        field.nblanks=nblanks;
                        for(z=0;z<NPEDINE;z++){
                            field.pedine[z]=copiapedine[z];
                            if(z<field.nblanks)
                                field.blanks[z]=copiablanks[z];
                        }
                        
                        }
                    }
                }else if(field.pedine[i].coord.x==field.pedine[index].coord.x-1){/*Cerco nell'altra diagonale*/
                    /*Cerco gli spazi di destinazione della mia pedina*/
                    for(j=0;j<field.nblanks;j++){
                        if(field.blanks[j].coord.y==field.pedine[index].coord.y-posy-posy &&
                        (field.blanks[j].coord.x==field.pedine[index].coord.x-2)){
                        /*TODO              */
                        enum color turno;

                        spostamento_pedine(&field, colore, index, j);/*Una delle possibili mosse*/
                        if(!colore)turno=WHITE;
                        else turno=BLACK;
                        /* printf("\n");
                        printf(" mossa %d \n",turno);
                        printf("\n"); */
                        res=cpu_pedina(field,depth-1,turno);
                        sol[countersol].index=index;
                        sol[countersol].score=res.score;
                        sol[countersol].indexb=j;
                        npedine2=n_pedine(&field,BLACK);
                        npedineAvv2=n_pedine(&field,WHITE);
                        npromosse2=n_promosse(&field,BLACK);
                        npromosseAvv2=n_promosse(&field,WHITE);
                        /*Assegnamento del punteggio*/
                        if(npedine1<npedine2)sol[countersol].score++;
                            else if(npedine1>npedine2)sol[countersol].score--;
                        if(npedineAvv1>npedineAvv2)sol[countersol].score++;
                            else if(npedineAvv1<npedineAvv2)sol[countersol].score--;
                        if(npromosse1<npromosse2)sol[countersol].score++;
                            else if(npromosse1>npromosse2)sol[countersol].score--;
                        if(npromosseAvv1>npromosseAvv2)sol[countersol].score++;
                            else if(npromosseAvv1< npromosseAvv2)sol[countersol].score--;
                        countersol++;
                        field.nblanks=nblanks;
                        for(z=0;z<NPEDINE;z++){
                            field.pedine[z]=copiapedine[z];
                            if(z<field.nblanks)
                                field.blanks[z]=copiablanks[z];
                        }
                        
                        }
                    }
                }
            }
            }
        }
    }
    
    /*Se non ho mosse obbligatorie controllo le mosse di una casella in diagonale*/
    }else{
        for(i=0;i<field.nblanks;i++){
            if(field.blanks[i].coord.y==field.pedine[index].coord.y+posy&&(
                    field.blanks[i].coord.x==field.pedine[index].coord.x+1|| 
                    field.blanks[i].coord.x==field.pedine[index].coord.x-1)) {
                    /*TODO                        */
                    enum color turno;
                    spostamento_pedine(&field, colore, index, i);/*Una delle possibili mosse*/
                    if(!colore)turno=WHITE;
                    else turno=BLACK;
                    /* printf("\n");
                    printf(" mossa %d \n",turno);
                    printf("\n"); */
                    res=cpu_pedina(field,depth-1,turno);
                    sol[countersol].index=index;
                    sol[countersol].score=res.score;
                    sol[countersol].indexb=i;
                    npedine2=n_pedine(&field,BLACK);
                    npedineAvv2=n_pedine(&field,WHITE);
                    npromosse2=n_promosse(&field,BLACK);
                    npromosseAvv2=n_promosse(&field,WHITE);
                        /*Assegnamento del punteggio*/
                        if(npedine1<npedine2)sol[countersol].score++;
                            else if(npedine1>npedine2)sol[countersol].score--;
                        if(npedineAvv1>npedineAvv2)sol[countersol].score++;
                            else if(npedineAvv1<npedineAvv2)sol[countersol].score--;
                        if(npromosse1<npromosse2)sol[countersol].score++;
                            else if(npromosse1>npromosse2)sol[countersol].score--;
                        if(npromosseAvv1>npromosseAvv2)sol[countersol].score++;
                            else if(npromosseAvv1< npromosseAvv2)sol[countersol].score--;
                    countersol++;
                    field.nblanks=nblanks;
                    for(z=0;z<NPEDINE;z++){
                            field.pedine[z]=copiapedine[z];
                            if(z<field.nblanks)
                                field.blanks[z]=copiablanks[z];
                    }
                    
            }else if(field.pedine[index].promossa){
                if(field.blanks[i].coord.y==field.pedine[index].coord.y-posy&&(
                    field.blanks[i].coord.x==field.pedine[index].coord.x+1|| 
                    field.blanks[i].coord.x==field.pedine[index].coord.x-1)) {
                    /*TODO                        */
                    enum color turno;
                    spostamento_pedine(&field, colore, index, i);/*Una delle possibili mosse*/
                    if(!colore)turno=WHITE;
                    else turno=BLACK;
                    /* printf("\n");
                    printf(" mossa %d \n",turno);
                    printf("\n"); */
                    res=cpu_pedina(field,depth-1,turno);
                    sol[countersol].index=index;
                    sol[countersol].score=res.score;
                    sol[countersol].indexb=i;
                    npedine2=n_pedine(&field,BLACK);
                    npedineAvv2=n_pedine(&field,WHITE);
                    npromosse2=n_promosse(&field,BLACK);
                    npromosseAvv2=n_promosse(&field,WHITE);
                        /*Assegnamento del punteggio*/
                        if(npedine1<npedine2)sol[countersol].score++;
                            else if(npedine1>npedine2)sol[countersol].score--;
                        if(npedineAvv1>npedineAvv2)sol[countersol].score++;
                            else if(npedineAvv1<npedineAvv2)sol[countersol].score--;
                        if(npromosse1<npromosse2)sol[countersol].score++;
                            else if(npromosse1>npromosse2)sol[countersol].score--;
                        if(npromosseAvv1>npromosseAvv2)sol[countersol].score++;
                            else if(npromosseAvv1< npromosseAvv2)sol[countersol].score--;
                    countersol++;
                    field.nblanks=nblanks;
                    for(z=0;z<NPEDINE;z++){
                            field.pedine[z]=copiapedine[z];
                            if(z<field.nblanks)
                                field.blanks[z]=copiablanks[z];
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
    if(countersol==0)printf("Errore countersol\n");
    retval=sol[0];
    while(countersol>0){
        if(retval.score<sol[countersol].score)
            retval=sol[countersol];
        countersol--;
    }

    return retval;
}

pair_t cpu_pedina(field_t field,int depth,enum color colore){
    
    int i, k=0;
    int inizio,fine,massimo=0, indicemassimo=0;
    /* stampa_field(&field);
    printf("\n");
    printf("%d",depth);
    printf("\n"); */
    bool_t control=FALSE;
    
    pair_t *max=malloc(sizeof(pair_t)*NPEDINE);
    if (depth==0){
        pair_t res;
        res.score = 0;
        res.indexb = -1;
        res.index = -1;
        return res;
    }
    if(!colore){/*Colore nero*/
        inizio=NPEDINE/2;
        fine=NPEDINE;
    }else{
        inizio=0;
        fine=NPEDINE/2;
    }
    movable(colore,&field);
    /*Prima controllo se ho la possibilità di mangiare una pedina*/
    for(i=inizio;i<fine;i++){
        if(field.pedine[i].is_obbligata){
            max[k]=cpu_mossa(field, i,depth,colore);
            max[k].index=i;
            k++;
            control=TRUE;
        }
    }  
    /*Se non ho mosse obbligate controllo se posso muovere la pedina*/
    if(!control){
        for(i=inizio;i<fine;i++){
            if(field.pedine[i].is_movable){
                max[k]=cpu_mossa(field, i,depth,colore);
                max[k].index=i;
                k++;
            }
        }
    }
    if(k==0){
        pair_t res;
        printf("Errore max\n");
        res.score = -3;
        res.indexb = -1;
        res.index = -1;
        return res;

    }
    massimo=max[0].score;
    for(i=0;i<k;i++){
        if(max[i].score>massimo){
            massimo=max[i].score;
            indicemassimo = i;
        }
    }
    return max[indicemassimo];
    
}
pair_t cpu_turn(field_t *field){
    field_t  campo=*field;
    /* pedina_t* copia = field->pedine; */

    /*pedina_t* copiapedine = (pedina_t*)malloc(sizeof(pedina_t)*NPEDINE);
    blanks_t* copiablanks = (blanks_t*)malloc(sizeof(blanks_t)*field->nblanks);
    int i,z;
    for(i=0;i<NPEDINE;i++){
        copiapedine[i]=field->pedine[i];
        if(i<field->nblanks)
            copiablanks[i]=field->blanks[i];
            }
    pair_t sol=cpu_pedina(campo,10,BLACK);
    for(z=0;z<NPEDINE;z++){
        field->pedine[z]=copiapedine[z];
        if(z<field->nblanks)
            field->blanks[z]=copiablanks[z];
    }*/
    pair_t sol=cpu_pedina(campo,10,BLACK);
    /* field->pedine = copia; */
    return sol;
}

/**
 * @brief Funzione di stampa del campo
 * 
 * @param field 
 */
void stampa_field(field_t *field){
    int i, j;
    /* int x=1, y=1; */
    char *ped = malloc(3*sizeof(char));
    ped[0]='|';
    ped[1]='|';
    ped[2]='|';
    printf("  |  1  |  2  |  3  |  4  |  5  |  6  |  7  | \n");
    printf("  ------------------------------------------- \n");
    for (i = 0; i < NROWS; i++){
        /*Stampo il numero di riga e la spaziatura*/
        printf("  |");
        for (j=0; j < NCOLS; j++){
            coord_to_char(field, j+1, i+1, ped);
            if(ped[1]=='|'){
                printf("||||||");
            }else{
                printf("  %c  |",ped[0]);
            }
            ped[0]='|';
            ped[1]='|';
            ped[2]='|';
        }
        printf("\n");
        printf("%d |", i + 1);  
        for (j=0; j < NCOLS; j++){
            coord_to_char(field, j+1, i+1, ped);
            if(ped[1]=='|'){
                printf("||||||");
            }else{
            printf("  %c  |",ped[1]);
            }
            ped[0]='|';
            ped[1]='|';
            ped[2]='|';
        }
        
        printf("\n");
        printf("  |");
        for (j=0; j < NCOLS; j++){
            coord_to_char(field, j+1, i+1, ped);
            if(ped[1]=='|'){
                printf("||||||");
            }else{
            printf("  %c  |",ped[2]);
            }
            ped[0]='|';
            ped[1]='|';
            ped[2]='|';
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
void create_pedine(field_t *field){
    int i,j,c=0;
    field->pedine=malloc(sizeof(struct pedina)*NPEDINE);

    for(i=0;i<NPEDINE;i++){
        field->pedine[i].altezza=SINGLE;
        field->pedine[i].in_game=TRUE;
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

/**
 * @brief Create a blanks object
 *          Funzione che serve a creare un array con le posizioni degli spazi vuoti
 * @param field 
 */
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

/**
 * @brief funzione da eseguire sempre prima di chiudere il programma
 * 
 * @param field 
 */
void free_pedine(field_t *field){
    free(field->pedine);
    free(field->blanks);
}

/**
 * @brief Funzione di partenza
 * 
 * @param field 
 */
void start_game2(field_t *field){
    
    field->cols=NCOLS;
    field->rows=NROWS;
    create_pedine(field);
    field->nblanks=3;
    create_blanks(field);
    field->partita.END_OF_PLAY=FALSE;
    field->partita.VICTORY_P1=FALSE;
    
}
int main() {
    pair_t mossacpu;
    int selezione=0;
    field_t field;
    start_game2(&field);
    printf("1: 1vsCPU/n 2: 1vs1 /n");
    scanf("%d",&selezione);
    if(selezione){
        while(!field.partita.END_OF_PLAY){
            print_pedine(&field);
            stampa_field(&field);
            movable(BLACK,&field);
            if(field.partita.END_OF_PLAY)
                break;
            mossacpu =cpu_turn(&field);
            print_pedine(&field);
            spostamento_pedine(&field,BLACK,mossacpu.index,mossacpu.indexb);
            printf("Score %d Index %d Indexb %d \n",mossacpu.score,mossacpu.index,mossacpu.indexb);
            print_pedine(&field);
            stampa_field(&field);
            movable(WHITE,&field);
            if(field.partita.END_OF_PLAY)
                break;
            sel_pedina(WHITE,&field);
        }
    }
    if(selezione==2){
        while(!field.partita.END_OF_PLAY){
            print_pedine(&field);
        stampa_field(&field);
        movable(BLACK,&field);
        if(field.partita.END_OF_PLAY)
            break;
        sel_pedina(BLACK,&field);
        print_pedine(&field);
        stampa_field(&field);
        movable(WHITE,&field);
        sel_pedina(WHITE,&field);
        }
    }
    free_pedine(&field);
    return 0;
}