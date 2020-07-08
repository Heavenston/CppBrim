#pragma once

#include "common.h"
#include "chunk.h"
#include "vec.h"
#include "scanner.h"

namespace brim {

    Chunk compile(const Vec<Token> &tokens);

}