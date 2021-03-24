

#include "MiniLaska.h"

/*gcc MiniLaska.c -c -oMiniLaska.o -ansi -pedantic*/
/*gcc Game.c -oLaska -ansi -pedantic MiniLaska.o*/
int main() {
    
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
    stampa_field(&field);
    movable(BLACK,&field);
    cpu_turn(field);

    free_pedine(&field);
    return 0;
}