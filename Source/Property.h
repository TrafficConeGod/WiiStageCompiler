#pragma once
#include "Type.h"

struct Property {
    Type* type = nullptr;
    size_t id = 0;
    std::string defaultValue = "0";
};