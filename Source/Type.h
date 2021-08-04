#pragma once
#include <vector>
#include <map>

class Type {
    private:
        std::vector<Type*> parentTypes;
        std::map<std::string, ushort> properties;
    public:
        std::string name;
        ushort id;

        Type(ushort id, std::map<std::string, Type*>& types, const std::vector<std::string>& csvLine);

        ushort GetPropertyId(std::string name);
};