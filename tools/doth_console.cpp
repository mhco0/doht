// Doth Library (C) by Marcos Oliveira
#include <doht/shc.h>

#include <CLI/CLI.hpp>

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <variant>

namespace doht {
struct CompressCommand {
  std::string input_path{};
  std::string compressed_path{};
  std::string encoder_path{};
};

struct DecompressCommand {
  std::string encoder_path{};
  std::string compressed_path{};
  std::string decompressed_path{};
};

using Command = std::variant<CompressCommand, DecompressCommand, int>;

Command AppInit(CLI::App& console_app, int argc, const char* argv[]) {
  console_app.allow_windows_style_options();
  auto* compress_subcommand =
      console_app.add_subcommand("compress", "Applies compression user file.");

  CompressCommand compress_command{};
  compress_subcommand
      ->add_option("--input", compress_command.input_path,
                   "Input text path to be compressed.")
      ->required();
  compress_subcommand
      ->add_option("--output", compress_command.compressed_path,
                   "Path to write the compressed version of the file.")
      ->required();
  compress_subcommand
      ->add_option("--enc-path", compress_command.encoder_path,
                   "Path to write the StaticHuffmanEncodeTree.")
      ->required();

  auto* decompress_subcommand = console_app.add_subcommand(
      "decompress", "Applies decompression on user file.");

  DecompressCommand decompress_command{};
  decompress_subcommand
      ->add_option("--input", decompress_command.compressed_path,
                   "Input file path to be decompressed.")
      ->required();
  decompress_subcommand
      ->add_option("--output", decompress_command.decompressed_path,
                   "Path to write the compressed version of the file.")
      ->required();
  decompress_subcommand
      ->add_option("--enc-path", decompress_command.encoder_path,
                   "Path to write the StaticHuffmanEncodeTree.")
      ->required();

  Command command{};
  compress_subcommand->callback(
      [&command, &compress_command] { command = compress_command; });
  decompress_subcommand->callback(
      [&command, &decompress_command] { command = decompress_command; });

  CLI11_PARSE(console_app, argc, argv);

  return command;
}

void AppRun(CLI::App& console_app, const CompressCommand& command) {
  std::ifstream input_file(command.input_path);
  std::ofstream compressed_file(command.compressed_path, std::ios::binary);

  std::stringstream buffer{};
  buffer << input_file.rdbuf();

  std::string text{buffer.str()};

  doht::StaticHuffmanCode shc{};

  auto encoded_text = shc.Encode(text);

  compressed_file.write(reinterpret_cast<const char*>(encoded_text.data()),
                        encoded_text.size());

  shc.Save(command.encoder_path);

  return;
}

void AppRun(CLI::App& console_app, const DecompressCommand& command) {
  StaticHuffmanCode shc{};
  std::cout << command.decompressed_path << "\n";
  std::cout << command.compressed_path << "\n";
  std::cout << command.encoder_path;
  shc.Load(command.encoder_path);

  // std::ifstream compressed_file(command.compressed_path, std::ios::binary);

  // compressed_file.unsetf(std::ios::skipws);

  // // get data size
  // compressed_file.seekg(0, std::ios::end);
  // std::streampos file_size{};
  // file_size = compressed_file.tellg();
  // compressed_file.seekg(0, std::ios::beg);

  // // reserve capacity
  // std::vector<unsigned char> file_data;
  // file_data.reserve(file_size);

  // // read the file_data
  // file_data.insert(file_data.begin(),
  //                  std::istream_iterator<unsigned char>(compressed_file),
  //                  std::istream_iterator<unsigned char>());

  std::ofstream decompressed_file(command.decompressed_path);

  // if (decompressed_file.is_open()) {
  //   std::vector<std::byte> data(file_data.size());
  //   std::memcpy(data.data(), file_data.data(), file_data.size());
  //   auto decoded_text = shc.Decode(data);

  //   decompressed_file << decoded_text;
  // }

  return;
}

void AppRun(CLI::App& console_app, const int& error) {
  std::cout << "Some parser error: " << error << std::endl;
}

}  // namespace doht

int main(int argc, const char* argv[]) {
  CLI::App console_app{
      "Doth console application. Apply compression and decompression on "
      "files."};

  auto command = doht::AppInit(console_app, argc, argv);

  std::visit([&console_app](auto&& arg) { doht::AppRun(console_app, arg); },
             command);

  return 0;
}