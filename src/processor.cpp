#include "processor.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "parse_file.h"

using std::cout;
using std::stof;
using std::string;
using std::to_string;
using std::vector;

float Processor::Utilization() {
  vector<string> cpu_utilization = LinuxParser::CpuUtilization();
  long user = std::stol(cpu_utilization[LinuxParser::CPUStates::kUser_]);
  long nice = std::stol(cpu_utilization[LinuxParser::CPUStates::kNice_]);
  long system = std::stol(cpu_utilization[LinuxParser::CPUStates::kSystem_]);
  long idle = std::stol(cpu_utilization[LinuxParser::CPUStates::kIdle_]);
  long iowait = std::stol(cpu_utilization[LinuxParser::CPUStates::kIOwait_]);
  long irq = std::stol(cpu_utilization[LinuxParser::CPUStates::kIRQ_]);
  long softirq = std::stol(cpu_utilization[LinuxParser::CPUStates::kSoftIRQ_]);
  long steal = std::stol(cpu_utilization[LinuxParser::CPUStates::kSteal_]);
  long guest = std::stol(cpu_utilization[LinuxParser::CPUStates::kGuest_]);
  long guest_nice =
      std::stol(cpu_utilization[LinuxParser::CPUStates::kGuestNice_]);

  long idle_time = idle + iowait;
  long total_time = user + nice + system + idle + iowait + irq + softirq +
                    steal + guest + guest_nice;

  float f = (float)idle_time / total_time;

  return 100.0 * (1.0 - f);
}