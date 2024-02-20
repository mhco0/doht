// Doth Library (C) by Marcos Oliveira
#include <doht/shc/shc.h>

#include <CLI/CLI.hpp>

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace doht {
void AppInit(CLI::App& console_app, int argc, const char* argv[]) {
  console_app.allow_windows_style_options();

  std::string text_path{};
  console_app.add_option("--input", text_path, "Input text to be compressed");

  std::string compressed_path{};
  console_app.add_option("--ouput", compressed_path,
                         "Output file to be generated");

  CLI11_PARSE(console_app, argc, argv);
}

void AppRun(CLI::App& console_app);
}  // namespace doht

int main(int argc, const char* argv[]) {
  CLI::App console_app{
      "Doth console application. Apply compression and decompression on "
      "files."};

  doht::AppInit(console_app, argc, argv);

  // std::ifstream input_file(text_path);
  // std::ofstream compressed_file(compressed_path, std::ios::binary);

  // std::stringstream buffer{};
  // buffer << input_file.rdbuf();

  // std::string text{buffer.str()};

  // doht::shc::StaticHuffmanCode shc{};

  // auto encoded_text = shc.Encode(text);

  // compressed_file.write(reinterpret_cast<const char*>(encoded_text.data()),
  //                       encoded_text.size());

  // std::string decompressed_path = text_path + "2";
  // std::ofstream decompressed_file(decompressed_path);

  // auto decoded_text = shc.Decode(encoded_text);
  // decompressed_file << decoded_text;

  return 0;
}