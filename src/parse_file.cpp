#include "parse_file.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

#include <fstream>
#include <regex>
#include <string>

vector<string> ParseFile::getValuesFromKeyValueFile(std::ifstream& filestream,
                                                    vector<string> keys) {
  vector<string> values;
  string line;
  const char REPLACE = '_';

  // Prepare keys
  for (string& key : keys) {
    std::replace(key.begin(), key.end(), ' ', REPLACE);
  }

  while (std::getline(filestream, line)) {
    std::replace(line.begin(), line.end(), ' ', REPLACE);
    std::replace(line.begin(), line.end(), '=', ' ');
    std::replace(line.begin(), line.end(), '"', ' ');
    std::istringstream linestream(line);

    string key, value;

    while (linestream >> key >> value) {
      if (std::find(keys.begin(), keys.end(), key) != keys.end()) {
        std::replace(value.begin(), value.end(), REPLACE, ' ');
        values.push_back(value);
      }
    }
  }
  return values;
}

vector<string> ParseFile::getValuesFromColonDelimitedFile(
    std::ifstream& filestream, vector<string> keys) {
  vector<string> values;
  string line;

  while (std::getline(filestream, line)) {
    size_t colon_pos = line.find(':');
    if (colon_pos != string::npos) {
      string key = line.substr(0, colon_pos);
      string value = line.substr(colon_pos + 1);

      // Trim whitespace from value
      size_t start = value.find_first_not_of(" \t");
      if (start != string::npos) {
        value = value.substr(start);
      }

      if (std::find(keys.begin(), keys.end(), key) != keys.end()) {
        values.push_back(value);
      }
    }
  }
  return values;
}

vector<string> ParseFile::getValuesFromSpaceSeparatedFile(
    std::ifstream& filestream, vector<string> keys) {
  vector<string> values;
  string line;

  while (std::getline(filestream, line)) {
    std::istringstream linestream(line);
    string key, value;

    // value can have spaces like key   10 20 30.  We need all of the values
    // after the key.

    std::ostringstream value_stream;
    if (linestream >> key) {
      while (linestream >> value) {
        value_stream << value << " ";
      }
    }

    value = value_stream.str();

    if (std::find(keys.begin(), keys.end(), key) != keys.end()) {
      values.push_back(value);
    }
  }
  return values;
}