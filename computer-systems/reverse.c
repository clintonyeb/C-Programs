#include <stdio.h>

void swap_inplace(int* x, int* y){
  *y = *x ^ *y;
  *x = *x ^ *y;
  *y = *x ^ *y;
}

void reverse_array(int* a, int len){
  int f, l;
  for(f = 0, l = len -1; f < l; f++, l--){
    swap_inplace(&a[f], &a[l]);
  }
}

void print_array(int* a, int len){
  int i;
  for(i = 0; i < len; i++){
    printf(" %d", a[i]);
  }
  printf("\n");
}

int main(){
  int a[] = {1, 2, 3, 4, 5, 6, 7};
  int len = sizeof(a)/sizeof(int);
  print_array(a, len);
  reverse_array(a, len);
  print_array(a, len);
  return 0;
}