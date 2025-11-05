//
// MySTL Core Module - Meta Programming Utilities (Implementation Detail)
// C++23 compatible
//
// ============================================================================
// 模块位置与架构说明
// ============================================================================
//
// 文件路径：stl/core/__detail/meta.hpp
// 所属模块：Core 模块（stl/core/__detail/）
//
// 本文件提供模板元编程的核心工具，是其他模块实现的基础。
// 这些工具通常不直接暴露给用户，而是被公共接口使用。
//
// ============================================================================
// 功能范围
// ============================================================================
//
// 本文件应提供以下元编程工具：
//
// 1. 整数序列（Integer Sequences）
//    - integer_sequence, index_sequence
//    - make_integer_sequence, make_index_sequence
//    - index_sequence_for
//    注：这些也可能在 utility.hpp 中，或两者共享实现
//
// 2. 类型特征工具
//    - void_t - SFINAE 辅助工具
//    - conjunction, disjunction, negation - 逻辑组合
//    - conjunction_v, disjunction_v, negation_v - 便捷别名
//
// 3. 类型检测工具（Detection Idiom）
//    - is_detected, detected_t, detected_or
//    - is_detected_v, detected_or_t
//
// 4. 类型列表（Type Lists，可选）
//    - type_list - 类型列表容器
//    - type_list 相关操作（front, back, at, size 等）
//
// 5. 其他元编程工具
//    - type_identity (C++20，也可能在 type_traits.hpp)
//    - enable_if 的变体
//
// ============================================================================
// 模块依赖关系
// ============================================================================
//
// 当前文件依赖：
//   - <mystl/config.hpp>          - 编译配置和特性检测
//   注：不依赖 type_traits.hpp，避免循环依赖。type_traits 依赖本文件。
//   注：不依赖标准库 <type_traits>，自行实现基础类型特征。
//
// 依赖当前文件的模块：
//   - Core 模块的公共接口（type_traits, utility, tuple 等）
//   - 其他模块的实现细节（__detail 子目录）
//
// ============================================================================
// 实现注意事项
// ============================================================================
//
// 1. 这些是内部实现细节，不应直接暴露给用户
// 2. 注意避免循环依赖（特别是与 type_traits.hpp）
// 3. 不依赖标准库 <type_traits>，自行实现基础类型特征
// 4. 充分利用 C++17/20/23 的现代特性
// 5. 优化编译时性能，减少模板实例化
// 6. 提供清晰的文档注释，便于维护
//
// ============================================================================

#ifndef MYSTL_CORE_DETAIL_META_HPP
#define MYSTL_CORE_DETAIL_META_HPP

#include <mystl/config.hpp>
#include <cstddef>

namespace mystl {
namespace detail {

// ============================================================================
// 基础类型特征（不依赖标准库 type_traits）
// ============================================================================

// integral_constant - 整数常量类型
template<typename T, T v>
struct integral_constant {
    static constexpr T value = v;
    using value_type = T;
    using type = integral_constant;
    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; }
};

// 布尔常量类型
using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

// conditional - 条件类型选择
template<bool B, typename T, typename F>
struct conditional {
    using type = T;
};

template<typename T, typename F>
struct conditional<false, T, F> {
    using type = F;
};

template<bool B, typename T, typename F>
using conditional_t = typename conditional<B, T, F>::type;

// ============================================================================
// 1. 整数序列（Integer Sequences）
// ============================================================================

template<typename T, T... Is>
struct integer_sequence {
    using value_type = T;
    static constexpr size_t size() noexcept { return sizeof...(Is); }
};

template<size_t... Is>
using index_sequence = integer_sequence<size_t, Is...>;

namespace detail_seq {
    // 辅助：生成整数序列的内部实现
    // 使用递归模板展开，但优化编译时性能
    
    // 递归展开：从 N-1 到 0
    template<typename T, T N, T... Is>
    struct make_integer_sequence_impl {
        static_assert(N >= 0, "make_integer_sequence requires non-negative size");
        using type = typename make_integer_sequence_impl<T, N - 1, N - 1, Is...>::type;
    };
    
    // 终止条件：当 N == 0 时，返回已构建的序列
    template<typename T, T... Is>
    struct make_integer_sequence_impl<T, 0, Is...> {
        using type = integer_sequence<T, Is...>;
    };
    
    // 优化：对于小序列使用特化避免深度递归
    template<typename T>
    struct make_integer_sequence_impl<T, 0> {
        using type = integer_sequence<T>;
    };
    
    template<typename T>
    struct make_integer_sequence_impl<T, 1> {
        using type = integer_sequence<T, 0>;
    };
    
    template<typename T>
    struct make_integer_sequence_impl<T, 2> {
        using type = integer_sequence<T, 0, 1>;
    };
    
