/**
 * @file Game.c
 * @author Gianmarco Perini (883492@stud.unive.it)
 * @brief
 * @version 2
 * @date 27-05-212021
 *
 * @copyright Copyright (c) 2021
 *
 */
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
        printf("\n\nModalità 1 vs CPU selezionata\n\n\n");
        while (!field.END_OF_PLAY){
            triple_t mossacpu;
            mossacpu.index = -1;
            mossacpu.indexb = -1;
            mossacpu.score = 0;
            stampa_field(&field);
            mossacpu = turn_cpu(&field);
            if (field.END_OF_PLAY){
                endgame(1);
                break;
            }
            printf("Score %d Index %d Indexb %d x: %d y: %d\n", mossacpu.score, mossacpu.index, mossacpu.indexb, field.blanks[mossacpu.indexb].coord.x, field.blanks[mossacpu.indexb].coord.y);
            stampa_field(&field);
            pedina_player(&field, WHITE);
            if (field.END_OF_PLAY){
                endgame(0);
                break;
            }
        }
    }
    if (selezione == 2){
        while (!field.END_OF_PLAY){
            stampa_field(&field);
            pedina_player(&field, BLACK);
            if (field.END_OF_PLAY){
                endgame(2);
                break;
            }
            stampa_field(&field);
            pedina_player(&field, WHITE);
            if (field.END_OF_PLAY){
                endgame(1);
                break;
            }
        }
    }
    printf("Grazie di aver giocato\n");
    free_pedine(&field);
    return 0;
}
