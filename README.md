# MySTL

一个 C++23 兼容的 STL 实现库

## 特性

- **C++23 标准兼容**：完整支持 C++23 特性
- **模块化设计**：清晰的模块划分和实现细节隔离
- **完整实现**：包含容器、算法、迭代器、内存管理、Ranges 等完整模块

## 构建要求

- CMake 3.20 或更高版本
- C++23 兼容的编译器：
  - GCC 13+ (推荐)
  - Clang 17+ (推荐)
  - MSVC 19.30+ (部分特性可能受限)

## 项目结构

```
mystl/
├── include/mystl/          # 头文件目录
│   ├── memory/             # 内存管理
│   ├── iterator/           # 迭代器
│   ├── container/          # 容器
│   ├── algorithm/          # 算法
│   ├── ranges/             # Ranges 模块
│   └── stl.hpp             # 聚合头文件
├── test/                   # 测试代码
├── examples/               # 示例代码
├── cmake/                  # CMake 模块
└── CMakeLists.txt          # 主 CMake 配置
```