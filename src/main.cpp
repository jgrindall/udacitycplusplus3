#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "ncurses_display.h"
#include "system.h"

using std::cout;
using std::stof;
using std::string;
using std::to_string;
using std::vector;

int main() {
  System system;
  NCursesDisplay::Display(system);
}

void log(){
  System system;
  while (1) {
    cout << "---------------------------------------\n";
    cout << "System Uptime: " << system.UpTime() << " seconds\n";
    cout << "Operating System: " << system.OperatingSystem() << '\n';
    cout << "Kernel: " << system.Kernel() << '\n';
    cout << "CPU Utilization: " << system.Cpu().Utilization() << '\n';
    cout << "Memory Utilization: " << system.MemoryUtilization() << '\n';
    cout << "Total Processes: " << system.TotalProcesses() << '\n';
    cout << "Running Processes: " << system.RunningProcesses() << '\n';
    cout << "---------------------------------------\n";

    system.refreshProcesses();

    for (auto& process : system.Processes()) {
      try{
        cout << process.Pid() << '\n';
        cout << "User: " << process.User() << '\n';
        cout << "ram: " << process.Ram() << '\n';
        cout << "Command: " << process.Command() << '\n';
        cout << "CPU: " << process.CpuUtilization() << '\n';
        cout << "---------------------------------------\n";
      } catch (const std::runtime_error& e) {
        cout << "Error retrieving process info: " << e.what() << '\n';
      }
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}