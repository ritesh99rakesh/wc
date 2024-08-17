#include "boost/program_options/parsers.hpp"
#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <istream>
#include <print>
#include <span>
#include <string>

struct CommandLineOptions {
  bool count_bytes = false;
  bool count_lines = false;
  bool count_words = false;
  bool count_chars = false;
  std::vector<std::string> input_files;
};

void parseCommandLine(int argc, char **argv, CommandLineOptions &options) {
  namespace po = boost::program_options;
  try {
    po::options_description desc("wc options");
    // clang-format off
    desc.add_options()
      ("help,h", "produce help message")
      ("bytes,c", po::bool_switch(&options.count_bytes), "print the byte counts")
      ("lines,l", po::bool_switch(&options.count_lines), "print the newline counts")
      ("words,w", po::bool_switch(&options.count_words), "print the word counts")
      ("chars,m", po::bool_switch(&options.count_chars), "print the character counts")
      ("files", po::value<std::vector<std::string>>(&options.input_files)->multitoken(), "input files")
    ;
    // clang-format on

    po::positional_options_description p;
    p.add("files", -1);

    po::variables_map vm;
    po::store(
        po::command_line_parser(argc, argv).options(desc).positional(p).run(),
        vm);
    po::notify(vm);

    if (vm.count("help")) {
      std::cout << desc << "\n";
      exit(1);
    }

    // If no option is specified, enable all counts
    if (!options.count_bytes && !options.count_lines && !options.count_words &&
        !options.count_chars) {
      options.count_bytes = options.count_lines = options.count_words = true;
    }
  } catch (std::exception &e) {
    std::println("parsing error: {}", e.what());
    exit(1);
  } catch (...) {
    std::println("unknown exception during parsing");
    exit(1);
  }
}

struct CountResult {
  size_t lines = 0;
  size_t words = 0;
  size_t bytes = 0;
  size_t chars = 0;
};

CountResult count_file(std::istream *input) {
  CountResult result;

  constexpr size_t BUFFER_SIZE = 16384;
  std::array<char, BUFFER_SIZE> buffer;

  bool in_word = false;

  const auto count = [&](std::span<const char> buffer_span) {
    result.bytes += buffer_span.size();
    result.chars += std::ranges::count_if(
        buffer_span, [](unsigned char ch) { return (ch >> 6) != 0b10; });
    result.lines += std::ranges::count(buffer_span, '\n');

    for (unsigned char ch : buffer_span) {
      if (std::isspace(ch)) {
        in_word = false;
      } else if (!in_word) {
        in_word = true;
        ++result.words;
      }
    }
  };

  while (input->read(buffer.data(), BUFFER_SIZE)) {
    std::span<const char> buffer_span(buffer.data(), input->gcount());
    count(buffer_span);
  }

  // Handle any remaining bytes
  std::span<const char> remaining(buffer.data(), input->gcount());
  count(remaining);

  return result;
}

int main(int argc, char **argv) {
  CommandLineOptions options;
  parseCommandLine(argc, argv, options);

  CountResult result;

  if (options.input_files.empty()) {
    result = count_file(&std::cin);
  } else {
    for (auto &&filename : options.input_files) {
      std::ifstream file(filename, std::ios::binary);

      if (!file) {
        std::println("Error: Unable to open file {}", filename);
        exit(1);
      }
      result = count_file(&file);
    }
  }
  std::println("{} {} {} {}", result.lines, result.words, result.bytes,
               result.chars);
}
