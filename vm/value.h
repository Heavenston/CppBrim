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
        union As {
            f64 number;
            bool boolean;
        } as;

    public:
        static Value create_null();
        static Value create_number(f64 as);
        static Value create_null(bool as);
        
        bool is_type(ValueType is);

        f64 number_val();
        bool boolean_val();
    };

}
