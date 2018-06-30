#include <stdio.h>
#include <stdlib.h>

int main(){
    static const char *name = "Clinton Yeboah";
    printf("Size of name is: %d\n", sizeof(name));
    exit(EXIT_SUCCESS);
}
