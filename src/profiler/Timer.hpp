#pragma once

#include <ostream>
#include <chrono>
#include <string>

class Timer {
 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
  std::ostream& out_;
  std::string message_;
 public:
  Timer(std::ostream &out, std::string message);
  ~Timer();
};
