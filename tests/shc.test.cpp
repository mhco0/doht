// Doth Library (C) by Marcos Oliveira
#include <doht/shc/shc.h>

#include <string>

#include <gtest/gtest.h>

namespace doht::shc {

class StaticHuffmanCodeTest : public ::testing::Test {
 protected:
  StaticHuffmanCodeTest() {}

  StaticHuffmanCode shc_{};
  std::string text_{"aaaaabbbccdeeee"};
  std::vector<std::byte> translation_{
      std::byte{0b00000001}, std::byte{0b11111111}, std::byte{0b11000000},
      std::byte{0b01101101}, std::byte{0b01010101}, std::byte{0b00000000},
  };
};

TEST_F(StaticHuffmanCodeTest, TestEncoding) {
  auto encoded = shc_.Encode(text_);
  ASSERT_EQ(encoded.size(), 6);
  ASSERT_EQ(encoded, translation_);
}

TEST_F(StaticHuffmanCodeTest, TestEncodeDecode) {
  auto encoded = shc_.Encode(text_);
  auto decoded = shc_.Decode(encoded);
  ASSERT_EQ(decoded.size(), 15);
  ASSERT_EQ(decoded, text_);
}

TEST_F(StaticHuffmanCodeTest, FunnyCharmander) {
  std::string charmander =
      "\
                  _.--\"\"`-..\n\
                ,'          `.\n\
              ,'          __  `.\n\
             /|          \" __   \n\
            , |           / |.   .\n\
            |,'          !_.'|   |\n\
          ,'             '   |   |\n\
         /              |`--'|   |\n\
        |                `---'   |\n\
         .   ,                   |                       ,\".\n\
          ._     '           _'  |                    , '  `\n\
      `.. `.`-...___,...---\"\"    |       __,.        ,`\"   L,|\n\
      |, `- .`._        _,-,.'   .  __.-'-. /        .   ,    \n\
    -:..     `. `-..--_.,.<       `\"      / `.        `-/ |   .\n\
      `,         \"\"\"\"'     `.              ,'         |   |  ',,\n\
        `.      '            '            /          '    |'. |/\n\
          `.   |              \\       _,-'           |       ''\n\
            `._'               \\   '\"\\                .      |\n\
               |                '     \\                `._  ,'\n\
               |                 '     \\                 .'|\n\
               |                 .      \\                | |\n\
               |                 |       L              ,' |\n\
               `                 |       |             /   '\n\
                                |       |           ,'   /\n\
              ,'                |  _.._ ,-..___,..-'    ,'\n\
             /     .             .      `!             ,j'\n\
            /       `.          /        .           .'/\n\
           .          `.       /         |        _.'.'\n\
            `.          7`'---'          |------\"'_.'\n\
           _,.`,_     _'                ,''-----\"'\n\
       _,-_    '       `.     .'      ,\n\
       -\" /`.         _,'     | _  _  _.|\n\
        \"\"--'---\"\"\"\"\"'        `' '! |! /\n\
                                `\" \" -' mh\n\
    \n\
    \n";
  auto encoded_charmander = shc_.Encode(charmander);
  auto decoded_charmander = shc_.Decode(encoded_charmander);
  ASSERT_EQ(decoded_charmander, charmander)
      << decoded_charmander.size() << " " << charmander.size();
}

}  // namespace doht::shc