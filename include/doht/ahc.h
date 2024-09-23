// Doth Library (C) by Marcos Oliveira
#pragma once

#include <memory>
#include <set>
#include <span>
#include <string>
#include <vector>

namespace doht {

class AdaptativeHuffmanCode final {
 public:
  explicit AdaptativeHuffmanCode(const std::set<char>& allowed_characters);
  ~AdaptativeHuffmanCode();
  AdaptativeHuffmanCode(const AdaptativeHuffmanCode&) = delete;

  AdaptativeHuffmanCode(AdaptativeHuffmanCode&& rval) noexcept = default;

  AdaptativeHuffmanCode operator=(const AdaptativeHuffmanCode&) noexcept =
      delete;

  std::vector<bool> Encode(const std::span<char>& text);
  std::string Decode(const std::vector<bool>& encoded);

 protected:
  struct AdaptativeHuffmanCodeImpl;
  std::unique_ptr<AdaptativeHuffmanCodeImpl> impl_;
};
}  // namespace doht