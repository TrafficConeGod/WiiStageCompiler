#include "parse.h"

std::vector<std::vector<std::string>> ParseCsv(std::ifstream& file) {
    std::vector<std::vector<std::string>> lines;
    std::string lineStr;
    while (std::getline(file, lineStr)) {
        std::vector<std::string> line;
        size_t sectionStartPos = 0;
        for (size_t i = 0; i < lineStr.size(); i++) {
            char ch = lineStr[i];
            if (ch == ';') {
                lineStr[i] = '\0';
                const char* section = &lineStr[sectionStartPos];
                line.push_back(section);
                lineStr[i] = ';';
                
                sectionStartPos = i + 1;
            }
        }
        const char* section = &lineStr[sectionStartPos];
        line.push_back(section);

        lines.push_back(line);
    }
    return lines;
}