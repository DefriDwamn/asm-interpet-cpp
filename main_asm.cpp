#include "Assembler.hpp"
#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

int main(int argc, char *argv[]) {

  std::string input_file, output_file, log_file;
  po::options_description desc("Allowed options");
  desc.add_options()("help,h", "Show help message")(
      "input,i", po::value<std::string>(&input_file)->required(),
      "Input assembly file")("output,o",
                             po::value<std::string>(&output_file)->required(),
                             "Output binary file")(
      "log,l", po::value<std::string>(&log_file)->default_value("logs.xml"),
      "Log file (defualt: logs.xml)");

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
    uvm::Assembler assembler;
    assembler.assemble(input_file, output_file, log_file);
  } catch (const std::exception &e) {
    std::cerr << "Assembler error: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
