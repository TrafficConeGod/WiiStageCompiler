#pragma once
#include "DataStream.h"
#include <string>
#include <map>

class Type {
    private:
    public:
        static Type* Create(std::string name, void (*Save)(DataStream& stream, std::string str));

        void (*Save)(DataStream& stream, std::string str);
};

extern std::map<std::string, Type*> types;