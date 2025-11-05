# Warnings configuration for MySTL
# Provides warning flags for different compilers

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  # GCC and Clang warnings
  add_compile_options(-Wall -Wextra -Wpedantic)
  add_compile_options(-Wconversion -Wsign-conversion)
  add_compile_options(-Wno-unused-parameter)  # Allow unused parameters in interfaces
  
  # Additional strict warnings
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 11)
      add_compile_options(-Wnull-dereference -Wduplicated-cond -Wduplicated-branches)
    endif()
  endif()
  
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wunreachable-code -Wmissing-prototypes)
  endif()
elseif(MSVC)
  # MSVC warnings
  add_compile_options(/W4 /permissive-)
  add_compile_options(/wd4127)  # conditional expression is constant (used in templates)
  add_compile_options(/wd4100)  # unreferenced formal parameter
endif()

