cmake_minimum_required(VERSION 3.15)
project(curl-clone-client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generates compile_commands.json automatically for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use project-relative paths so it works for others too
include_directories("${CMAKE_SOURCE_DIR}/include")

# Collect all .cpp files in lib and src
file(GLOB_RECURSE SOURCES "lib/*.cpp" "src/*.cpp")

add_executable(client ${SOURCES})

# Linking
target_link_libraries(client PRIVATE Ws2_32 ssl crypto)