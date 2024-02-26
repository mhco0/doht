// Doth Library (C) by Marcos Oliveira
#include <doht/data_manager.h>

#include <string>
#include <string_view>

#include <gtest/gtest.h>

#include <doht/shc/shc.h>

namespace doht {

TEST(DataManagerTest, TestSaveAndLoadSHC) {
  std::string basic_text = "encoding test and saving.";
  shc::StaticHuffmanCode shc{};

  auto encoded = shc.Encode(basic_text);

  shc::Save(shc, "./DataManagerTest.test.shc.txt");

  shc::StaticHuffmanCode new_shc{};

  shc::Load("./DataManagerTest.test.shc.txt", new_shc);

  auto decoded = new_shc.Decode(encoded);

  ASSERT_EQ(decoded, basic_text);
}

}  // namespace doht