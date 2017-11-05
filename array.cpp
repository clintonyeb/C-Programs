#include <cstdlib>
#include <new>
#include <cassert>

using namespace std;

class Array{
    char* p;
public:
    Array(size_t initial_size = 0): p(nullptr) {
        resize(initial_size);
    }

    void resize(size_t new_size) {
        if (new_size == 0) {
            free(p);
            p = nullptr;
        } else {
            if (void* mem = realloc(p, new_size)){
                p = static_cast<char *> (mem);
            } else {
                throw bad_alloc();
            }
        }
    }

    ~Array(){
        free(p);
    }

    char& operator[](size_t i){ return p[i];}
    char operator[](size_t i) const { return p[i];}
};

int main(){
    Array arr(50);
    arr[20] = 'c';
    arr.resize(30);
    assert(arr[20] == 'c');
    arr.resize(100);
    assert(arr[20] == 'c');
}
