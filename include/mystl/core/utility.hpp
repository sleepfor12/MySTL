//
// MySTL Core Module - Utility
// C++23 compatible
//
// ============================================================================
// 模块位置与架构说明
// ============================================================================
//
// 文件路径：stl/core/utility.hpp
// 所属模块：Core 模块（stl/core/）
//
// 本文件提供通用工具函数和类型，是其他模块广泛依赖的基础组件。
// 包含完美转发、移动语义、类型操作等核心功能。
//
// ============================================================================
// 功能范围
// ============================================================================
//
// 本文件应提供标准库 <utility> 的核心功能，包括但不限于：
//
// 1. 完美转发（Perfect Forwarding）
//    - forward, forward_like (C++23)
//
// 2. 移动语义（Move Semantics）
//    - move, move_if_noexcept
//
// 3. 交换操作（Swap Operations）
//    - swap, exchange
//
// 4. 类型操作（Type Operations）
//    - declval
//
// 5. 整数序列（Integer Sequences）
//    - integer_sequence, index_sequence
//    - make_integer_sequence, make_index_sequence
//    - index_sequence_for
//    注：可能委托给 __detail/meta.hpp 实现
//
// 6. 其他工具
//    - as_const (C++17)
//    - to_underlying (C++23)
//
// 注：move.hpp 和 swap.hpp 的功能可以合并到本文件中
//
// ============================================================================
// 模块依赖关系
// ============================================================================
//
// 当前文件依赖：
//   - <mystl/config.hpp>          - 编译配置和特性检测
//   - <mystl/core/type_traits.hpp> - 类型特征支持
//   - <mystl/core/__detail/meta.hpp> - 元编程工具（index_sequence 等，可能使用）
//
// 依赖当前文件的模块：
//   - 所有其他模块（utility 是最基础的模块之一）
//   - Container 模块（容器的构造、赋值、交换都需要 utility）
//   - Algorithm 模块（算法参数传递）
//   - Memory 模块（智能指针的移动语义）
//
// ============================================================================
// 实现注意事项
// ============================================================================
//
// 1. forward 和 move 必须使用引用折叠规则正确实现
// 2. 确保 noexcept 正确性，特别是 move_if_noexcept
// 3. swap 需要支持 ADL（Argument-Dependent Lookup）
// 4. 遵循标准库语义，确保与 std:: 版本兼容
// 5. 充分利用 C++23 新特性（如 forward_like）
//
// ============================================================================

#ifndef MYSTL_CORE_UTILITY_HPP
#define MYSTL_CORE_UTILITY_HPP

#include <mystl/config.hpp>
#include <mystl/core/type_traits.hpp>
// 注：index_sequence 等可能从 __detail/meta.hpp 或直接在此实现

// TODO: 实现工具函数和类型
// 1. 完美转发函数（forward, forward_like）
// 2. 移动语义函数（move, move_if_noexcept）
// 3. 交换操作（swap, exchange）
// 4. 类型操作（declval, as_const）
// 5. 整数序列（index_sequence 等，或委托给 __detail/meta.hpp）

#endif // MYSTL_CORE_UTILITY_HPP
