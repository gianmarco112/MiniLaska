/**
 * @file MiniLaska.h
 * @author Gianmarco Perini (883492@stud.unive.it)
 * @brief
 * @version 2.0
 * @date 20-05-2021
 *
 * @copyright Copyright (c) 2021
 *
 */



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
    bool_t promossa;
    bool_t is_movable;
    bool_t is_obbligata;
    bool_t in_game;
}pedina_t;

typedef struct field{
    int rows, cols;
    struct pedina* pedine;
    struct blanks* blanks;
    unsigned char nblanks;
    bool_t END_OF_PLAY;
}field_t;


/*LISTA DELLE FUNZIONI*/
/*Funzioni di controllo*/
void movable(enum color colore, field_t* field);
void obbligata(field_t* field, int index, enum color colore);
void promossa(field_t* field, int index);
/*Pedine*/
void sel_pedina2(enum color colore, field_t* field, vect* soluzione);
void possible_moves2(enum color colore, field_t* field, int index, vect* soluzione);
void spostamento_pedine(field_t* field, enum color colore, int index, int indexb);
/*Player*/
void pedina_player(field_t* field, enum color colore);
int mossa_player(field_t* field, enum color colore, int index);
/*CPU*/
triple_t turn_cpu(field_t* field);
triple_t pedina_cpu(field_t field, enum color colore, int depth);
triple_t mossa_cpu(field_t field, enum color colore, int index, int depth);
/*Score*/
int n_promosse(field_t* field, enum color colore);
int n_pedine(field_t* field, enum color colore);
/*Funzioni di stampa*/
void stampa_field(field_t* field);
void coord_to_char(field_t* field, int x, int y, char* ped);
/*Funzioni di inizio/fine game*/
void start_game(field_t* field);
void create_pedine(field_t* field);
void create_blanks(field_t* field);
void free_pedine(field_t* field);
void endgame(int giocatore);
/*FINE LISTA DELLE FUNZIONI*/