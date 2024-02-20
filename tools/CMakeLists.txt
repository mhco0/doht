find_package(CLI11 CONFIG REQUIRED)

add_executable(doth_console "doth_console.cpp")
target_link_libraries(doth_console PUBLIC shc CLI11::CLI11)
