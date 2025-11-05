//
// MySTL Core Module - Three-Way Comparison
// C++23 compatible
//
// ============================================================================
// 模块位置与架构说明
// ============================================================================
//
// 文件路径：stl/core/compare.hpp
// 所属模块：Core 模块（stl/core/）
//
// 本文件提供 C++20 三路比较运算符（<=>）支持。
// 三路比较是现代 C++ 中关系运算符的基础。
//
// ============================================================================
// 功能范围
// ============================================================================
//
// 本文件应提供标准库 <compare> 的功能，包括但不限于：
//
// 1. 比较类别（Comparison Categories）
//    - strong_ordering, weak_ordering, partial_ordering
//    - strong_equality, weak_equality (C++20，已弃用)
//
// 2. 命名比较函数对象
//    - compare_three_way (C++20)
//    - compare_partial_order_fallback (C++23)
//    - compare_strong_order_fallback (C++23)
//    - compare_weak_order_fallback (C++23)
//
// 3. 比较函数
//    - strong_order, weak_order, partial_order
//    - compare_strong_order_fallback, compare_weak_order_fallback
//    - compare_partial_order_fallback
//
// 4. 常用类型的比较类别
//    - 内置类型的比较类别定义
//    - 指针的比较类别
//
// 5. 类型特征（可能在 type_traits.hpp 中）
//    - common_comparison_category
//    - compare_three_way_result
//
// 注：部分实现细节可能在 __detail/compare_impl.hpp 中
//
// ============================================================================
// 模块依赖关系
// ============================================================================
//
// 当前文件依赖：
//   - <mystl/config.hpp>                    - 编译配置和特性检测
//   - <mystl/core/concepts.hpp>              - 概念支持（三路比较相关概念）
//   - <mystl/core/type_traits.hpp>           - 类型特征支持
//   - <mystl/core/__detail/compare_impl.hpp> - 内部实现细节
//
// 依赖当前文件的模块：
//   - Core 模块（pair, tuple 的关系运算符）
//   - Container 模块（容器的关系运算符）
//   - Algorithm 模块（比较相关的算法）
//   - Ranges 模块（范围比较）
//
// ============================================================================
// 实现注意事项
// ============================================================================
//
// 1. 正确实现三种比较类别（strong, weak, partial）
// 2. 确保与标准库的语义一致
// 3. 提供合理的回退机制（fallback）
// 4. 充分利用 C++20 的 <=> 运算符
// 5. 考虑 C++23 的新特性
//
// ============================================================================

#ifndef MYSTL_CORE_COMPARE_HPP
#define MYSTL_CORE_COMPARE_HPP

#include <mystl/config.hpp>
#include <mystl/core/concepts.hpp>
#include <mystl/core/type_traits.hpp>

// TODO: 实现三路比较支持
// 1. 比较类别类型（strong_ordering, weak_ordering, partial_ordering）
// 2. 命名比较函数对象（compare_three_way 等）
// 3. 比较函数（strong_order, weak_order, partial_order）
// 4. 回退函数（fallback functions）
// 5. 常用类型的比较类别定义
// 部分实现可委托给 __detail/compare_impl.hpp

#endif // MYSTL_CORE_COMPARE_HPP
