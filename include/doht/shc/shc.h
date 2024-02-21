// Doth Library (C) by Marcos Oliveira
#pragma once

#include <cstddef>
#include <memory>
#include <span>
#include <string>
#include <vector>

namespace doht::shc {

class StaticHuffmanCode final {
 public:
  explicit StaticHuffmanCode();
  ~StaticHuffmanCode();
  StaticHuffmanCode(const StaticHuffmanCode& copy) = delete;
  StaticHuffmanCode(StaticHuffmanCode&& rval) noexcept = default;

  StaticHuffmanCode operator=(const StaticHuffmanCode&) noexcept = delete;

  std::vector<std::byte> Encode(const std::span<char>& text);
  std::string Decode(const std::vector<std::byte>& encoded);

  void Save(const std::string& save_path);
  void Load(const std::string& load_path);

 private:
  struct StaticHuffmanCodeImpl;
  std::unique_ptr<StaticHuffmanCodeImpl> impl_;
};

}  // namespace doht::shc