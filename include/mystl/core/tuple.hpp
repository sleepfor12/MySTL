//
// MySTL Core Module - Tuple
// C++23 compatible
//
// ============================================================================
// 模块位置与架构说明
// ============================================================================
//
// 文件路径：stl/core/tuple.hpp
// 所属模块：Core 模块（stl/core/）
//
// 本文件提供元组（tuple）类型和操作，支持固定大小的异构值集合。
// tuple 是函数式编程和元编程的重要工具。
//
// ============================================================================
// 功能范围
// ============================================================================
//
// 本文件应提供标准库 <tuple> 的功能，包括但不限于：
//
// 1. 核心类型
//    - tuple, tuple_size, tuple_element
//    - tuple_size_v, tuple_element_t (C++14/17)
//
// 2. 构造和赋值
//    - 默认构造、拷贝构造、移动构造
//    - 从 pair 构造
//    - 从不同类型的 tuple 构造（转换构造）
//    - 赋值操作符
//
// 3. 元素访问
//    - get (通过索引和类型)
//    - apply (C++17)
//    - make_from_tuple (C++17)
//
// 4. 关系运算符
//    - ==, !=, <, <=, >, >= (C++20 三路比较 <=>)
//
// 5. 辅助函数
//    - make_tuple, forward_as_tuple
//    - tie, tuple_cat
//
// 6. 特殊化
//    - pair 的 tuple_size 和 tuple_element 特化
//    - array 的 tuple_size 和 tuple_element 特化（可选）
//
// ============================================================================
// 模块依赖关系
// ============================================================================
//
// 当前文件依赖：
//   - <mystl/config.hpp>          - 编译配置和特性检测
//   - <mystl/core/type_traits.hpp> - 类型特征支持
//   - <mystl/core/utility.hpp>     - forward, move 等工具
//   - <mystl/core/pair.hpp>        - pair 类型（相互依赖，需前向声明）
//   - <mystl/core/__detail/meta.hpp> - 元编程工具（index_sequence 等，必需）
//
// 依赖当前文件的模块：
//   - 所有使用多返回值的模块
//   - Algorithm 模块（某些算法返回 tuple）
//   - Ranges 模块（zip_view 等需要 tuple）
//   - Utility 模块（variant 的实现可能需要 tuple）
//
// ============================================================================
// 实现注意事项
// ============================================================================
//
// 1. tuple 的实现需要递归模板技术，注意编译时性能
// 2. 支持空 tuple（tuple<>），需要特殊处理
// 3. 正确实现引用折叠和完美转发
// 4. 确保异常安全性（strong exception safety）
// 5. tuple_cat 需要处理大量参数时的模板实例化问题
// 6. 充分利用 C++20/23 特性（如 constexpr, concepts）
//
// ============================================================================

#ifndef MYSTL_CORE_TUPLE_HPP
#define MYSTL_CORE_TUPLE_HPP

#include <mystl/config.hpp>
#include <mystl/core/type_traits.hpp>
#include <mystl/core/utility.hpp>
#include <mystl/core/__detail/meta.hpp>

// TODO: 实现 tuple 类型和操作
// 1. tuple 核心类型定义
// 2. tuple_size 和 tuple_element 特化
// 3. 构造和赋值操作
// 4. get 函数（索引和类型版本）
// 5. make_tuple, forward_as_tuple, tie, tuple_cat
// 6. 关系运算符（C++20 三路比较）
// 7. apply 和 make_from_tuple (C++17)

#endif // MYSTL_CORE_TUPLE_HPP
