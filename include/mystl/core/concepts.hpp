//
// MySTL Core Module - Concepts
// C++23 compatible
//
// ============================================================================
// 模块位置与架构说明
// ============================================================================
//
// 文件路径：stl/core/concepts.hpp
// 所属模块：Core 模块（stl/core/）
//
// 本文件提供 C++20 概念（Concepts）支持，为标准库和用户代码提供类型约束。
// 概念是 C++20 引入的强大的类型系统工具，用于表达模板参数的约束条件。
//
// ============================================================================
// 功能范围
// ============================================================================
//
// 本文件应提供标准库 <concepts> 的功能，包括但不限于：
//
// 1. 核心语言概念（Core Language Concepts）
//    - same_as, derived_from, convertible_to
//    - common_reference_with, common_with
//    - integral, signed_integral, unsigned_integral
//    - floating_point
//    - assignable_from, swappable, swappable_with
//
// 2. 比较概念（Comparison Concepts）
//    - equality_comparable, equality_comparable_with
//    - totally_ordered, totally_ordered_with
//    - three_way_comparable, three_way_comparable_with
//
// 3. 对象概念（Object Concepts）
//    - movable, copyable, semiregular, regular
//    - invocable, regular_invocable
//    - predicate, relation, equivalence_relation, strict_weak_order
//
// 4. 可调用概念（Callable Concepts）
//    - invocable, regular_invocable
//    - predicate, relation
//    - equivalence_relation, strict_weak_order
//
// 注：部分概念依赖于 type_traits.hpp 和 __detail/invoke.hpp
//
// ============================================================================
// 模块依赖关系
// ============================================================================
//
// 当前文件依赖：
//   - <mystl/config.hpp>                    - 编译配置和特性检测
//   - <mystl/core/type_traits.hpp>           - 类型特征支持（type_traits 依赖 concepts_base）
//   - <mystl/core/__detail/concepts_base.hpp> - 基础概念（间接依赖，通过 type_traits）
//   - <mystl/core/__detail/invoke.hpp>        - 调用封装（用于 invocable 相关概念）
//
// 依赖当前文件的模块：
//   - Ranges 模块（所有范围概念依赖基础概念）
//   - Iterator 模块（迭代器概念）
//   - Algorithm 模块（算法约束）
//   - Container 模块（容器概念约束）
//
// ============================================================================
// 实现注意事项
// ============================================================================
//
// 1. 充分利用 C++20 concepts 语法，提供清晰的类型约束
// 2. 与 type_traits.hpp 协同工作，但优先使用概念进行约束
// 3. 确保概念组合的语义正确性（如 regular = semiregular + equality_comparable）
// 4. 为自定义类型提供扩展点，支持用户定义概念
// 5. 遵循 C++23 标准，支持最新的概念特性
//
// ============================================================================

#ifndef MYSTL_CORE_CONCEPTS_HPP
#define MYSTL_CORE_CONCEPTS_HPP

#include <mystl/config.hpp>
#include <mystl/core/type_traits.hpp>

// TODO: 实现概念定义
// 1. 核心语言概念
// 2. 比较概念
// 3. 对象概念
// 4. 可调用概念

#endif // MYSTL_CORE_CONCEPTS_HPP
