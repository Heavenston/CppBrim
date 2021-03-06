
#include <string>

#include "common.h"
#include "value.h"

using namespace brim;

Value::Value() {
    type = ValueType::Null;
}
Value::Value(f64 v) {
    type = ValueType::Number;
    payload.number = v;
}
Value::Value(bool v) {
    type = ValueType::Null;
    payload.boolean = v;
}
Value::Value(Object *obj) {
    type = ValueType::Null;
    payload.object = obj;
}

bool Value::is_type(ValueType is) {
    return type == is;
}

f64 Value::number_val() {
    return payload.number;
}
bool Value::boolean_val() {
    return payload.boolean;
}

const char* Value::to_string() {
    switch (type) {
        case ValueType::Object:
            return payload.object->to_string();
        break;
        case ValueType::Boolean:
            if (payload.boolean) {
                return "true";
            }
            else {
                return "false";
            }
        break;
        case ValueType::Null:
            return "null";
        break;
        case ValueType::Number:
            return std::to_string(payload.number).c_str();
        break;
    }
}