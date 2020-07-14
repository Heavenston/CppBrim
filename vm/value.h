#pragma once

#include "common.h"
#include "objects/object.h"

namespace brim {

    enum class ValueType {
        Number,
        Boolean,
        Null,
        Object,
    };

    class Value {
    private:
        ValueType type;
        union Payload {
            f64 number;
            bool boolean;
            Object *object;
        } payload;

    public:
        Value();
        Value(f64 as);
        Value(bool as);
        Value(Object *obj);
        
        bool is_type(ValueType is);

        f64 number_val();
        bool boolean_val();

        const char* to_string();
    };

}
