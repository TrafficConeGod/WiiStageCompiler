#pragma once
#include "DataStream.h"
#include <string>
#include <map>
#include <functional>

class Type {
    private:
    public:
        static Type* Create(std::string name, void (*Save)(DataStream& stream, std::string str));

        std::function<void(DataStream& stream, std::string str)> Save;
};

extern std::map<std::string, Type*> types;