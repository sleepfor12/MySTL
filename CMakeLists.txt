cmake_minimum_required(VERSION 3.20)
project(MySTL 
  VERSION 0.1.0
  LANGUAGES CXX
  DESCRIPTION "A minimal STL implementation for C++23"
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include CMake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Configure compiler features
include(CompilerFeatures)

# Library interface target
add_library(mystl INTERFACE)
add_library(mystl::mystl ALIAS mystl)
target_include_directories(mystl INTERFACE 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# Apply warnings
include(Warnings)

# Apply sanitizers (for tests/examples, not the library itself)
include(Sanitizers)

enable_testing()

# Common definitions for consumers
target_compile_definitions(mystl INTERFACE
  $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS=1>
  $<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS=1>
)

# Per-compiler compile options (interface propagation)
target_compile_options(mystl INTERFACE
  $<$<CXX_COMPILER_ID:MSVC>:/permissive- /Zc:__cplusplus>
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion>
  $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion>
)

# Installation
include(GNUInstallDirs)
install(TARGETS mystl
  EXPORT MySTLTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_subdirectory(examples)
add_subdirectory(tests)


