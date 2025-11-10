cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/unit/*.cpp)

add_executable(mystl_tests ${TEST_SOURCES})
target_link_libraries(mystl_tests PRIVATE mystl)
target_include_directories(mystl_tests PRIVATE ${CMAKE_SOURCE_DIR})

# Windows平台需要指定控制台应用程序
# 移除之前的方法，改为强制链接为console应用
if (WIN32)
  target_link_libraries(mystl_tests PRIVATE -Wl,--subsystem,console)
endif()

if (MYSTL_ENABLE_ASAN AND NOT MSVC)
  target_compile_options(mystl_tests PRIVATE -fsanitize=address,undefined)
  target_link_options(mystl_tests PRIVATE -fsanitize=address,undefined)
endif()

# MSVC exception handling and RTTI for tests
if (MSVC)
  target_compile_options(mystl_tests PRIVATE /EHsc /GR)
endif()

add_test(NAME mystl_tests COMMAND mystl_tests)

# Property tests
file(GLOB_RECURSE PROP_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/property/*.cpp)
if (PROP_SOURCES)
  add_executable(mystl_prop ${PROP_SOURCES})
  target_link_libraries(mystl_prop PRIVATE mystl)
  target_include_directories(mystl_prop PRIVATE ${CMAKE_SOURCE_DIR})
  if (MYSTL_ENABLE_ASAN AND NOT MSVC)
    target_compile_options(mystl_prop PRIVATE -fsanitize=address,undefined)
    target_link_options(mystl_prop PRIVATE -fsanitize=address,undefined)
  endif()
  add_test(NAME mystl_prop COMMAND mystl_prop)
endif()

# Benchmarks
file(GLOB_RECURSE BENCH_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/*.cpp)
if (BENCH_SOURCES)
  add_executable(mystl_bench ${BENCH_SOURCES})
  target_link_libraries(mystl_bench PRIVATE mystl)
  target_include_directories(mystl_bench PRIVATE ${CMAKE_SOURCE_DIR})
endif()

# Fuzz target (Clang + libFuzzer recommended)
option(MYSTL_ENABLE_FUZZ "Build fuzz targets (requires Clang/libFuzzer)" OFF)
if (MYSTL_ENABLE_FUZZ)
  file(GLOB_RECURSE FUZZ_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/fuzz/*.cpp)
  if (FUZZ_SOURCES)
    add_executable(mystl_fuzz ${FUZZ_SOURCES})
    target_link_libraries(mystl_fuzz PRIVATE mystl)
    target_include_directories(mystl_fuzz PRIVATE ${CMAKE_SOURCE_DIR})
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      target_compile_options(mystl_fuzz PRIVATE -fsanitize=fuzzer,address,undefined)
      target_link_options(mystl_fuzz PRIVATE -fsanitize=fuzzer,address,undefined)
    endif()
  endif()
endif()


