
#include "array.h"
#include "../vec.h"

using namespace brim;

ArrayObj::ArrayObj(Vec<Value> v) {
    vec = v;
}

Value ArrayObj::toString() {
    return Value(this);
}