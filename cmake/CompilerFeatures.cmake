# Compiler features detection and configuration

# Detect C++ standard features
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 13.0)
    message(WARNING "GCC < 13.0 may not fully support C++23 features")
  endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 16.0)
    message(WARNING "Clang < 16.0 may not fully support C++23 features")
  endif()
elseif(MSVC)
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19.30)
    message(WARNING "MSVC < 19.30 may not fully support C++23 features")
  endif()
endif()

# Check for required features
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++23" COMPILER_SUPPORTS_CXX23)
if(NOT COMPILER_SUPPORTS_CXX23)
  message(WARNING "Compiler may not support C++23, some features may be unavailable")
endif()

