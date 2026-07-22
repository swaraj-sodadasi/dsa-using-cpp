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
add_subdirectory(src/04-recursion)
add_subdirectory(src/05-arrays)
add_subdirectory(src/06-searching)
add_subdirectory(src/07-sorting)
add_subdirectory(src/08-matrix)
add_subdirectory(src/09-hashing)
add_subdirectory(src/10-strings)
add_subdirectory(src/11-linkedlist)
add_subdirectory(src/12-stack)
add_subdirectory(src/13-queue)
# Add other categories as you create them

# ==============================================================================
# REROUTING OUTPUT EXECUTABLES TO A CENTRAL /documentation-bin DIRECTORY
# ==============================================================================
# Wipes out mirror directory routing behavior and places every single compiled 
# executable target cleanly inside the 'build/bin' directory.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/documentation-bin)

# ==============================================================================
# INTEGRATING DOXYGEN AUTOMATION
# ==============================================================================
find_package(Doxygen)

if(DOXYGEN_FOUND)
    # Appends a custom target named 'docs_gen' into CMake
    add_custom_target(docs_gen
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating project documentation with Doxygen"
        VERBATIM
    )
else()
    message(STATUS "Doxygen engine not found. Documentation targets will not be built.")
endif()
