#include <iostream>

using namespace std;

void display(int* door){
    for (int i = 0; i < 100; ++i) {
        cout << "Door: " << i + 1 << ": " <<  (door[i] ? "Opened" : "Closed") << endl;
    }
    cout << endl;
}

int main(){
    int doors[100] = {0};
    //display(doors);
    for (int i = 0; i < 100; ++i) {
        for (int j = i; j < 100; j = j + i + 1) {
            doors[j] ^= 1;
        }
    }
    display(doors);
    return 0;
}
