cmake_minimum_required(VERSION 3.22)

# 项目定义，包含版本号
project(MySTL 
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "A C++23 compatible STL implementation library"
)

# C++ 标准设置
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置默认构建类型
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

include(cmake/CompilerOptions.cmake)

add_library(mystl INTERFACE)
add_library(mystl::mystl ALIAS mystl)

target_include_directories(mystl INTERFACE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(mystl INTERFACE cxx_std_23)

set_target_properties(mystl PROPERTIES
    EXPORT_NAME mystl
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

set(MYSTL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

enable_testing()

option(MYSTL_BUILD_TESTS "Build tests" ON)
option(MYSTL_BUILD_EXAMPLES "Build examples" ON)

if(MYSTL_BUILD_TESTS)
    add_subdirectory(test)
endif()

if(MYSTL_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

include(cmake/InstallConfig.cmake)
