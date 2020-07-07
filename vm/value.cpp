
#include "brim.h"
#include "value.h"

using namespace brim;

Value Value::create_null() {
    Value v;
    v.type = ValueType::Null;
    return v;
}
Value Value::create_number(f64 as) {
    Value v;
    v.type = ValueType::Number;
    v.as.number = as;
    return v;
}
Value Value::create_null(bool as) {
    Value v;
    v.type = ValueType::Null;
    v.as.boolean = as;
    return v;
}

bool Value::is_type(ValueType is) {
    return type == is;
}

f64 Value::number_val() {
    return as.number;
}
bool Value::boolean_val() {
    return as.boolean;
}