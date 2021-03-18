#include <stdio.h>
#include <stdlib.h>
#define NPEDINE (22)
#define NROWS (7)
#define NCOLS (7)

typedef struct field field_t;

typedef enum bools bool_t;

enum bools{
    TRUE=1,
    FALSE=0
};


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
/**
 * @brief 
 * @struct 
 */
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
void stampa_field(field_t *field);
void coord_to_char(field_t *field, int x, int y, char *ped);
void endgame(field_t *field);
void start_game2(field_t *field);
/*FINE LISTA DELLE FUNZIONI*/