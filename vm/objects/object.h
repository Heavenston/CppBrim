#pragma once

#include "../common.h"
#include "../value.h"
#include "string.h"

namespace brim {
    
    class Value;

    enum class ObjectType: u8 {
        String,
        Array,
        Class,
        Instance,
    };

    class Object {
    private:
        ObjectType type;

    public:
        virtual Value toString() {
            return Value(new StringObj("[Object]"));
        }

    };

}