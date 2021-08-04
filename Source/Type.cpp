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
        if (line[i].size()) {
            properties[line[i]] = i - 2;
        }
    }
}

size_t Type::GetPropertiesSize() {
    return properties.size() + parentTypesOffset;
}

int Type::GetPropertyId(const std::string& name) {
    if (properties.count(name)) {
        return properties[name] + parentTypesOffset;
    } else {
        for (auto type : parentTypes) {
            int id = type->GetPropertyId(name);
            if (id != -1) {
                return id;
            }
        }
    }
    return -1;
}