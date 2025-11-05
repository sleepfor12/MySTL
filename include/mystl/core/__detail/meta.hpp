//
// MySTL Core Module - Meta Programming Utilities (Implementation Detail)
// C++23 compatible
//

#ifndef MYSTL_CORE_DETAIL_META_HPP
#define MYSTL_CORE_DETAIL_META_HPP

#include <mystl/config.hpp>
#include <cstddef>

namespace mystl::__detail {

// integral_constant - 整数常量类型
template <typename T, T v>
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
template <bool B, typename T, typename F>
struct conditional {
  using type = T;
};

template <typename T, typename F>
struct conditional<false, T, F> {
  using type = F;
};

template <bool B, typename T, typename F>
using conditional_t = typename conditional<B, T, F>::type;

// 1. 整数序列（Integer Sequences） - 混合策略优化版
template <typename T, T... Is>
struct integer_sequence {
    using value_type = T;
    static constexpr size_t size() noexcept { return sizeof...(Is); }
};

template <size_t... Is>
using index_sequence = integer_sequence<size_t, Is...>;

namespace detail_seq {

// 合并两个整数序列
template <typename T, typename Seq1, typename Seq2>
struct sequence_merge;

template <typename T, T... Is1, T... Is2>
struct sequence_merge<T, integer_sequence<T, Is1...>, integer_sequence<T, Is2...>> {
    using type = integer_sequence<T, Is1..., (sizeof...(Is1) + Is2)...>;
};

// 线性展开生成器 - 修复版本
template <typename T, T N, T Offset, T... Is>
struct linear_generator {
    using type = typename linear_generator<T, N - 1, Offset, Offset + (N - 1), Is...>::type;
};

// 修复：添加 N=0 的特化
template <typename T, T Offset, T... Is>
struct linear_generator<T, 0, Offset, Is...> {
    using type = integer_sequence<T, Is...>;
};

// 混合策略主模板
template <typename T, T N, T Offset = 0, bool UseBinary = (N > 16)>
struct make_integer_sequence_hybrid;

// 小序列情况：使用线性展开（N <= 16）
template <typename T, T N, T Offset>
struct make_integer_sequence_hybrid<T, N, Offset, false> {
    static_assert(N >= 0, "Sequence size must be non-negative");
    using type = typename linear_generator<T, N, Offset>::type;
};

// 大序列情况：使用二分法（N > 16）
template <typename T, T N, T Offset>
struct make_integer_sequence_hybrid<T, N, Offset, true> {
private:
    static_assert(N > 0, "Sequence size must be positive");
    
    // 将序列分成两半（尽量均衡）
    static constexpr T LeftSize = N / 2;
    static constexpr T RightSize = N - LeftSize;
    
