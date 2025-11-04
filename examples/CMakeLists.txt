# 示例可执行文件
add_executable(example_basic basic_example.cpp)
target_link_libraries(example_basic PRIVATE mystl::mystl)
set_target_properties(example_basic PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/examples
)
