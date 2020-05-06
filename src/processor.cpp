#include "processor.h"
#include "linux_parser.h"

#include <string>
#include <vector>

#include <fstream>
#include <sstream>

#include <chrono> // for cpu calc
#include <thread> // for cpu calc

#include <iostream> // REMOVE LATER

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  float PrevIdle, Idle, PrevNonIdle, NonIdle, PrevTotal, Total, totald, idled, CPU_percentage;

  PrevTotal = LinuxParser::Jiffies();
  PrevIdle = LinuxParser::IdleJiffies();
  std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // sleep for 1000 milliseconds (1 second)
  Total = LinuxParser::Jiffies();
  Idle = LinuxParser::IdleJiffies();
  
  totald = Total - PrevTotal;
  idled = Idle - PrevIdle;

  CPU_percentage = (float)(totald - idled) / (float)totald;

  return CPU_percentage;
}