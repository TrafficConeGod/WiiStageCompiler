#pragma once
#include <vector>
#include <map>
#include <Property.h>

class Type {
    private:
        std::vector<Type*> parentTypes;
        size_t parentTypesOffset;
        std::map<std::string, Property*> properties;
    public:
        std::string name;
        size_t id;

        Type(size_t id, std::map<std::string, Type*>& types, const std::vector<std::string>& csvLine);

        size_t GetPropertiesSize();
        Property* GetProperty(const std::string& name);
};