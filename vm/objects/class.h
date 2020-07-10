#pragma once

#include <unordered_map>
#include <string>

#include "object.h"
#include "../vec.h"

namespace brim {

    class ClassObj: public Object {
    private:
        ObjectType type = ObjectType::Class;
        std::unordered_map<std::string, Value> methods;

    public:
        Value toString();
        
    };

}