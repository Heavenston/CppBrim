
#include <cstring>

#include "../common.h"
#include "../value.h"
#include "string.h"

using namespace brim;

StringObj::StringObj(const char* v) {
    usize len = strlen(v);
    value = (char*)malloc(len);
    memcpy(value, v, len);
}
StringObj::StringObj(std::string v) {
    value = (char*)malloc(v.length());
    memcpy(value, v.c_str(), v.length());
}


Value StringObj::toString() {
    return Value(this);
}