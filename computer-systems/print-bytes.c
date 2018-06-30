#include <stdio.h>
#include <string.h>

typedef unsigned char* bytes_pointer;

void show_bytes(bytes_pointer start, size_t size){
    int i;
    for(i = 0; i < size; i++){
        printf(" %.2x", start[i]);
    }
    printf("\n");
}

void show_int(int x) {
    show_bytes((bytes_pointer) &x, sizeof(int));
}

void show_float(float x) {
    show_bytes((bytes_pointer) &x, sizeof(float));
}

void show_double(double x) {
    show_bytes((bytes_pointer) &x, sizeof(double));
}

void show_pointer(void* x) {
    show_bytes((bytes_pointer) &x, sizeof(void *));
}


int main(){
    int i = 0x12345678;
    bytes_pointer ap = (bytes_pointer) &i;
    const char * m = "clintona";
    show_bytes((bytes_pointer) m, strlen(m));
    // show_bytes(ap, 2);
    // show_bytes(ap, 3);
    return 0;
}
