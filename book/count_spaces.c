#include <stdio.h>

int main(){
    int c, count = 0;
    FILE *fp = fopen("file.txt", "r");
    FILE *fpout = fopen("output.txt", "w");
    int prev = EOF;

    while((c = fgetc(fp)) != EOF) {
        if(c == ' ' && prev == ' ')
            continue;
        fputc(c, fpout);
        prev = c;
    }
        

    printf("Count: %d", count);
    fclose(fp);
    fclose(fpout);
    return 0;
}