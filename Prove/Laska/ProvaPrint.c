#include <stdio.h>
#include <stdlib.h>
#define NPEDINE (22)
#define NROWS (7)
#define NCOLS (7)

typedef struct field field_t;



typedef enum bool{
    true=1,
    false=0
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


struct blanks{
    coord_t coord;
};

typedef struct pedina{
    enum color colore;
    struct coord coord;
    enum height altezza;
    enum is_promoted promossa;
    bool_t is_movable;
    bool_t is_obbligata;
    bool_t in_game;
}pedina_t;

void coord_to_char(field_t *field,x,y,char *ped){
  int i;
  bool singola=false;
  for(i=0;i<NPEDINE;i++){
    if(field->pedine[i].coord.x==x&&
      field->pedine[i].coord.y==y&&
      field->pedine[i].altezza==SINGLE){
        singola=true;
      }
  }
  if(!singola){
    for(i=0;i<NPEDINE;i++){
      if(field->pedine[i].coord.x==x&&
        field->pedine[i].coord.y==y){
          if(field->pedine[i].altezza==TOP){
            if(field->pedine[i].colore){
              ped[0]='w';
            }else{
              ped[0]='b';
            }
          }else if(field->pedine[i].altezza==CENTER){
            if(field->pedine[i].colore){
              ped[1]='w';
            }else{
              ped[1]='b';
            }
          }else{
            if(field->pedine[i].colore){
              ped[2]='w';
            }else{
              ped[2]='b';
            }
          }
        }
    }
  }

}

stampa_field(field_t *field){
  int i,j;
  int x=1,y=1;
  char ped[3]={   };
  printf("    1 2 3 4 5 6 7   \n");
  for(i=0;i<NROWS;i++){
      /*Stampo il numero di riga e la spaziatura*/
      if(i%2==0||i==0) {
          j = 3;
          printf("%d |",i+1);
      }else {
          j = 4;
          printf("%d   |",i+1);
    for(;j>NCOLS:j++){

      }
    }
  }
}
/*Un prototipo di come dovrebbe essere la print*/
void print_fieldTEST(){
    int i,j;
    printf("    1 2 3 4 5 6 7   \n");
    for(i=0;i<7;i++){
        /*printf("  --------------\n");*/
        if(i%2==0||i==0) {
            j = 3;
            printf("%d |",i+1);
        }else {
            j = 4;
            printf("%d   |",i+1);
        }
        for(;j<7;j++){

            if(j%2==0)
            printf(" w |");
            else
            printf(" b |");
        }
        printf("\n");
    }
}



int main(){
  return 0;
}
