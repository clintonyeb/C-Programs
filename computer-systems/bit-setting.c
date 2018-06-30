#include <stdio.h>

int main(){
  int x = 0x87654321;
  printf("%.8x\n", x);
  printf("1. %.8x\n", x & 0xFF);
  printf("2. %.8x\n", x ^ ~0xFF);
  printf("3. %.8x\n", x | 0xFF);
  return 0;
}