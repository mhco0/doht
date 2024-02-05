# Doth Libray (C) by Marcos Oliveira

enable_testing()

include(GoogleTest)
include(CTest)

add_executable(hello_teste "hello.test.cpp")

target_link_libraries(hello_teste PUBLIC hello GTest::gtest GTest::gtest_main)

gtest_discover_tests(hello_teste WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                                                   DISCOVERY_TIMEOUT 30)
