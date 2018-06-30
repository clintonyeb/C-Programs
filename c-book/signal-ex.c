#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static void catch_sig(int signal){
    printf("Signal caught %d\n", signal);
}

int main(){
    if(signal(SIGINT, catch_sig) == SIG_ERR){
        fputs("Error assigning handler", stderr);
    }

    puts("Raising interactive signal");

    if(raise(SIGINT) != 0){
        fputs("Error raising signal", stderr);
        return EXIT_FAILURE;
    }

    puts("Exiting...");
    exit(EXIT_SUCCESS);
}
