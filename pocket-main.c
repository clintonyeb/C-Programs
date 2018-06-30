#include <stdio.h>

int main(){
    //printf("%25sPOCKET-DB\n%24s+---------+","","");
    const char* s = "Hello";
    printf("\t[%10s]\n\t[%-10s]\n\t[%*s]\n\t[%-10.*s]\n\t[%-*.*s]\n",
                s, s, 10, s, 4, s, 10, 4, s);
    return 0;
}
