#include "process.h"

#include <unistd.h>

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::cout;
using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid(pid) {}

int Process::Pid() { return this->pid; }

float Process::CpuUtilization() const{

  //https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599 
  
  vector<string> cpu_utilization = LinuxParser::ProcessCpuUtilization(this->pid);
  long utime = std::stol(cpu_utilization[LinuxParser::ProcessCPUStates::kUTime_]);
  long stime = std::stol(cpu_utilization[LinuxParser::ProcessCPUStates::kSTime_]);
  long starttime = std::stol(cpu_utilization[LinuxParser::ProcessCPUStates::kStartTime_]);

  long total_time = utime + stime;  // do not add child times too, as we want only this process's CPU usage. 

  float total_time_secs = (float)total_time / sysconf(_SC_CLK_TCK);
  long seconds = LinuxParser::UpTime() - (starttime / sysconf(_SC_CLK_TCK));

  float f = total_time_secs / seconds;

  return (100.0 * f);

}

string Process::Command() { return LinuxParser::Command(this->pid); }

string Process::Ram() {
  string ram = LinuxParser::Ram(this->pid);
  long kb = std::stol(ram);
  return to_string(kb / 1024);
}

int Process::Uid() {
  string uid = LinuxParser::Uid(this->pid);
  //cout << "UID for pid " << this->pid << " is " << uid << '\n';
  return stoi(uid);
}

string Process::User() {
  int userId = this->Uid();
  return LinuxParser::User(userId);
}

long int Process::UpTime() { 
  return LinuxParser::UpTime(this->pid);
}

bool Process::operator<(Process const& a) const { 
  return this->CpuUtilization() > a.CpuUtilization();
 }