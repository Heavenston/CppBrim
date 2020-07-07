#pragma once

#include <string>

#include "chunk.h"
#include "vec.h"
#include "value.h"

namespace brim {

    class VM {
        brim::Vec<brim::Value> stack;
        brim::Chunk *chunk = nullptr;
        usize ip = 0;

    public:
        VM();

        void interpret(char *program);
        void interpret_file(char *path);

        brim::Chunk *compile(char *program);
        void run();

    };

}