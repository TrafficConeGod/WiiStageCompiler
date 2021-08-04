#pragma once
#include <vector>
#include <map>
#include "Property.h"

class UserType {
    private:
        std::vector<UserType*> parentUserTypes;
        size_t parentUserTypesOffset;
        std::map<std::string, Property*> properties;
    public:
        std::string name;
        size_t id;

        UserType(size_t id, std::map<std::string, UserType*>& userTypes, const std::vector<std::string>& csvLine);

        size_t GetPropertiesSize();
        Property* GetProperty(const std::string& name);
};