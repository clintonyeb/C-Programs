#include <iostream>
#include <cstdlib>

int main(){
    int i, d;
    while ( i < 5){
        d = rand() % 5;
        std::cout << d << "  ";
        i++;
    }
    return 0;
}
