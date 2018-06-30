#include <stdio.h>

int equal(int x, int y){
  return !!(x & y);
}

int main(){
  int a = 4, b = 1;
  printf("%d", equal(a, b));
  return 0;
}