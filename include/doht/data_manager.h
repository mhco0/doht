// Doth Library (C) by Marcos Oliveira
#pragma once

#include <memory>
#include <string_view>

#include <doht/shc/shc.h>

namespace doht {

namespace shc {
void Save(const StaticHuffmanCode& shc, const std::string_view& path);

void Load(const std::string_view& path, StaticHuffmanCode& shc);
}  // namespace shc

}  // namespace doht