# 测试可执行文件
add_executable(test_main test_main.cpp)
target_link_libraries(test_main PRIVATE mystl::mystl)
set_target_properties(test_main PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test
)
add_test(NAME MySTL_BasicTests COMMAND test_main)
