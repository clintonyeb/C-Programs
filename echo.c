#include <stdio.h>

int main(int argc, char **argv){
    int i;
    for(i = 1; i < argc; i++){
        fprintf(stdout, "%s", argv[i]);
        if (i < argc - 1){
            fprintf(stdout, " ");
        }
    }
    if (argc > 1){
        fprintf(stdout, "\n");
    }
    return 0;
}
