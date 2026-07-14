cmake_minimum_required(VERSION 3.15)
project(DSACppPractice LANGUAGES CXX)

# Force C++17 or C++20 for modern DSA features (like std::string_view, structured bindings)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable compiler warnings to catch potential bugs early
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Global include directory so your files can share custom templates/helpers easily
include_directories(include)

# Add subdirectories for your categories
add_subdirectory(src/01-introduction)
add_subdirectory(src/02-mathematics)
add_subdirectory(src/03-bit-manipulation)
# Add other categories as you create them