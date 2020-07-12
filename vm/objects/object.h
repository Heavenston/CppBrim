#pragma once

#include "../common.h"

namespace brim {
    
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
        const char *toString() {
            return "[Object]";
        }

    };

}