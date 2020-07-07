#pragma once

#include <string>
#include "common.h"
#include "chunk.h"
#include "vec.h"

using namespace brim;

namespace brim {

    class VM {
        Vec<Value> stack;
        Chunk *chunk = nullptr;
        usize ip = 0;

    public:
        VM();

        void interpret(char *program);
        void interpret_file(char *path);

        Chunk *compile(char *program);
        void run();

    };

}