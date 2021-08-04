#include "Type.h"

Type::Type(size_t _id, std::map<std::string, Type*>& types, const std::vector<std::string>& line) : id{_id}, name{line.at(0)} {
    std::string parentTypesSection(line.at(1));
    if (parentTypesSection.size()) {
        std::vector<std::string> parentTypeNames;
        size_t nameStartPos = 0;
        for (size_t i = 0; i < parentTypesSection.size(); i++) {
            char ch = parentTypesSection[i];
            if (ch == ' ') {
                nameStartPos++;
                continue;
            }
            if (ch == ',') {
                parentTypesSection[i] = '\0';
                const char* name = &parentTypesSection[nameStartPos];
                parentTypeNames.push_back(name);
                parentTypesSection[i] = ',';
                
                nameStartPos = i + 1;
            }
        }
        const char* name = &parentTypesSection[nameStartPos];
        parentTypeNames.push_back(name);

        for (auto name : parentTypeNames) {
            if (types.count(name)) {
                parentTypes.push_back(types[name]);
                parentTypesOffset += types[name]->GetPropertiesSize();
            }
        }
    }

    for (size_t i = 2; i < line.size(); i++) {
        std::string section(line[i]);
        if (section.size()) {
            Property* property = new Property;
            property->id = (i - 2) + parentTypesOffset;
            for (size_t j = 0; j < section.size(); j++) {
                char ch = section[j];
                if (ch == ' ') {
                    section[j] = '\0';
                    const char* typeName = &section[0];
                    property->typeName = typeName;
                    section[j] = ' ';

                    const char* name = &section.at(j + 1);
                    properties[name] = property;
                    break;
                }
            }
        }
    }
}

size_t Type::GetPropertiesSize() {
    return properties.size() + parentTypesOffset;
}

Property* Type::GetProperty(const std::string& name) {
    if (properties.count(name)) {
        return properties[name];
    } else {
        for (auto type : parentTypes) {
            Property* property = type->GetProperty(name);
            if (property != nullptr) {
                return property;
            }
        }
    }
    return nullptr;
}