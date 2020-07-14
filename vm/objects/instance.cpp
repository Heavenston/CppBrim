
#include "../common.h"
#include "instance.h"

using namespace brim;

InstanceObj::InstanceObj() {}
InstanceObj::InstanceObj(ClassObj *a_parent): parent(a_parent) {}

const char *InstanceObj::to_string() {
    char *str = (char*)malloc(11+strlen(parent->get_name())+2);
    sprintf(str, "[Instance %s]", parent->get_name());
    return str;
}
