//
// MySTL Core Module - Type Traits
// C++23 compatible
//
// ============================================================================
// 模块位置与架构说明
// ============================================================================
//
// 文件路径：stl/core/type_traits.hpp
// 所属模块：Core 模块（stl/core/）
//
// Core 模块是 MySTL 的基础核心模块，提供所有其他模块依赖的底层基础设施。
// 该模块包含类型特征、概念、工具类、元编程支持等基础组件。
//
// ============================================================================
// Core 模块完整文件清单
// ============================================================================
//
// 核心头文件（stl/core/）：
//   ✓ type_traits.hpp      - 类型特征（当前文件）
//   ✗ concepts.hpp         - C++20 概念支持（依赖 type_traits）
//   ✗ utility.hpp          - 工具函数和类型（forward, move, exchange 等）
//   ✗ tuple.hpp            - 元组类型和操作（依赖 pair 和 meta）
//   ✗ pair.hpp             - 配对类型（依赖 utility）
//   ✗ move.hpp             - 移动语义支持（可与 utility.hpp 合并）
//   ✗ swap.hpp             - 交换操作支持（可与 utility.hpp 合并）
//   ✗ compare.hpp          - 三路比较运算符 <=> 支持（依赖 concepts）
//   ✗ initializer_list.hpp - 初始化列表支持
//   ✗ limits.hpp           - 数值类型界限（或直接使用 <limits>）
//
// 私有实现子目录（stl/core/__detail/）：
//   ✓ meta.hpp             - 模板元编程核心工具（最底层，无依赖）
//                              - index_sequence, make_index_sequence
//                              - void_t, conjunction, disjunction, negation
//                              - is_detected, detected_t, detected_or
//                              - 类型列表操作（type_list）
//   ✓ concepts_base.hpp    - 基础概念定义（依赖 meta）
//                              - same_as, derived_from, convertible_to
//   ✗ invoke.hpp           - 通用调用封装（依赖 type_traits, utility）
//                              - invoke, invoke_r
//                              - is_invocable, is_invocable_r
//                              - invoke_result, invoke_result_t
//   ✗ compare_impl.hpp     - 三路比较运算符内部实现细节（依赖 type_traits）
//
// 依赖关系链：meta → concepts_base → type_traits → concepts
//                              ↓
//                           utility → pair → tuple
//                              ↓
//                           compare (依赖 concepts)
//
// ============================================================================
// type_traits.hpp 功能范围
// ============================================================================
//
// 本文件应提供标准库 <type_traits> 的功能，包括但不限于：
//
// 1. 类型分类特征（Type Classification Traits）
//    - is_void, is_null_pointer, is_integral, is_floating_point
//    - is_array, is_enum, is_union, is_class, is_function
//    - is_pointer, is_lvalue_reference, is_rvalue_reference
//    - is_member_object_pointer, is_member_function_pointer
//
// 2. 类型属性查询（Type Property Queries）
//    - is_const, is_volatile, is_trivial, is_trivially_copyable
//    - is_standard_layout, is_pod, is_literal_type
//    - is_empty, is_polymorphic, is_abstract, is_final
//    - is_aggregate, is_signed, is_unsigned
//    - is_bounded_array, is_unbounded_array
//
// 3. 类型关系特征（Type Relationship Traits）
//    - is_same, is_base_of, is_convertible, is_nothrow_convertible
//    - is_invocable, is_invocable_r (C++17，依赖 __detail/invoke.hpp)
//    - is_assignable, is_trivially_assignable, is_nothrow_assignable
//
// 4. 类型变换（Type Transformations）
//    - remove_const, remove_volatile, remove_cv
//    - add_const, add_volatile, add_cv
//    - remove_reference, add_lvalue_reference, add_rvalue_reference
//    - remove_pointer, add_pointer
//    - make_signed, make_unsigned
//    - remove_extent, remove_all_extents
//    - remove_cvref (C++20)
//    - type_identity, type_identity_t (C++20)
//
// 5. 常量条件（Constant Condition Traits）
//    - conditional, conditional_t
//    - enable_if, enable_if_t
//
// 6. 逻辑运算符特征（Logical Operator Traits）
//    - conjunction, disjunction, negation (C++17)
//    - conjunction_v, disjunction_v, negation_v (C++17)
//
// 7. 调用特征（Invocation Traits，C++17）
//    - invoke_result, invoke_result_t
//    - is_invocable, is_invocable_r
//    - 注：具体实现应在 __detail/invoke.hpp 中
//
// 8. 其他辅助特征
//    - decay, decay_t
//    - common_type, common_type_t
//    - underlying_type, underlying_type_t
//    - result_of (C++11-14，已弃用，使用 invoke_result)
//
// 9. C++20/23 扩展
//    - is_constant_evaluated (C++20，函数而非类型特征)
//    - is_bounded_array, is_unbounded_array (C++20)
//    - is_scoped_enum (C++23)
//    - is_pointer_interconvertible_base_of (C++20)
//
// ============================================================================
// 模块依赖关系
// ============================================================================
//
// 当前文件依赖：
//   - <mystl/config.hpp>                    - 编译配置和特性检测
//   - <mystl/prelude.hpp>                    - 通用宏和编译器适配
//   - <mystl/core/__detail/concepts_base.hpp> - 基础概念（same_as, derived_from 等）
//   - <mystl/core/__detail/meta.hpp>          - 元编程工具（void_t, index_sequence 等）
//
// 依赖当前文件的模块：
//   - Memory 模块（allocator_traits, pointer_traits 等）
//   - Iterator 模块（iterator_traits）
//   - Container 模块（所有容器都需要类型特征支持）
//   - Algorithm 模块（算法需要类型特征进行优化选择）
//   - Ranges 模块（范围概念和特征）
//   - Utility 模块（variant, optional 等）
//
// ============================================================================
// 实现注意事项
// ============================================================================
//
// 1. 遵循 C++23 标准，充分利用 constexpr if、concepts 等现代特性
// 2. 优先使用标准库 <type_traits>，但提供必要的扩展和优化
// 3. 对于复杂特征（如 is_invocable），委托给 __detail 子目录实现
// 4. 确保 noexcept 正确性，避免不必要的异常传播
// 5. 考虑编译时性能，减少模板实例化深度
// 6. 提供 SFINAE 友好的接口设计
//
// ============================================================================
// 相关标准文档
// ============================================================================
//
// - C++23 Standard: [meta]
// - C++20 Concepts: [concepts]
// - Effective C++: Item 48 (Template metaprogramming)
// - Google C++ Style Guide: Template Metaprogramming
//
// ============================================================================

#ifndef MYSTL_CORE_TYPE_TRAITS_HPP
#define MYSTL_CORE_TYPE_TRAITS_HPP

#include <mystl/config.hpp>
#include <mystl/core/__detail/concepts_base.hpp>
#include <mystl/core/__detail/meta.hpp>

// TODO: 实现类型特征功能
// 1. 基础类型分类特征
// 2. 类型属性查询特征
// 3. 类型关系特征
// 4. 类型变换工具
// 5. 常量条件特征
// 6. 逻辑运算符特征
// 7. C++20/23 扩展特征

#endif // MYSTL_CORE_TYPE_TRAITS_HPP
