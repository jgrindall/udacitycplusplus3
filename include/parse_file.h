#ifndef PARSE_FILE_H
#define PARSE_FILE_H

#include <fstream>
#include <regex>
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace ParseFile {

vector<string> getValuesFromKeyValueFile(std::ifstream& filestream,
                                         vector<string> keys);

vector<string> getValuesFromColonDelimitedFile(std::ifstream& filestream,
                                               vector<string> keys);

vector<string> getValuesFromSpaceSeparatedFile(std::ifstream& filestream,
                                               vector<string> keys);
}  // namespace ParseFile
#endif