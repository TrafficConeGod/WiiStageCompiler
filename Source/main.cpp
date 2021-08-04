#include "parse.h"
#include "UserType.h"
#include <fstream>
#include <map>
#include <iostream>
#include <algorithm>
#include "Type.h"

std::map<std::string, Type*> types;

int main(int argCount, char** args) {
    if (argCount < 4) {
        puts("Not enough args");
        return 1;
    }
    std::string userTypesPath(args[1]);
    std::string stageInputPath(args[2]);
    // std::string outputPath(args[3]);

    std::ifstream userTypesFile(userTypesPath);
    std::vector<std::vector<std::string>> userTypesLines(ParseCsv(userTypesFile));

    std::map<std::string, UserType*> userTypes;
    size_t i = 1;
    for (auto line : userTypesLines) {
        UserType* userType = new UserType(i, userTypes, line);
        userTypes[userType->name] = userType;
        i++;
    }

    for (auto [_, userType] : userTypes) {
        std::cout << userType->name << " ";
        Property* property = userType->GetProperty("Image");
        if (property != nullptr) {
            std::cout << property->id;
        }
        std::cout << "\n";
    }

    std::ifstream stageInputFile(stageInputPath);
    std::vector<std::vector<std::string>> stageLines(ParseCsv(stageInputFile));

    DataStream stageOutputStream;
    for (auto line : stageLines) {
        std::string userTypeName(line.at(0));
        if (!userTypes.count(userTypeName)) {
            std::cout << "Invalid user type " << userTypeName << "\n";
            return 1;
        }
        UserType* userType = userTypes[userTypeName];

        struct PropertyValue {
            Property* property;
            std::string value;
        };
        std::vector<PropertyValue> propertyValues;

        for (size_t i = 1; i < line.size(); i += 2) {
            std::string propertyName(line[i]);
            if (propertyName.size()) {
                Property* property = userType->GetProperty(propertyName);
                if (property == nullptr) {
                    std::cout << "Invalid property " << propertyName << "\n";
                    return 1;
                }

                std::string value(line.at(i + 1));
                propertyValues.push_back({ property, value });
            }
        }

        std::sort(propertyValues.begin(), propertyValues.end(), [](const PropertyValue& a, const PropertyValue& b) {
            return a.property->id < b.property->id;
        });
        
        for (size_t i = 0; i < propertyValues.size(); i++) {
            const PropertyValue& propertyValue = propertyValues[i];
            if (propertyValue.property->id != i) {
                std::cout << "Missing property" << "\n";
                return 1;
            }
            propertyValue.property->type->Save(stageOutputStream, propertyValue.value);
        }
    }
}