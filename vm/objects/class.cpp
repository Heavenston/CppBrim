
#include "class.h"

using namespace brim;

ClassObj::ClassObj() {}
ClassObj::ClassObj(const char* a_name): name(a_name) {}

const char *ClassObj::get_name() {
    return name;
}

const char *ClassObj::to_string() {
    char *str = (char*)malloc(11+strlen(name)+2);
    sprintf(str, "[Class %s]", name);
    return str;
}