#include "UserType.h"
#include "Type.h"

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
                    if (!types.count(typeName)) {
                        std::cout << "Invalid type: " << typeName << "\n";
                        exit(1);
                    }
                    property->type = types[typeName];
                    section[j] = ' ';

                    const char* name = &section.at(j + 1);
                    properties[name] = property;
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