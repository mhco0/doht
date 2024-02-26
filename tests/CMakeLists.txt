# Doth Libray (C) by Marcos Oliveira

enable_testing()

include(GoogleTest)
include(CTest)

add_executable(shc_test "shc.test.cpp")

target_link_libraries(shc_test PUBLIC shc GTest::gtest GTest::gtest_main)

gtest_discover_tests(shc_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                                                DISCOVERY_TIMEOUT 30)

add_executable(data_manager_test "data_manager.test.cpp")

target_link_libraries(data_manager_test PUBLIC data_manager GTest::gtest
                                               GTest::gtest_main)

gtest_discover_tests(data_manager_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                                                         DISCOVERY_TIMEOUT 30)
