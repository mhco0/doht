// Doth Library (C) by Marcos Oliveira
#include <doht/data_manager.h>

#include <string>
#include <string_view>

#include <highfive/H5Easy.hpp>

#include <doht/shc/shc.h>

namespace doht {

namespace shc {
void Save(const StaticHuffmanCode& shc, const std::string_view& path) {
    
}

void Load(const std::string_view& path, StaticHuffmanCode& shc) {}
}  // namespace shc

}  // namespace doht