#pragma once
#include <vector>
#include <map>

class Type {
    private:
        std::vector<Type*> parentTypes;
        std::map<std::string, ushort> properties;
    public:
        ushort id;

        Type(std::vector<std::string> csvLine);

        ushort GetPropertyId(std::string name);
};