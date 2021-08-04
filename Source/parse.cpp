#include "parse.h"
#include <iostream>

std::vector<std::vector<std::string>> ParseCsv(std::ifstream& file) {
    std::vector<std::vector<std::string>> lines;
    std::string lineStr;
    while (std::getline(file, lineStr)) {
        std::cout << lineStr << "\n";
    }
    return lines;
}