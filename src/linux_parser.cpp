#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>

#include "parse_file.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem() {
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    vector<string> keys = {"PRETTY_NAME"};
    vector<string> values = ParseFile::getValuesFromKeyValueFile(filestream, keys);

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
    vector<string> values = ParseFile::getValuesFromColonDelimitedFile(filestream, keys);

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

vector<string> LinuxParser::ProcessCpuUtilization(int pid) {
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {

    string line;
    std::getline(filestream, line);
    std::istringstream linestream(line);

    vector<string> cpu_utilization = {};
    string value;
    while (linestream >> value) {
      cpu_utilization.push_back(value);
    }
    return cpu_utilization;
    
    throw std::runtime_error("ProcessCpuUtilization: Failed to read values");
  }
  throw std::runtime_error("ProcessCpuUtilization: Failed to read file");
}


vector<string> LinuxParser::CpuUtilization() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    vector<string> keys = {"cpu"};
    vector<string> values = ParseFile::getValuesFromSpaceSeparatedFile(filestream, keys);

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
    vector<string> values = ParseFile::getValuesFromSpaceSeparatedFile(filestream, keys);

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
    vector<string> values = ParseFile::getValuesFromSpaceSeparatedFile(filestream, keys);

    if (values.size() == 1) {
      return stoi(values[0]);
    }

    throw std::runtime_error("RunningProcesses: Failed to read values");
  }
  throw std::runtime_error("RunningProcesses: Failed to read file");
}

string LinuxParser::Command(int pid) {
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    string line;
    std::getline(filestream, line);
    return line;
  }
  throw std::runtime_error("Command: Failed to read file for pid " + to_string(pid));
}

string LinuxParser::Ram(int pid) {
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    vector<string> keys = {"VmSize"};
    vector<string> values = ParseFile::getValuesFromColonDelimitedFile(filestream, keys);
    if (values.size() == 1) {
      return values[0];
    } else {
      throw std::runtime_error("Ram: Failed to read values for pid " + to_string(pid));
    }
  }
  throw std::runtime_error("Ram: Failed to read file for pid " + to_string(pid));
}

string LinuxParser::Uid(int pid) {
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    vector<string> keys = {"Uid"};
    vector<string> values = ParseFile::getValuesFromColonDelimitedFile(filestream, keys);
    if (values.size() == 1) {
      // 1 space separated line
      std::istringstream linestream(values[0]);
      vector<string> uids = {};
      string value;
      while (linestream >> value) {
        uids.push_back(value);
      }
      return uids[0];
    }
    throw std::runtime_error("Uid: Failed to read values for pid " + to_string(pid));
  }
  throw std::runtime_error("Uid: Failed to read file for pid " + to_string(pid));
}

string LinuxParser::User(int userId) {
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    string line;
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      string username, x, entryUserId;
      linestream >> username >> x >> entryUserId;
      if (entryUserId == to_string(userId)) {
        return username;
      }
    }
    throw std::runtime_error("User: Failed to find user for uid " + to_string(userId));
  }
  throw std::runtime_error("User: Failed to read file");
}

long LinuxParser::UpTime(int pid ) { 
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    string uptimeInTicks = ParseFile::getNthValueFromSpaceSeparatedString(filestream, 21); // the 22nd
    long ticks = stol(uptimeInTicks);
    long secs = ticks / sysconf(_SC_CLK_TCK);
    return secs;
  }
  throw std::runtime_error("UpTime: Failed to read file for pid " + to_string(pid));
}
