#include "Assembler.hpp"
#include "Common.hpp"
#include <boost/property_tree/xml_parser.hpp>
#include <fstream>
#include <sstream>

namespace uvm {

void Assembler::assemble(const std::string &input_file,
                         const std::string &output_file,
                         const std::string &log_file) {
  std::ifstream input(input_file);
  if (!input.is_open())
    throw UVMException("Cannot open input file: " + input_file);

  std::ofstream output(output_file, std::ios::binary);
  if (!output.is_open())
    throw UVMException("Cannot open output file: " + output_file);

  boost::property_tree::ptree log_tree;
  std::string line;

  while (std::getline(input, line)) {
    std::istringstream iss(line);
    std::string command;
    iss >> command;

    if (command == "LOAD_CONST") {
      int32_t value;
      iss >> value;
      writeLoadConst(output, log_tree, value);
    } else if (command == "READ_MEM") {
      uint16_t address;
      iss >> address;
      writeReadMem(output, log_tree, address);
    } else if (command == "WRITE_MEM") {
      uint16_t address;
      iss >> address;
      writeWriteMem(output, log_tree, address);
    } else if (command == "BIT_NOT") {
      uint16_t dest, src;
      iss >> dest >> src;
      writeBitNot(output, log_tree, dest, src);
    } else {
      throw UVMException("Unknown command: " + command);
    }
  }

  boost::property_tree::write_xml(log_file, log_tree);
}

void Assembler::writeLoadConst(std::ofstream &output,
                               boost::property_tree::ptree &log,
                               int32_t value) {
  uint8_t opcode = static_cast<uint8_t>(OpCode::LOAD_CONST);
  output.put(opcode);
  output.write(reinterpret_cast<char *>(&value), sizeof(value));

  boost::property_tree::ptree node;
  node.put("command", "LOAD_CONST");
  node.put("value", value);
  log.add_child("commands.command", node);
}

void Assembler::writeReadMem(std::ofstream &output,
                             boost::property_tree::ptree &log,
                             uint16_t address) {
  uint8_t opcode = static_cast<uint8_t>(OpCode::READ_MEM);
  output.put(opcode);
  output.write(reinterpret_cast<char *>(&address), sizeof(address));

  boost::property_tree::ptree node;
  node.put("command", "READ_MEM");
  node.put("address", address);
  log.add_child("commands.command", node);
}

void Assembler::writeWriteMem(std::ofstream &output,
                              boost::property_tree::ptree &log,
                              uint16_t address) {
  uint8_t opcode = static_cast<uint8_t>(OpCode::WRITE_MEM);
  output.put(opcode);
  output.write(reinterpret_cast<char *>(&address), sizeof(address));

  boost::property_tree::ptree node;
  node.put("command", "WRITE_MEM");
  node.put("address", address);
  log.add_child("commands.command", node);
}

void Assembler::writeBitNot(std::ofstream &output,
                            boost::property_tree::ptree &log, uint16_t dest,
                            uint16_t src) {
  uint8_t opcode = static_cast<uint8_t>(OpCode::BIT_NOT);
  output.put(opcode);
  output.write(reinterpret_cast<char *>(&dest), sizeof(dest));
  output.write(reinterpret_cast<char *>(&src), sizeof(src));

  boost::property_tree::ptree node;
  node.put("command", "BIT_NOT");
  node.put("dest", dest);
  node.put("src", src);
  log.add_child("commands.command", node);
}

} // namespace uvm
