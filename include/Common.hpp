#pragma once
#include <cstdint>
#include <stdexcept>
#include <string>

namespace uvm {

enum class OpCode : uint8_t {
  LOAD_CONST = 0b0111,
  READ_MEM = 0b0110,
  WRITE_MEM = 0b0101,
  BIT_NOT = 0b0001
};

class UVMException : public std::runtime_error {
public:
  explicit UVMException(const std::string &message);
};

} // namespace uvm
