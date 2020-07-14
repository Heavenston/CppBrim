#pragma once

#include <string>

#include "../common.h"
#include "object.h"

namespace brim {

    class StringObj: public Object {
    private:
        ObjectType type = ObjectType::String;
        char* value;
        usize length;

    public:
        StringObj(const char* v);
        StringObj(std::string v);

        const char *to_string();
    };

}