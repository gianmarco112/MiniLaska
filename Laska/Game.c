
#include <stdio.h>
#include "MiniLaska.h"
#include <stdlib.h>
/*gcc MiniLaska.c -c -oMiniLaska.o -ansi -pedantic*/
/*gcc Game.c -oLaska -ansi -pedantic MiniLaska.o*/
/**
 * @brief
 *
 * @return int
 */
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
            mossacpu = cpu_turn(&field);
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
