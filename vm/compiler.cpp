
#include "compiler.h"

using namespace brim;

struct State {
    const Vec<Token> &tokens;
    usize current;

    Chunk *chunk;

    const Token &current_token() const {
        return tokens[current];
    }
    const Token &next() {
        return tokens[current++];
    }
};

Chunk *compile(const Vec<Token> &tokens) {
    State state {
        tokens: tokens,
        current: 0,
        chunk: new Chunk()
    };


    return state.chunk;
}