    // 递归生成左右子序列
    using LeftSeq = typename make_integer_sequence_hybrid<T, LeftSize, Offset>::type;
    using RightSeq = typename make_integer_sequence_hybrid<T, RightSize, Offset + LeftSize>::type;
    
public:
    // 合并子序列
    using type = typename sequence_merge<T, LeftSeq, RightSeq>::type;
};

// 基础情况特化 - 优化小序列性能
template <typename T, T Offset>
struct make_integer_sequence_hybrid<T, 0, Offset, false> {
    using type = integer_sequence<T>;
};

template <typename T, T Offset>
struct make_integer_sequence_hybrid<T, 1, Offset, false> {
    using type = integer_sequence<T, Offset>;
};

template <typename T, T Offset>
struct make_integer_sequence_hybrid<T, 2, Offset, false> {
    using type = integer_sequence<T, Offset, Offset + 1>;
};

template <typename T, T Offset>
struct make_integer_sequence_hybrid<T, 3, Offset, false> {
    using type = integer_sequence<T, Offset, Offset + 1, Offset + 2>;
};

template <typename T, T Offset>
struct make_integer_sequence_hybrid<T, 4, Offset, false> {
    using type = integer_sequence<T, Offset, Offset + 1, Offset + 2, Offset + 3>;
};

template <typename T, T Offset>
struct make_integer_sequence_hybrid<T, 5, Offset, false> {
    using type = integer_sequence<T, Offset, Offset + 1, Offset + 2, Offset + 3, Offset + 4>;
};

template <typename T, T Offset>
struct make_integer_sequence_hybrid<T, 6, Offset, false> {
    using type = integer_sequence<T, Offset, Offset + 1, Offset + 2, Offset + 3, Offset + 4, Offset + 5>;
};

template <typename T, T Offset>
struct make_integer_sequence_hybrid<T, 7, Offset, false> {
    using type = integer_sequence<T, Offset, Offset + 1, Offset + 2, Offset + 3, Offset + 4, Offset + 5, Offset + 6>;
};

template <typename T, T Offset>
struct make_integer_sequence_hybrid<T, 8, Offset, false> {
    using type = integer_sequence<T, Offset, Offset + 1, Offset + 2, Offset + 3, Offset + 4, Offset + 5, Offset + 6, Offset + 7>;
};

// 主入口
template <typename T, T N>
using make_integer_sequence_impl = 
    typename make_integer_sequence_hybrid<T, N, 0>::type;

}  // namespace detail_seq

// 公共接口
template <typename T, T N>
using make_integer_sequence = detail_seq::make_integer_sequence_impl<T, N>;

template <size_t N>
using make_index_sequence = make_integer_sequence<size_t, N>;

template <typename... Ts>
using index_sequence_for = make_index_sequence<sizeof...(Ts)>;

// 2. 类型特征工具
// void_t - SFINAE 辅助工具
template <typename...>
using void_t = void;

// conjunction - 逻辑与组合（短路求值）
template <typename...>
struct conjunction : true_type {};

template <typename B1>
struct conjunction<B1> : B1 {};

template <typename B1, typename B2>
struct conjunction<B1, B2>
    : conditional_t<static_cast<bool>(B1::value), B2, B1> {};

template <typename B1, typename B2, typename B3, typename... Bn>
struct conjunction<B1, B2, B3, Bn...>
    : conditional_t<static_cast<bool>(B1::value), conjunction<B2, B3, Bn...>,
                    B1> {};

// disjunction - 逻辑或组合（短路求值）
template <typename...>
struct disjunction : false_type {};

template <typename B1>
struct disjunction<B1> : B1 {};

template <typename B1, typename B2>
struct disjunction<B1, B2>
    : conditional_t<static_cast<bool>(B1::value), B1, B2> {};

template <typename B1, typename B2, typename B3, typename... Bn>
struct disjunction<B1, B2, B3, Bn...>
    : conditional_t<static_cast<bool>(B1::value), B1,
                    disjunction<B2, B3, Bn...>> {};

// negation - 逻辑非
template <typename B>
struct negation : integral_constant<bool, !static_cast<bool>(B::value)> {};

// C++17 便捷别名
template <typename... B>
inline constexpr bool conjunction_v = conjunction<B...>::value;

template <typename... B>
inline constexpr bool disjunction_v = disjunction<B...>::value;

template <typename B>
inline constexpr bool negation_v = negation<B>::value;

// 3. 类型检测工具（Detection Idiom）
// 非类型检测结果
struct nonesuch {
  ~nonesuch() = delete;
  nonesuch(nonesuch const&) = delete;
  void operator=(nonesuch const&) = delete;
};

// 检测表达式是否合法
namespace detail_detected {
template <typename Default, typename AlwaysVoid,
          template <typename...> class Op, typename... Args>
struct detector {
  using value_t = false_type;
  using type = Default;
};

template <typename Default, template <typename...> class Op, typename... Args>
struct detector<Default, void_t<Op<Args...>>, Op, Args...> {
  using value_t = true_type;
  using type = Op<Args...>;
};
}  // namespace detail_detected

template <template <typename...> class Op, typename... Args>
using is_detected =
    typename detail_detected::detector<nonesuch, void, Op, Args...>::value_t;

template <template <typename...> class Op, typename... Args>
using detected_t =
    typename detail_detected::detector<nonesuch, void, Op, Args...>::type;

template <typename Default, template <typename...> class Op, typename... Args>
using detected_or = detail_detected::detector<Default, void, Op, Args...>;

template <typename Default, template <typename...> class Op, typename... Args>
using detected_or_t = typename detected_or<Default, Op, Args...>::type;

template <template <typename...> class Op, typename... Args>
inline constexpr bool is_detected_v = is_detected<Op, Args...>::value;

// 4. 类型列表（Type Lists）
// 基础类型列表
template <typename... Ts>
struct type_list {};

// 获取类型列表大小
template <typename List>
struct type_list_size;

template <typename... Ts>
struct type_list_size<type_list<Ts...>>
    : integral_constant<size_t, sizeof...(Ts)> {};

template <typename List>
inline constexpr size_t type_list_size_v = type_list_size<List>::value;

// 获取第一个类型 - 修复空列表情况
template <typename List>
struct type_list_front;

template <typename T, typename... Ts>
struct type_list_front<type_list<T, Ts...>> {
  using type = T;
};

// 空列表特化 - 新增
template <>
struct type_list_front<type_list<>> {
  // 没有 type，使用时会产生编译错误
};

template <typename List>
using type_list_front_t = typename type_list_front<List>::type;

// 获取最后一个类型 - 修复空列表情况
template <typename List>
struct type_list_back;

template <typename T>
struct type_list_back<type_list<T>> {
  using type = T;
};

template <typename T, typename... Ts>
struct type_list_back<type_list<T, Ts...>> {
  using type = typename type_list_back<type_list<Ts...>>::type;
};

// 空列表特化 - 新增
template <>
struct type_list_back<type_list<>> {
  // 没有 type，使用时会产生编译错误
};

template <typename List>
using type_list_back_t = typename type_list_back<List>::type;

// 获取第 N 个类型（从 0 开始）
namespace detail_type_list {
template <size_t N, typename List>
struct type_list_at_impl;

template <size_t N, typename T, typename... Ts>
struct type_list_at_impl<N, type_list<T, Ts...>>
    : type_list_at_impl<N - 1, type_list<Ts...>> {};

template <typename T, typename... Ts>
struct type_list_at_impl<0, type_list<T, Ts...>> {
  using type = T;
};

// 空列表或索引越界情况 - 新增
template <size_t N>
struct type_list_at_impl<N, type_list<>> {
  // 没有 type，使用时会产生编译错误
};
}  // namespace detail_type_list

template <size_t N, typename List>
struct type_list_at : detail_type_list::type_list_at_impl<N, List> {};

template <size_t N, typename List>
using type_list_at_t = typename type_list_at<N, List>::type;

// 从类型列表移除第一个类型
template <typename List>
struct type_list_pop_front;

template <typename T, typename... Ts>
struct type_list_pop_front<type_list<T, Ts...>> {
  using type = type_list<Ts...>;
};

// 空列表特化 - 新增
template <>
struct type_list_pop_front<type_list<>> {
  using type = type_list<>;
};

template <typename List>
using type_list_pop_front_t = typename type_list_pop_front<List>::type;

// 连接两个类型列表
template <typename List1, typename List2>
struct type_list_concat;

template <typename... Ts1, typename... Ts2>
struct type_list_concat<type_list<Ts1...>, type_list<Ts2...>> {
  using type = type_list<Ts1..., Ts2...>;
};

template <typename List1, typename List2>
using type_list_concat_t = typename type_list_concat<List1, List2>::type;

// 5. 其他元编程工具
// type_identity - 类型恒等
template <typename T>
struct type_identity {
  using type = T;
};

template <typename T>
using type_identity_t = typename type_identity<T>::type;

// 布尔常量辅助（便捷别名）
template <bool B>
using bool_constant = integral_constant<bool, B>;

}  // namespace mystl::__detail

#endif // MYSTL_CORE_DETAIL_META_HPP