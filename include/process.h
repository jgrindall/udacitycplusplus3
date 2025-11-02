#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
 public:
  // constructor
  Process(int pid);

  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;

 private:
  int pid;
};

#endif