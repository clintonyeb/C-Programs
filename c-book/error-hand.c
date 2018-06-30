#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

extern int errno;

int main(){
    char* p = malloc(100000000000UL);

    if(p == NULL) {
        puts("malloc failed");
        puts(strerror(errno));
    } else {
        free(p);
    }

    exit(EXIT_SUCCESS);
}
