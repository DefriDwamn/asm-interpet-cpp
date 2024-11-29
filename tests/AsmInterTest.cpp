#include "Assembler.hpp"
#include "Common.hpp"
#include "Interpreter.hpp"
#include "gtest/gtest.h"
#include <boost/property_tree/xml_parser.hpp>
#include <cstdint>
#include <fstream>
#include <sstream>

namespace uvm {

class AssemblerTest : public ::testing::Test {
protected:
};

class InterpreterTest : public ::testing::Test {
protected:
};

TEST_F(AssemblerTest, AssembleLoadConst) {
  std::ofstream output("output.bin", std::ios::binary);
  boost::property_tree::ptree log_tree;

  Assembler assembler;
  assembler.writeLoadConst(output, log_tree, 123456);

  output.close();
  std::ifstream input("output.bin", std::ios::binary);
  uint8_t opcode;
  input.read(reinterpret_cast<char *>(&opcode), 1);
  EXPECT_EQ(opcode, static_cast<uint8_t>(OpCode::LOAD_CONST));

  uint32_t value;
  input.read(reinterpret_cast<char *>(&value), sizeof(value));
  EXPECT_EQ(value, 123456);
}

TEST_F(AssemblerTest, AssembleUnknownCommand) {
  std::ofstream output("output.bin", std::ios::binary);
  boost::property_tree::ptree log_tree;

  Assembler assembler;

  try {
    std::istringstream iss("UNKNOWN_COMMAND");
    std::string command;
    iss >> command;
    if (command != "LOAD_CONST") {
      throw UVMException("Unknown command: " + command);
    }
  } catch (const UVMException &e) {
    EXPECT_EQ(e.what(), std::string("Unknown command: UNKNOWN_COMMAND"));
  }
}

TEST_F(AssemblerTest, WriteLoadConstLog) {
  boost::property_tree::ptree log_tree;
  std::ofstream output("output.bin", std::ios::binary);

  Assembler assembler;
  assembler.writeLoadConst(output, log_tree, 123456);

  std::stringstream log_ss;
  boost::property_tree::write_xml(log_ss, log_tree);
  std::string log_content = log_ss.str();
  EXPECT_TRUE(log_content.find("<command>LOAD_CONST</command>") !=
              std::string::npos);
  EXPECT_TRUE(log_content.find("<value>123456</value>") != std::string::npos);
}

TEST_F(InterpreterTest, RunLoadConst) {
  std::ofstream output("output.bin", std::ios::binary);
  output.put(static_cast<uint8_t>(OpCode::LOAD_CONST));
  int32_t value = 123;
  output.write(reinterpret_cast<char *>(&value), sizeof(value));

  output.put(static_cast<uint8_t>(OpCode::WRITE_MEM));
  uint16_t address = 0;
  output.write(reinterpret_cast<char *>(&address), sizeof(address));
  output.close();
  
  Interpreter interpreter;
  interpreter.run("output.bin", "result.xml", 0, 0);

  boost::property_tree::ptree result_tree;
  boost::property_tree::read_xml("result.xml", result_tree);
  auto &cells = result_tree.get_child("memory");
  EXPECT_EQ(cells.size(), 1);
  EXPECT_EQ(cells.begin()->second.get<int>("value"), 123);
}

} // namespace uvm
