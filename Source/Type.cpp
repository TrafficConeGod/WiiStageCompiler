#include "Type.h"

Type* Type::Create(std::string name, void (*_Save)(DataStream& stream, std::string str)) {
    Type* type = new Type;
    type->Save = _Save;
    types[name] = type;
    return type;
}