
#include "array.h"
#include "../vec.h"
#include "../value.h"

using namespace brim;

ArrayObj::ArrayObj(Vec<Value> v) {
    vec = v;
}
