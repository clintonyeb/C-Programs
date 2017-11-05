#include <iostream>
#include <cstdlib>

int main(){
    int* p = (int *) std::malloc(4 * sizeof(int));

    for (int i = 0; i < 4; ++i) {
        p[i] = i * i;
    }

    for (int i = 0; i < 4; ++i) {
        std::cout << p[i] << " ";
    }
    std::cout << std::endl;
    std::free(p);

    return 0;
}
