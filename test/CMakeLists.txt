# 测试可执行文件
add_executable(test_main test_main.cpp)

target_link_libraries(test_main PRIVATE mystl::mystl)

# 添加测试目录到包含路径（用于测试框架头文件）
target_include_directories(test_main PRIVATE
    ${CMAKE_SOURCE_DIR}/test
)

set_target_properties(test_main PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test
)

# 添加测试
add_test(NAME MySTL_BasicTests COMMAND test_main)

# 设置测试工作目录
set_tests_properties(MySTL_BasicTests PROPERTIES
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test
)
