#include "Interpreter.hpp"
#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

int main(int argc, char *argv[]) {

  std::string binary_file, result_file;
  int memory_start, memory_end;
  po::options_description desc("Allowed options");
  desc.add_options()("help,h", "Show help message")(
      "binary,b", po::value<std::string>(&binary_file)->required(),
      "Input binary file")("result,r",
                           po::value<std::string>(&result_file)->required(),
                           "Output result file")(
      "start,s", po::value<int>(&memory_start)->required(),
      "Start memory address")("end,e", po::value<int>(&memory_end)->required(),
                              "End memory address");

  po::variables_map vm;
  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    if (vm.count("help")) {
      std::cout << desc << "\n";
      return 0;
    }
    po::notify(vm);
  } catch (const po::error &e) {
    std::cerr << "Error: " << e.what() << "\n" << desc << "\n";
    return 1;
  }

  try {
    uvm::Interpreter interpreter;
    interpreter.run(binary_file, result_file, memory_start, memory_end);
  } catch (const std::exception &e) {
    std::cerr << "Interpreter error: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
