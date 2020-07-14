#pragma once

#include "object.h"
#include "class.h"

namespace brim {

    class InstanceObj: public Object {
    private:
        ObjectType type = ObjectType::Array;
        ClassObj *parent = nullptr;

    public:
        InstanceObj();
        InstanceObj(ClassObj *a_parent);

        const char *to_string();
        
    };

}