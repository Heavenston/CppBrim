#pragma once

#include <string>
#include "common.h"
#include "chunk.h"
#include "vec.h"
#include "value.h"

using namespace brim;

namespace brim {

    class VM {
        Vec<Value> stack;
        Chunk *chunk = nullptr;
        usize ip = 0;

    public:
        VM();

        void interpret(const char *program);
        void interpret_file(const char *path);

        void run();

    };

}