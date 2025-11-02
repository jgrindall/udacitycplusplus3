#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "parse_file.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem() {
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    vector<string> keys = {"PRETTY_NAME"};
    vector<string> values =
        ParseFile::getValuesFromKeyValueFile(filestream, keys);

    if (!values.empty()) {
      return values[0];
    }

    throw std::runtime_error("OperatingSystem: Failed to read values");
  }

  throw std::runtime_error("OperatingSystem: Failed to read file");
}

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    vector<string> keys = {"MemTotal", "MemFree"};
    vector<string> values =
        ParseFile::getValuesFromColonDelimitedFile(filestream, keys);

    if (values.size() == 2) {
      float memTotal = stof(values[0]);
      float memFree = stof(values[1]);
      float memUsed = memTotal - memFree;
      return memUsed / memTotal;
    }

    throw std::runtime_error("MemoryUtilization: Failed to read values");
  }
  throw std::runtime_error("MemoryUtilization: Failed to read file");
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    // two numbers: uptime and idle time
    string line;
    std::getline(filestream, line);
    std::istringstream linestream(line);
    long uptime;
    linestream >> uptime;
    return uptime;
  }
  throw std::runtime_error("UpTime: Failed to read file");
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

vector<string> LinuxParser::CpuUtilization() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    vector<string> keys = {"cpu"};
    vector<string> values =
        ParseFile::getValuesFromSpaceSeparatedFile(filestream, keys);

    if (values.size() == 1) {
      // 1 space separated line
      std::istringstream linestream(values[0]);
      vector<string> cpu_utilization = {};
      string value;
      while (linestream >> value) {
        cpu_utilization.push_back(value);
      }
      return cpu_utilization;
    }

    throw std::runtime_error("CpuUtilization: Failed to read values");
  }
  throw std::runtime_error("CpuUtilization: Failed to read file");
}

int LinuxParser::TotalProcesses() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    vector<string> keys = {"processes"};
    vector<string> values =
        ParseFile::getValuesFromSpaceSeparatedFile(filestream, keys);

    if (values.size() == 1) {
      return stoi(values[0]);
    }

    throw std::runtime_error("TotalProcesses: Failed to read values");
  }
  throw std::runtime_error("TotalProcesses: Failed to read file");
}

int LinuxParser::RunningProcesses() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    vector<string> keys = {"procs_running"};
    vector<string> values =
        ParseFile::getValuesFromSpaceSeparatedFile(filestream, keys);

    if (values.size() == 1) {
      return stoi(values[0]);
    }

    throw std::runtime_error("RunningProcesses: Failed to read values");
  }
  throw std::runtime_error("RunningProcesses: Failed to read file");
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]]) {
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    string line;
    std::getline(filestream, line);
    return line;
  }
  throw std::runtime_error("Command: Failed to read file for pid " +
                           to_string(pid));
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }
