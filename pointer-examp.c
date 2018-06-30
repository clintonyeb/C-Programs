#include <stdio.h>

int main(){
  int a = 4;
  int b = (*(volatile int *)  &a);
  printf("Output: %i\n", b);
  b = 10;
  printf("Input: %i\n", a);
  printf("Output: %i\n", b);
  return 0;
}
