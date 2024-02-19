// Doth Library (C) by Marcos Oliveira
#include <doht/shc/shc.h>

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, const char* argv[]) {
  assert(argc == 3);

  std::string text_path(argv[1]);
  std::string compressed_path(argv[2]);

  std::ifstream input_file(text_path);
  std::ofstream compressed_file(compressed_path, std::ios::binary);

  std::stringstream buffer{};
  buffer << input_file.rdbuf();

  std::string text{buffer.str()};

  doht::shc::StaticHuffmanCode shc{};

  auto encoded_text = shc.Encode(text);

  compressed_file.write(reinterpret_cast<const char*>(encoded_text.data()),
                        encoded_text.size());

  std::string decompressed_path = text_path + "2";
  std::ofstream decompressed_file(decompressed_path);

  auto decoded_text = shc.Decode(encoded_text);
  decompressed_file << decoded_text;

  return 0;
}