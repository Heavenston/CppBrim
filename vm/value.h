#pragma once

#include "common.h"

namespace brim {

    enum class ValueType {
        Number,
        Boolean,
        Null,
    };

    class Value {
    private:
        ValueType type;
        union Payload {
            f64 number;
            bool boolean;
        } payload;

    public:
        Value();
        Value(f64 as);
        Value(bool as);
        
        bool is_type(ValueType is);

        f64 number_val();
        bool boolean_val();
    };

}
