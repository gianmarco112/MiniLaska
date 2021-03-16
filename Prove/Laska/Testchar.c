#include <stdio.h>
#include <stdlib.h>

int main(){
  char *ped=malloc(3*sizeof(char));
  ped[1]='w';
  ped[2]='w';
  ped[0]='w';
  printf("%s\n",ped);
  return 0;
}
