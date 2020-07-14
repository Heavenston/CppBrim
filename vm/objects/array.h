#pragma once

#include "object.h"
#include "../vec.h"
#include "../value.h"

namespace brim {

    class ArrayObj: public Object {
    private:
        ObjectType type = ObjectType::Array;
        Vec<Value> vec;

    public:
        ArrayObj(Vec<Value> v);

        const char *to_string();

    };

}