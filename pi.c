#include "pi.h"

char* getPi(int quantity){
  char* pi = (char*)malloc(sizeof(char)*quantity+3);
  //quantity+3 pois consideramos '3' ',' e '\n'
  sprintf(pi,"%d",quantity);
  pi[quantity+3] = '\n';
  return pi;
}
