# MySTL

一个手搓的 C++23 兼容的简易 STL 实现库

## 设计原则

- **遵循《Effective C++》和《More Effective C++》**：最佳实践和设计模式
- **异常安全**：异常仅用于不可恢复错误和构造函数失败（抛出而非半初始化状态）
- **代码质量**：遵循 Google C++ 编码规范和《代码整洁之道》
- **性能优先**：禁止未测量的过早优化，禁止牺牲安全性的 hack
- **标准兼容**：接口与语义尽量与 STL 同构，必要处清晰记录差异

## 特性

- **C++23 标准兼容**：完整支持 C++23 特性
- **模块化设计**：清晰的模块划分和实现细节隔离（`__details` 目录）
- **完整实现**：包含容器、算法、迭代器、内存管理、Ranges 等完整模块
- **自定义测试框架**：轻量级测试工具，无需外部依赖

## 构建要求

- CMake 3.20 或更高版本
- C++23 兼容的编译器：
  - GCC 13+ (推荐)
  - Clang 17+ (推荐)
  - MSVC 19.30+ (部分特性可能受限)

## 快速开始

```bash
# 配置构建
mkdir build && cd build
cmake ..

# 编译
cmake --build .

# 运行测试
ctest
```

## 项目结构

```
MySTL/
├── include/mystl/          # 头文件目录
│   ├── config/             # 配置和特性检测
│   ├── core/               # 核心工具（utility、assert、异常安全）
│   ├── memory/             # 内存管理（allocator、智能指针、uninitialized）
│   ├── iterator/           # 迭代器（concepts、traits、facade、适配器）
│   ├── containers/         # 容器
│   │   ├── __details/      # 内部实现细节（红黑树、哈希表等）
│   │   └── adapters/       # 容器适配器
│   ├── algorithms/         # 算法（非修改、修改、排序、堆、数值）
│   └── ranges/             # Ranges 模块（concepts、views）
├── tests/                  # 测试代码
│   ├── framework/          # 自定义测试框架
│   └── unit/               # 单元测试
├── examples/               # 示例代码
├── cmake/                  # CMake 模块（Warnings、Sanitizers、CompilerFeatures）
└── CMakeLists.txt          # 主 CMake 配置
```

## 容器列表

### 顺序容器 (Sequence Containers)

- ✅ `vector` - 动态数组
- ✅ `deque` - 双端队列
- ✅ `array` (C++11) - 固定大小数组
- ✅ `list` (C++11) - 双向链表
- ✅ `forward_list` (C++11) - 单向链表
- ✅ `inplace_vector` (C++26) - 固定容量向量（栈分配）
- ✅ `hive` (C++26) - 稳定引用容器，O(1) 删除

### 关联容器 (Associative Containers)

- ✅ `map` - 有序映射（红黑树）
- ✅ `multimap` - 有序多值映射
- ✅ `set` - 有序集合
- ✅ `multiset` - 有序多值集合

### 无序关联容器 (Unordered Associative Containers)

- ✅ `unordered_map` (C++11) - 哈希映射
- ✅ `unordered_multimap` (C++11) - 哈希多值映射
- ✅ `unordered_set` (C++11) - 哈希集合
- ✅ `unordered_multiset` (C++11) - 哈希多值集合

### 容器适配器 (Container Adapters)

- ✅ `stack` - 栈
- ✅ `queue` - 队列
- ✅ `priority_queue` - 优先队列

### 视图容器 (View Containers)

- ✅ `span` (C++20) - 连续内存视图
- ✅ `mdspan` (C++23) - 多维内存视图
- ✅ `string_view` - 字符串视图

### 字符串

- ✅ `string` - 字符串（SBO 优化）

## 实现状态

当前为占位阶段，所有头文件已创建，包含详细注释说明设计意图。后续将按以下顺序实现：

1. **阶段 0**：基础设施（配置、断言、工具函数）
2. **阶段 1**：迭代器与内存基础（traits、allocator、uninitialized）
3. **阶段 2**：顺序容器与字符串视图（vector、array、span、string_view、string）
4. **阶段 3**：链式与分段容器（list、forward_list、deque、适配器）
5. **阶段 4**：关联容器（红黑树实现的 map/set 系列）
6. **阶段 5**：无序关联容器（哈希表实现的 unordered_* 系列）
7. **阶段 6**：算法库（非修改、修改、排序、堆、数值）
8. **阶段 7**：Ranges 视图子集与完善

## 文档

- 详细开发计划与路线图请见：`docs/plan.md`
- Memory 模块依赖关系分析与开发步骤：`docs/memory_dependencies.md`

## 构建选项

```bash
# 启用 AddressSanitizer
cmake -DMYSTL_ENABLE_ASAN=ON ..

# 启用 UndefinedBehaviorSanitizer
cmake -DMYSTL_ENABLE_UBSAN=ON ..

# 同时启用多个 sanitizers
cmake -DMYSTL_ENABLE_ASAN=ON -DMYSTL_ENABLE_UBSAN=ON ..
```

## 测试

项目包含自定义轻量级测试框架，位于 `tests/framework/mystl_test.hpp`。

```cpp
#include "tests/framework/mystl_test.hpp"

MYSTL_TEST("container.vector", {
  mystl::vector<int> v;
  v.push_back(42);
  MYSTL_EXPECT_EQ(v.size(), 1);
});
```

运行测试：
```bash
cd build
./tests/mystl_tests
```
