#include "UserType.h"
#include "Type.h"

void UserType::CreateEnumProperty(Property* property, std::string section) {
    std::map<std::string, size_t> values;
    size_t valueStartPos = 5;
    size_t currentValueId = 0;
    for (size_t i = 5; i < section.size(); i++) {
        char ch = section[i];
        if (ch == ' ' && section.at(i - 1) != ',') {
            section[i] = '\0';
            const char* name = &section[valueStartPos];
            values[name] = currentValueId;
            currentValueId++;
            section[i] = ',';

            valueStartPos = i + 1;
            break;
        } else if (ch == ' ') {
            valueStartPos++;
            continue;
        }
        if (ch == ',') {
            section[i] = '\0';
            const char* name = &section[valueStartPos];
            values[name] = currentValueId;
            currentValueId++;
            section[i] = ',';
            
            valueStartPos = i + 1;
        }
    }
    Type* type = new Type;
    type->Save = [values](DataStream& stream, std::string str) {
        if (values.count(str)) {
            uint valueId = values.at(str);
            stream << valueId;
        } else {
            std::cout << "Invalid enum value " << str << "\n";
            exit(0);
        }
    };
    property->type = type;

    std::string name(&section[valueStartPos]);
    properties[name] = property;
}

void UserType::CreateArrayProperty(Property* property, std::string typeName) {
    if (!types.count(typeName)) {
        std::cout << "Invalid type: " << typeName << "\n";
        exit(0);
    }
    Type* templateType = types[typeName];
    Type* type = new Type;
    type->Save = [templateType](DataStream& stream, std::string str) {
        if (!str.size()) {
            uint size = 0;
            stream << size;
            return;
        }
        std::vector<std::string> subStrs;
        size_t subStrStartPos = 0;
        for (size_t i = 0; i < str.size(); i++) {
            char ch = str[i];
            if (ch == ' ') {
                subStrStartPos++;
            } else if (ch == ',') {
                str[i] = '\0';
                const char* subStr = &str[subStrStartPos];
                subStrs.push_back(subStr);
                str[i] = ',';

                subStrStartPos = i + 1;
            }
        }
        const char* subStr = &str[subStrStartPos];
        subStrs.push_back(subStr);

        uint size = subStrs.size();
        stream << size;
        for (std::string subStr : subStrs) {
            templateType->Save(stream, subStr);
        }
    };
    property->type = type;
}

UserType::UserType(size_t _id, std::map<std::string, UserType*>& userTypes, const std::vector<std::string>& line) : id{_id}, name{line.at(0)} {
    std::string parentUserTypesSection(line.at(1));
    if (parentUserTypesSection.size()) {
        std::vector<std::string> parentUserTypeNames;
        size_t nameStartPos = 0;
        for (size_t i = 0; i < parentUserTypesSection.size(); i++) {
            char ch = parentUserTypesSection[i];
            if (ch == ' ') {
                nameStartPos++;
                continue;
            }
            if (ch == ',') {
                parentUserTypesSection[i] = '\0';
                const char* name = &parentUserTypesSection[nameStartPos];
                parentUserTypeNames.push_back(name);
                parentUserTypesSection[i] = ',';
                
                nameStartPos = i + 1;
            }
        }
        const char* name = &parentUserTypesSection[nameStartPos];
        parentUserTypeNames.push_back(name);

        for (auto name : parentUserTypeNames) {
            if (userTypes.count(name)) {
                parentUserTypes.push_back(userTypes[name]);
                parentUserTypesOffset += userTypes[name]->GetPropertiesSize();
            } else {
                std::cout << "Invalid parent type " << name << "\n";
            }
        }
    }

    for (size_t i = 2; i < line.size(); i++) {
        std::string section(line[i]);
        if (section.size()) {
            Property* property = new Property;
            property->id = (i - 2) + parentUserTypesOffset;
            for (size_t j = 0; j < section.size(); j++) {
                char ch = section[j];
                if (ch == ' ') {
                    section[j] = '\0';
                    std::string typeName(&section[0]);
                    if (typeName == "Enum") {
                        CreateEnumProperty(property, section);
                        break;
                    } else if (!types.count(typeName)) {
                        std::cout << "Invalid type: " << typeName << "\n";
                        exit(0);
                    }
                    property->type = types[typeName];
                    section[j] = ' ';
                    
                    bool foundDefault = false;
                    for (auto k = j + 1; k < section.size(); k++) {
                        if (section[k] == ' ') {
                            section[k] = '\0';
                            property->defaultValue = &section.at(k + 1);
                            std::string propertyName = &section.at(j + 1);
                            properties[propertyName] = property;
                            foundDefault = true;
                            break;
                        }
                    }
                    if (!foundDefault) {
                        std::string propertyName = &section.at(j + 1);
                        properties[propertyName] = property;
                    }
                    break;
                } else if (ch == '[') {
                    section[j] = '\0';
                    std::string typeName(&section[0]);
                    section[j] = '[';
                    CreateArrayProperty(property, typeName);

                    bool foundDefault = false;
                    for (auto k = j + 3; k < section.size(); k++) {
                        if (section[k] == ' ') {
                            section[k] = '\0';
                            property->defaultValue = &section.at(k + 1);
                            std::string propertyName = &section.at(j + 3);
                            properties[propertyName] = property;
                            foundDefault = true;
                            break;
                        }
                    }
                    if (!foundDefault) {
                        std::string propertyName = &section.at(j + 3);
                        properties[propertyName] = property;
                    }
                    break;

                    break;
                }
            }
        }
    }
}

size_t UserType::GetPropertiesSize() {
    return properties.size() + parentUserTypesOffset;
}

Property* UserType::GetProperty(const std::string& name) {
    if (properties.count(name)) {
        return properties[name];
    } else {
        for (auto userType : parentUserTypes) {
            Property* property = userType->GetProperty(name);
            if (property != nullptr) {
                return property;
            }
        }
    }
    return nullptr;
}

std::vector<Property*> UserType::GetProperties() {
    std::vector<Property*> result;
    for (auto [_, property] : properties) {
        result.push_back(property);
    }
    for (auto userType : parentUserTypes) {
        auto parentProperties = userType->GetProperties();
        result.insert(result.end(), parentProperties.begin(), parentProperties.end());
    }
    return result;
}