cmake_minimum_required(VERSION 3.15)
project(curl-clone-client LANGUAGES CXX)

# 1. Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Include directories (-I)
# Use forward slashes even on Windows for CMake compatibility
include_directories("C:/Programming/Projects/curl-clone/include")

# 3. Collect source files (lib/*.cpp and src/main.cpp)
file(GLOB_RECURSE ALL_SOURCES 
  "lib/*.cpp" 
  "src/main.cpp"
)
set(SOURCES ${LIB_SOURCES} "src/main.cpp")

# 4. Define the executable (-o client.exe)
add_executable(client ${SOURCES})

# 5. Link libraries (-lWs2_32 -lssl -lcrypto)
# On Windows, we link to Ws2_32 for networking
target_link_libraries(client PRIVATE Ws2_32 ssl crypto)