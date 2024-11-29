#pragma once
#include <boost/property_tree/ptree.hpp>
#include <string>

namespace uvm {

class Assembler {
public:
  void assemble(const std::string &input_file, const std::string &output_file,
                const std::string &log_file);

  void writeLoadConst(std::ofstream &output, boost::property_tree::ptree &log,
                      int32_t value);
  void writeReadMem(std::ofstream &output, boost::property_tree::ptree &log,
                    uint16_t address);
  void writeWriteMem(std::ofstream &output, boost::property_tree::ptree &log,
                     uint16_t address);
  void writeBitNot(std::ofstream &output, boost::property_tree::ptree &log,
                   uint16_t dest, uint16_t src);
};

} // namespace uvm
