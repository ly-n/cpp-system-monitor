#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include <fstream>
#include <sstream>
#include <locale> // for isdigit
#include <filesystem>
#include <experimental/filesystem>

#include <iostream> // REMOVE LATER

#include "linux_parser.h"
#include "format.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, ignore;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> ignore >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() { 
  vector<int> pids;
  std::filesystem::path path(kProcDirectory.c_str());
  std::string filename;
  
  for ( const auto& file : std::filesystem::directory_iterator(path) ) {
    if ( std::filesystem::is_directory(file) ) {
      filename = file.path().filename();

      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = std::stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::string line;
  float memTotal, memFree;
  std::string ignore;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  std::istringstream linestream;
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> ignore >> memTotal;

    std::getline(filestream, line);
    std::istringstream linestream2(line);
    linestream2 >> ignore >> memFree;
  }
  return ( (memTotal-memFree) / memTotal );
}


// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  std::string line;
  long uptime;

  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return uptime;
}




// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  long Idle, NonIdle, Total;

  Idle = IdleJiffies();
  NonIdle = ActiveJiffies();
  Total = Idle + NonIdle;
  return Total;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  // utime + stime + cutime + cstime
  float total_time;
  float utime, stime, cutime, cstime;
  vector<std::string> cpu = CpuUtilization(pid);

  bool caught = false;
  try {
    // cpu = {utime, stime, cutime, cstime}
    utime = std::stof(cpu[0]);
    stime = std::stof(cpu[1]);
    cutime = std::stof(cpu[2]);
    cstime = std::stof(cpu[3]);
  } catch(...) {
    caught = true;
  }

  total_time = utime + stime + cutime + cstime;
  return total_time;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  std::string line, ignore;
  long user, nice, system, idle, iowait, irq, softirq, steal, guest, guestnice;
  long NonIdle;
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> ignore >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guestnice;
  }
  NonIdle = user + nice + system + irq + softirq + steal;

  return  NonIdle; 
  return 0; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  std::string line, ignore;
  long user, nice, system, idle, iowait, irq, softirq, steal, guest, guestnice;
  long Idle;
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> ignore >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guestnice;
  }
  Idle = idle + iowait;

  return Idle; 
  return 0; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization(int pid) { 
  // calculate https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
  std::string line, token;
  std::string utime, stime, cutime, cstime;

  std::ifstream filestream(kProcDirectory  + std::to_string(pid) + kStatFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i=0; i<23; i++) { 
      linestream >> token;
      switch(i) {
        case 13:
          utime = token;
          [[fallthrough]];
        case 14:
          stime = token;
          [[fallthrough]];
        case 15:
          cutime = token;
          [[fallthrough]];
        case 16:
          cstime = token;
          [[fallthrough]];
        default:
          break;
      }

    }
  }

  return {utime, stime, cutime, cstime};
}



// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::string line, label, value;
  int totalProcesses;
  
  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> label >> value;
      if (label == "processes") {
        totalProcesses = std::stoi(value);
      }
    }
  }

  //return 0;
  return totalProcesses;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::string line, label, value;
  int runningProcesses;
  
  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> label >> value;
      if (label == "procs_running") {
        runningProcesses = std::stoi(value);
      }
    }
  }
  return runningProcesses;

}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
//string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }
string LinuxParser::Command(int pid) { 
  std::string line;
  std::string cmd;

  std::ifstream filestream(kProcDirectory  + std::to_string(pid) + kCmdlineFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
  
    for (int i=0; i<3; i++) {
      linestream >> cmd;
    }
  
  }
  return cmd;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
//string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }
string LinuxParser::Ram(int pid) { 
  std::string line, label, unit;
  float ram;
  
  std::ifstream filestream(kProcDirectory  + std::to_string(pid) + kStatusFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if (line.find("VmSize:") != std::string::npos) {
        std::istringstream linestream(line);
        linestream >> label >> ram >> unit;
      }
    }
  }
  return std::to_string(ram / 1000);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
//string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }
string LinuxParser::Uid(int pid) {
  std::string line, label;
  std::string uid;
  
  std::ifstream filestream(kProcDirectory  + std::to_string(pid) + kStatusFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) { 
      if (line.find("Uid:") != std::string::npos) {
        std::istringstream linestream(line);
        linestream >> label >> uid;
      }
    }
  }

  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
//string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }
string LinuxParser::User(int pid) {
  std::string uid = Uid(pid);

  // get user from uid from /etc/passwd 
  std::string line;
  std::string user;
  int str_end_pos;
  
  std::ifstream filestream(kPasswordPath);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      str_end_pos = line.find(":x:" + uid + ":");
      if (str_end_pos != std::string::npos) {
        user = line.substr(0,str_end_pos);
      }
    }
  }

  return user;
  return string(); 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
//long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }
long LinuxParser::UpTime(int pid) {
  std::string line, token;
  long starttime, system_uptime;

  /* 22 starttime  %llu
  The time the process started after system boot.  In kernels before Linux 2.6, this value was expressed in jiffies.
  Since Linux 2.6, the value is expressed in clock ticks (divide by sysconf(_SC_CLK_TCK)).
  http://man7.org/linux/man-pages/man5/proc.5.html
  */
  std::ifstream filestream(kProcDirectory  + std::to_string(pid) + kStatFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i=0; i<22; i++) { // actual index 21 since man page starts at 1
      linestream >> token;
    }
  }

  bool caught = false;
  try {
    starttime = std::stol(token);
  } catch(...) {
    caught = true;
  }

  starttime = starttime / sysconf(_SC_CLK_TCK);
  system_uptime = UpTime();


  return (system_uptime - starttime);
}