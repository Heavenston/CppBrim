#include <iostream>
#include <brim.h>

using namespace brim;
using namespace std;

int main() {
    Vec<u8> vec;

    for (u8 i = 0; i <= 254; i++) {
       vec.push(i);
    }

    return 0;
}