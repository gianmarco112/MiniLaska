
#include <stdio.h>
#include "MiniLaska.h"
/*gcc MiniLaska.c -c -oMiniLaska.o -ansi -pedantic*/
/*gcc Game.c -oLaska -ansi -pedantic MiniLaska.o*/

int main(){
    int selezione = 0;
    field_t field;
    start_game(&field);
    printf("Scegli la modalità di gioco:\n1: 1vsCPU\n2: 1vs1 \n");
    scanf("%d", &selezione);

    if (selezione == 1){
        printf("Modalità 1 vs CPU selezionata\n");
        while (!field.END_OF_PLAY){
            triple_t mossacpu;
            mossacpu.index = -1;
            mossacpu.indexb = -1;
            mossacpu.score = 0;
            stampa_field(&field);
            movable(BLACK, &field);
            if (field.END_OF_PLAY){
                /* printf("Vince il giocatore\n"); */
                endgame(1);
                break;
            }
            mossacpu = turn_cpu(&field);
            printf("Score %d Index %d Indexb %d x: %d y: %d\n", mossacpu.score, mossacpu.index, mossacpu.indexb, field.blanks[mossacpu.indexb].coord.x, field.blanks[mossacpu.indexb].coord.y);
            spostamento_pedine(&field, BLACK, mossacpu.index, mossacpu.indexb);
            stampa_field(&field);
            movable(WHITE, &field);
            if (field.END_OF_PLAY){
                /* printf("Vince il computer\n"); */
                endgame(0);
                break;
            }
            pedina_player(&field, WHITE);
        }
    }
    if (selezione == 2){
        while (!field.END_OF_PLAY){
            print_pedine(&field);
            stampa_field(&field);
            movable(BLACK, &field);
            if (field.END_OF_PLAY){
                endgame(2);
                break;
            }
            pedina_player(&field, BLACK);
            print_pedine(&field);
            stampa_field(&field);
            movable(WHITE, &field);
            if (field.END_OF_PLAY){
                endgame(1);
                break;
            }
            pedina_player(&field, WHITE);
        }
    }
    printf("Grazie di aver giocato\n");
    free_pedine(&field);
    return 0;
}
