# Doth Libray (C) by Marcos Oliveira

enable_testing()

include(GoogleTest)
include(CTest)

add_executable(shc_test "shc.test.cpp")

target_link_libraries(shc_test PUBLIC shc GTest::gtest GTest::gtest_main)

gtest_discover_tests(shc_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                                                DISCOVERY_TIMEOUT 30)
