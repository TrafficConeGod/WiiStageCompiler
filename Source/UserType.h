#pragma once
#include <vector>
#include <map>
#include "Property.h"

class UserType {
    private:
        std::vector<UserType*> parentUserTypes;
        size_t parentUserTypesOffset = 0;
        std::map<std::string, Property*> properties;

        void CreateEnumProperty(Property* property, std::string section);
        void CreateArrayProperty(Property* property, std::string typeName, std::string propertyName);
    public:
        std::string name;
        size_t id = 0;

        UserType(size_t id, std::map<std::string, UserType*>& userTypes, const std::vector<std::string>& csvLine);

        size_t GetPropertiesSize();
        Property* GetProperty(const std::string& name);
};

extern std::map<std::string, size_t> labelNames;