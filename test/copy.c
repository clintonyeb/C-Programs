#include <stdio.h>

int main(){
  char t[20] = "clinton";
  char s[20] = "";

  while(*s++ = *t++);

  printf("%s", s);
  return 0;
}
