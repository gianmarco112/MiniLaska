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

/**
 * @brief Boolean type
 *
 */
typedef enum bool{
    TRUE = 1,
    FALSE = 0
}bool_t;

/**
 * @brief Type for BLACK and WHITE color
 *
 */
enum color{
    BLACK = 0,
    WHITE = 1
};

/**
 * @brief Height of pedina
 *
 */
enum height{
    SINGLE, TOP, BOTTOM, CENTER
};

/**
 * @brief Coord type
 *
 */
typedef struct coord{
    int x, y;
}coord_t;

/**
 * @brief 3 int for the cpu recursion
 *
 */
typedef struct triple_int{
    int score;
    int index;
    int indexb;
}triple_t;

/**
 * @brief Array with size and a bool
 *
 */
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
/**
 * @brief Function that initialize the field you pass by pointer
 *
 * @param field
 */
void start_game(field_t* field);
/**
 * @brief It prints the field
 *
 * @param field
 */
void stampa_field(field_t* field);
/**
 * @brief
 *
 * @param field
 * @param colore
 */
void pedina_player(field_t* field, enum color colore);
/**
 * @brief
 *
 * @param field
 * @return triple_t
 */
triple_t turn_cpu(field_t* field);
/**
 * @brief
 *
 * @param field
 */
void free_pedine(field_t* field);
/**
 * @brief
 *
 * @param giocatore
 */
void endgame(int giocatore);
/*FINE LISTA DELLE FUNZIONI*/