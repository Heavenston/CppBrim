#pragma once
#include "brim.h"

namespace brim {

    enum class OpCode: u8 {
        Add,
        Mul,
        Div,
        Sub,
        Pow,
        Mod,

        Neg,
        Not,

        And,
        Or,

        Return,

        Number,
        String,
        Null,
        True,
        False,
    }

}