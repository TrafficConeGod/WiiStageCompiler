#include "parse.h"
#include "Type.h"
#include <fstream>

int main(int argCount, char** args) {
    if (argCount < 4) {
        puts("Not enough args");
        return 1;
    }
    std::string typesPath(args[1]);
    // std::string inputPath(args[2]);
    // std::string outputPath(args[3]);

    std::ifstream typesFile(typesPath);
    std::vector<std::vector<std::string>> typesLines(ParseCsv(typesFile));
}