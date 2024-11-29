#include "Common.hpp"

namespace uvm {

UVMException::UVMException(const std::string &message)
    : std::runtime_error(message) {}

} // namespace uvm
