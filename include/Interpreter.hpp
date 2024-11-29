#pragma once
#include <boost/property_tree/ptree.hpp>
#include <string>

namespace uvm {

class Interpreter {
public:
  void run(const std::string &binary_file, const std::string &result_file,
           int mem_start, int mem_end);
};

} // namespace uvm
