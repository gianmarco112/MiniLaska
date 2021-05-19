
typedef struct field field_t;

typedef enum bools bool_t;

enum bools{
    TRUE = 1,
    FALSE = 0
};


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
    int rows, cols;
    struct pedina* pedine;
    struct blanks* blanks;
    unsigned char nblanks;
    struct play partita;
};
typedef struct triple_int{
    int score;
    int index;
    int indexb;
}triple_t;

/*LISTA DELLE FUNZIONI*/
void pedina_player(field_t* field, enum color colore);
void movable(enum color colore, field_t* field);
void stampa_field(field_t* field);
void endgame(field_t* field);
void start_game(field_t* field);
void free_pedine(field_t* field);
void spostamento_pedine(field_t* field, enum color colore, int index, int indexb);
triple_t cpu_turn(field_t* field);
/*FINE LISTA DELLE FUNZIONI*/
