// Doth Libray (C) by Marcos Oliveira

#include <gtest/gtest.h>

#include <doht/hello.h>

TEST(HelloTest, TestHelloString){
    ASSERT_EQ(hello(), "Hello doht!");
}