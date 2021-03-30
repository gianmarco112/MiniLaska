
#include <stdio.h>
#include "MiniLaska.h"
#include <stdlib.h>
/*gcc MiniLaska.c -c -oMiniLaska.o -ansi -pedantic*/
/*gcc Game.c -oLaska -ansi -pedantic MiniLaska.o*/
/*int main() {
    pair_t mossacpu;
    /* field_t *field=(field_t*)malloc(sizeof (field_t)); */
    field_t field;
    start_game2(&field);
    
    /* while(!field.partita.END_OF_PLAY){
        stampa_field(&field);
        movable(WHITE,&field);
        if(field.partita.END_OF_PLAY)
            break;
        sel_pedina(WHITE,&field);
        stampa_field(&field);
        movable(BLACK,&field);
        sel_pedina(BLACK,&field); 
    } */
    /* while(!field.partita.END_OF_PLAY){
        stampa_field(&field);
        movable(WHITE,&field);
        if(field.partita.END_OF_PLAY)
            break;
        sel_pedina(WHITE,&field);
        stampa_field(&field);
        movable(BLACK,&field);
        cpu_turn(field);
        
    }*/  
    while(!field.partita.END_OF_PLAY){
    stampa_field(&field);
    mossacpu =cpu_turn(&field);
    spostamento_pedine(&field,BLACK,mossacpu.index,mossacpu.indexb);
    printf("Score %d Index %d Indexb %d \n",mossacpu.score,mossacpu.index,mossacpu.indexb);
    stampa_field(&field);
    movable(WHITE,&field);
    if(field.partita.END_OF_PLAY)
        break;
    sel_pedina(WHITE,&field);
    }
    free_pedine(&field);
    return 0;
}*/