    template<typename T>
    struct make_integer_sequence_impl<T, 3> {
        using type = integer_sequence<T, 0, 1, 2>;
    };
}

template<typename T, T N>
using make_integer_sequence = typename detail_seq::make_integer_sequence_impl<T, N>::type;

template<size_t N>
using make_index_sequence = make_integer_sequence<size_t, N>;

template<typename... Ts>
using index_sequence_for = make_index_sequence<sizeof...(Ts)>;

// ============================================================================
// 2. 类型特征工具
// ============================================================================

// void_t - SFINAE 辅助工具
template<typename...>
using void_t = void;

// conjunction - 逻辑与组合（短路求值）
template<typename...>
struct conjunction : true_type {};

template<typename B1>
struct conjunction<B1> : B1 {};

template<typename B1, typename B2>
struct conjunction<B1, B2> 
    : conditional_t<static_cast<bool>(B1::value), B2, B1> {};

template<typename B1, typename B2, typename B3, typename... Bn>
struct conjunction<B1, B2, B3, Bn...>
    : conditional_t<static_cast<bool>(B1::value),
                    conjunction<B2, B3, Bn...>,
                    B1> {};

// disjunction - 逻辑或组合（短路求值）
template<typename...>
struct disjunction : false_type {};

template<typename B1>
struct disjunction<B1> : B1 {};

template<typename B1, typename B2>
struct disjunction<B1, B2>
    : conditional_t<static_cast<bool>(B1::value), B1, B2> {};

template<typename B1, typename B2, typename B3, typename... Bn>
struct disjunction<B1, B2, B3, Bn...>
    : conditional_t<static_cast<bool>(B1::value),
                    B1,
                    disjunction<B2, B3, Bn...>> {};

// negation - 逻辑非
template<typename B>
struct negation : integral_constant<bool, !static_cast<bool>(B::value)> {};

// C++17 便捷别名
template<typename... B>
inline constexpr bool conjunction_v = conjunction<B...>::value;

template<typename... B>
inline constexpr bool disjunction_v = disjunction<B...>::value;

template<typename B>
inline constexpr bool negation_v = negation<B>::value;

// ============================================================================
// 3. 类型检测工具（Detection Idiom）
// ============================================================================

// 非类型检测结果
struct nonesuch {
    ~nonesuch() = delete;
    nonesuch(nonesuch const&) = delete;
    void operator=(nonesuch const&) = delete;
};

// 检测表达式是否合法
namespace detail_detected {
    template<typename Default, typename AlwaysVoid, template<typename...> class Op, typename... Args>
    struct detector {
        using value_t = false_type;
        using type = Default;
    };
    
    template<typename Default, template<typename...> class Op, typename... Args>
    struct detector<Default, void_t<Op<Args...>>, Op, Args...> {
        using value_t = true_type;
        using type = Op<Args...>;
    };
}

template<template<typename...> class Op, typename... Args>
using is_detected = typename detail_detected::detector<nonesuch, void, Op, Args...>::value_t;

template<template<typename...> class Op, typename... Args>
using detected_t = typename detail_detected::detector<nonesuch, void, Op, Args...>::type;

template<typename Default, template<typename...> class Op, typename... Args>
using detected_or = detail_detected::detector<Default, void, Op, Args...>;

template<typename Default, template<typename...> class Op, typename... Args>
using detected_or_t = typename detected_or<Default, Op, Args...>::type;

template<template<typename...> class Op, typename... Args>
inline constexpr bool is_detected_v = is_detected<Op, Args...>::value;

// ============================================================================
// 4. 类型列表（Type Lists，可选但有用）
// ============================================================================

// 基础类型列表
template<typename... Ts>
struct type_list {};

// 获取类型列表大小
template<typename List>
struct type_list_size;

template<typename... Ts>
struct type_list_size<type_list<Ts...>> : integral_constant<size_t, sizeof...(Ts)> {};

template<typename List>
inline constexpr size_t type_list_size_v = type_list_size<List>::value;

// 获取第一个类型
template<typename List>
struct type_list_front;

template<typename T, typename... Ts>
struct type_list_front<type_list<T, Ts...>> {
    using type = T;
};

template<typename List>
using type_list_front_t = typename type_list_front<List>::type;

// 获取最后一个类型
template<typename List>
struct type_list_back;

template<typename T>
struct type_list_back<type_list<T>> {
    using type = T;
};

template<typename T, typename... Ts>
struct type_list_back<type_list<T, Ts...>> {
    using type = typename type_list_back<type_list<Ts...>>::type;
};

template<typename List>
using type_list_back_t = typename type_list_back<List>::type;

// 获取第 N 个类型（从 0 开始）
namespace detail_type_list {
    template<size_t N, typename List>
    struct type_list_at_impl;
    
    template<size_t N, typename T, typename... Ts>
    struct type_list_at_impl<N, type_list<T, Ts...>>
        : type_list_at_impl<N - 1, type_list<Ts...>> {};
    
    template<typename T, typename... Ts>
    struct type_list_at_impl<0, type_list<T, Ts...>> {
        using type = T;
    };
}

template<size_t N, typename List>
struct type_list_at : detail_type_list::type_list_at_impl<N, List> {};

template<size_t N, typename List>
using type_list_at_t = typename type_list_at<N, List>::type;

// 从类型列表移除第一个类型
template<typename List>
struct type_list_pop_front;

template<typename T, typename... Ts>
struct type_list_pop_front<type_list<T, Ts...>> {
    using type = type_list<Ts...>;
};

template<typename List>
using type_list_pop_front_t = typename type_list_pop_front<List>::type;

// 连接两个类型列表
template<typename List1, typename List2>
struct type_list_concat;

template<typename... Ts1, typename... Ts2>
struct type_list_concat<type_list<Ts1...>, type_list<Ts2...>> {
    using type = type_list<Ts1..., Ts2...>;
};

template<typename List1, typename List2>
using type_list_concat_t = typename type_list_concat<List1, List2>::type;

// ============================================================================
// 5. 其他元编程工具
// ============================================================================

// type_identity - 类型恒等（C++20）
template<typename T>
struct type_identity {
    using type = T;
};

template<typename T>
using type_identity_t = typename type_identity<T>::type;

// 布尔常量辅助（便捷别名）
template<bool B>
using bool_constant = integral_constant<bool, B>;

} // namespace detail
} // namespace mystl

#endif // MYSTL_CORE_DETAIL_META_HPP
