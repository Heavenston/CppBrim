#pragma once

#include <unordered_map>
#include <string>

#include "object.h"
#include "../vec.h"
#include "../value.h"

namespace brim {

    class ClassObj: public Object {
    private:
        ObjectType type = ObjectType::Class;
        std::unordered_map<std::string, Value> methods;

        const char* name = ""; 

    public:
        ClassObj();
        ClassObj(const char* a_name);
        const char *get_name();

        const char *to_string();
    };

}