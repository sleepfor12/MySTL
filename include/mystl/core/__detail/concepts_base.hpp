//
// MySTL Core Module - Concepts Base (Implementation Detail)
// C++23 compatible
//
// ============================================================================
// 模块位置与架构说明
// ============================================================================
//
// 文件路径：stl/core/__detail/concepts_base.hpp
// 所属模块：Core 模块（stl/core/__detail/）
//
// 本文件提供最基础的概念定义，是 concepts.hpp 和 type_traits.hpp 的基础。
// 这些是最底层的概念，不依赖于 type_traits，避免循环依赖。
//
// ============================================================================
// 功能范围
// ============================================================================
//
// 本文件应提供以下最基础的概念：
//
// 1. 类型关系概念（Type Relationship Concepts）
//    - same_as - 两个类型完全相同
//    - derived_from - 继承关系检查
//    - convertible_to - 类型可转换
//
// 2. 其他基础概念（可选）
//    - 可能包含其他不依赖 type_traits 的基础概念
//
// 注：这些概念为 type_traits.hpp 和 concepts.hpp 提供基础支持
//
// ============================================================================
// 模块依赖关系
// ============================================================================
//
// 当前文件依赖：
//   - <mystl/config.hpp>          - 编译配置和特性检测
//   - <mystl/core/__detail/meta.hpp> - 元编程工具（如 void_t）
//
// 依赖当前文件的模块：
//   - Core 模块的公共接口（type_traits.hpp, concepts.hpp）
//   - type_traits.hpp 需要这些基础概念来构建类型特征
//
// ============================================================================
// 实现注意事项
// ============================================================================
//
// 1. 这些是内部实现细节，不应直接暴露给用户
// 2. 避免循环依赖，不依赖 type_traits.hpp
// 3. 使用最基础的 SFINAE 技术实现
// 4. 充分利用 C++20 concepts 语法
// 5. 为上层模块提供稳定的基础
//
// ============================================================================

#ifndef MYSTL_CORE_DETAIL_CONCEPTS_BASE_HPP
#define MYSTL_CORE_DETAIL_CONCEPTS_BASE_HPP

#include <mystl/config.hpp>
#include <mystl/core/__detail/meta.hpp>

// TODO: 实现基础概念定义
// 1. same_as - 类型完全相同检查
// 2. derived_from - 继承关系检查
// 3. convertible_to - 类型可转换检查
// 这些概念为 type_traits.hpp 提供基础支持

#endif // MYSTL_CORE_DETAIL_CONCEPTS_BASE_HPP
