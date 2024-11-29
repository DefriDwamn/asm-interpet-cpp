#include "Interpreter.hpp"
#include "Common.hpp"

#include <boost/property_tree/xml_parser.hpp>
#include <cstring>
#include <fstream>

namespace uvm {

void Interpreter::run(const std::string &binary_file,
                      const std::string &result_file, int mem_start,
                      int mem_end) {
  std::ifstream binary(binary_file, std::ios::binary);
  if (!binary.is_open())
    throw UVMException("Cannot open binary file: " + binary_file);

  std::vector<int32_t> memory(1024, 0);
  uint32_t accumulator = 0;

  while (binary.peek() != EOF) {
    uint8_t opcode;
    binary.read(reinterpret_cast<char *>(&opcode), 1);

    switch (static_cast<OpCode>(opcode)) {
    case OpCode::LOAD_CONST: {
      uint32_t value;
      binary.read(reinterpret_cast<char *>(&value), sizeof(value));
      accumulator = value;
      break;
    }
    case OpCode::READ_MEM: {
      uint16_t address;
      binary.read(reinterpret_cast<char *>(&address), sizeof(address));
      accumulator = memory[address];
      break;
    }
    case OpCode::WRITE_MEM: {
      uint16_t address;
      binary.read(reinterpret_cast<char *>(&address), sizeof(address));
      memory[address] = accumulator;
      break;
    }
    case OpCode::BIT_NOT: {
      uint16_t dest, src;
      binary.read(reinterpret_cast<char *>(&dest), sizeof(dest));
      binary.read(reinterpret_cast<char *>(&src), sizeof(src));
      memory[dest] = ~memory[src];
      break;
    }
    default:
      throw UVMException("Unknown opcode: " + std::to_string(opcode));
    }
  }

  boost::property_tree::ptree result_tree;
  for (int i = mem_start; i <= mem_end; ++i) {
    boost::property_tree::ptree mem_node;
    mem_node.put("address", i);
    mem_node.put("value", memory[i]);
    result_tree.add_child("memory.cell", mem_node);
  }

  boost::property_tree::write_xml(result_file, result_tree);
}

} // namespace uvm
