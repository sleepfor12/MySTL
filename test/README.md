# MySTL 测试框架

## 概述

MySTL 提供轻量级、自包含的测试框架，支持单元测试、性能基准测试和测试结果报告。

## 文件结构

```
test/
├── test_framework.hpp      # 测试框架核心接口
├── test_macros.hpp         # 测试宏定义
├── test_main.cpp          # 测试主程序
├── __detail/
│   ├── test_registry.hpp  # 测试用例注册机制
│   ├── assert_impl.hpp    # 断言实现
│   ├── timer.hpp          # 性能计时工具
│   └── benchmark_tools.hpp # 基准测试工具
└── CMakeLists.txt         # 测试构建配置
```

## 使用方法

### 基本测试用例

```cpp
#include "test_framework.hpp"
#include "test_macros.hpp"

TEST(SuiteName, TestName) {
    ASSERT_TRUE(condition);
    EXPECT_EQ(expected, actual);
}
```

### 断言宏

**ASSERT 系列**（失败时立即终止测试）：
- `ASSERT_TRUE(condition)` - 断言条件为真
- `ASSERT_FALSE(condition)` - 断言条件为假
- `ASSERT_EQ(expected, actual)` - 断言相等
- `ASSERT_NE(expected, actual)` - 断言不等
- `ASSERT_LT/LE/GT/GE(lhs, rhs)` - 断言比较关系
- `ASSERT_STREQ(expected, actual)` - 断言字符串相等
- `ASSERT_THROW(statement, exception_type)` - 断言抛出异常
- `ASSERT_NO_THROW(statement)` - 断言不抛出异常

**EXPECT 系列**（失败时继续执行）：
- `EXPECT_TRUE/FALSE(condition)`
- `EXPECT_EQ/NE/LT/LE/GT/GE(lhs, rhs)`
- `EXPECT_STREQ(expected, actual)`

### 性能测试

```cpp
#include "__detail/timer.hpp"
#include "__detail/benchmark_tools.hpp"

TEST(PerformanceTest, BenchmarkExample) {
    TIMER_SCOPE("test_operation");
    // 执行操作
}

TEST(PerformanceTest, BenchmarkStats) {
    BENCHMARK("operation_name", 100);
    auto stats = BENCHMARK_RUN({
        // 执行操作
    });
    MYSTL_UNIQUE_NAME(bench).print_stats(stats);
}
```

### 测试辅助宏

- `SKIP_TEST(reason)` - 跳过当前测试
- `FAIL_TEST(message)` - 强制测试失败
- `SUCCEED_TEST()` - 标记测试成功

## 运行测试

```bash
# 构建测试
cmake --build build --target test_main

# 运行测试
./build/bin/test/test_main

# 或使用 CTest
ctest -C Release
```

## 测试结果

测试框架会输出：
- 每个测试用例的执行状态
- 测试统计信息（总数、通过、失败、跳过）
- 失败测试的详细错误信息

## 设计特点

1. **轻量级**：不依赖外部测试库（如 Google Test）
2. **自动注册**：使用静态初始化自动注册测试用例
3. **异常安全**：捕获所有异常并报告
4. **性能支持**：内置性能计时和基准测试工具
5. **C++23 兼容**：充分利用现代 C++ 特性

