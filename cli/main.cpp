#include <iostream>
#include <string>
#include <brim.h>

using namespace brim;
using namespace std;

int main() {
    
    auto tokens = scan("5+5/5");
    Chunk *chunk = compile(tokens);
    chunk->debug();
    
    return 0;
}