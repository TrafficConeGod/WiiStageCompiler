#pragma once
#include <vector>
#include <string>
#include <fstream>

std::vector<std::vector<std::string>> ParseCsv(std::ifstream& file);