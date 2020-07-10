#pragma once

#include "object.h"

namespace brim {

    class InstanceObj: public Object {
    private:
        ObjectType type = ObjectType::Array;

    public:
        InstanceObj();
        
    };

}