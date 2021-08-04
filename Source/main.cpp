#include "parse.h"
#include "UserType.h"
#include <fstream>
#include <map>
#include <iostream>
#include "Type.h"

std::map<std::string, Type*> types;

int main(int argCount, char** args) {
    if (argCount < 4) {
        puts("Not enough args");
        return 1;
    }
    std::string userTypesPath(args[1]);
    // std::string inputPath(args[2]);
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
}