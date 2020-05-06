#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include <chrono> // for cpu calc
#include <thread> // for cpu calc

#include "process.h"
#include "linux_parser.h" // included for system path variables

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() {
  return this->pid;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  // calculate https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
  // seconds = system_uptime - (starttime / Hertz) a.k.a. UpTime
  // cpu_usage = 100 * ((total_time / Hertz) / seconds)

  float total_active, prev_total_active, system_uptime_seconds, totald, cpu_usage;

  total_active = LinuxParser::ActiveJiffies(pid);

  system_uptime_seconds = UpTime();

  cpu_usage = 100 * ( (total_active / sysconf(_SC_CLK_TCK)) / system_uptime_seconds );
  return cpu_usage;
}

// TODO: Return the command that generated this process
string Process::Command() {
  return LinuxParser::Command(this->pid);
}

// TODO: Return this process's memory utilization
string Process::Ram() {
  return LinuxParser::Ram(this->pid);
}

// TODO: Return the user (name) that generated this process
string Process::User() {
  std::string user = LinuxParser::User(this->pid);
  return user;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
  return LinuxParser::UpTime(this->pid);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }
