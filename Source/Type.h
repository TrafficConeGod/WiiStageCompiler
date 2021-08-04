#pragma once
#include <vector>
#include <map>

class Type {
    private:
        std::vector<Type*> parentTypes;
        size_t parentTypesOffset;
        std::map<std::string, size_t> properties;
    public:
        std::string name;
        size_t id;

        Type(size_t id, std::map<std::string, Type*>& types, const std::vector<std::string>& csvLine);

        size_t GetPropertiesSize();
        int GetPropertyId(const std::string& name);
};