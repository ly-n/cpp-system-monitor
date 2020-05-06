#include <string>

//#include <cmath>
//#include <cstdlib>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
//string Format::ElapsedTime(long seconds[[maybe_unused]]) { return string(); }
string Format::ElapsedTime(long seconds) {
  int seconds_, hours_, minutes_;
  std::string time;

  hours_ = (int)(seconds / 3600);
  minutes_ = (seconds % 3600) / 60;
  seconds_ = seconds % 60;

  time = std::to_string(hours_) + ":" + std::to_string(minutes_) + ":" + std::to_string(seconds_);

  return time;
}