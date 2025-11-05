#ifndef MYSTL_ITERATOR_TRAITS_HPP
#define MYSTL_ITERATOR_TRAITS_HPP

/**
 * @file iterator/traits.hpp
 * @brief 迭代器特性萃取器 (Iterator Traits)
 *
 * 本文件实现 mystl::iterator_traits，用于从迭代器类型中提取标准迭代器属性。
 * 这是 STL 迭代器系统的核心基础设施，所有容器和算法都依赖它来获取迭代器的类型信息。
 *
 * ## 功能目标 (W2)
 * - 提供与 std::iterator_traits 兼容的接口
 * - 支持指针类型（T*, const T*）的特化
 * - 支持自定义迭代器类型的特化
 * - 提取以下类型信息：
 *   - value_type: 迭代器指向的值类型
 *   - difference_type: 迭代器距离类型（通常是 ptrdiff_t）
 *   - pointer: 指针类型
 *   - reference: 引用类型
 *   - iterator_category: 迭代器类别（input/forward/bidirectional/random_access）
 *
 *
 * ## 异常安全
 * 本文件仅定义类型，不涉及运行时操作，无异常安全问题。
 *
 * ## 实现计划
 * 1. 基础模板声明（提取自定义迭代器的类型成员）
 * 2. 指针类型特化（T*, const T*, volatile T* 等）
 * 3. 与 std::iterator_traits 的互操作性（必要时提供转换）
 */

#include <cstddef>
#include <iterator>

namespace mystl {

// 迭代器类别标签
using std::input_iterator_tag;
using std::output_iterator_tag;
using std::forward_iterator_tag;
using std::bidirectional_iterator_tag;
using std::random_access_iterator_tag;
using std::contiguous_iterator_tag;  // C++20

/**
 * @brief 迭代器特性萃取器
 * 
 * 根据 cppreference.com/std::iterator_traits
 * 提取迭代器的标准类型信息
 */
template <class It>
struct iterator_traits {
  using difference_type   = typename It::difference_type;
  using value_type        = typename It::value_type;
  using pointer           = typename It::pointer;
  using reference         = typename It::reference;
  using iterator_category = typename It::iterator_category;
  
  // TODO: 从迭代器类型 It 中提取类型成员
  // 思路：如果 It 是自定义迭代器，直接从其类型成员中提取
  // 如果提取失败，使用 SFINAE 回退到指针特化
};

// 指针类型特化（T*）
template <class T>
struct iterator_traits<T*> {
  using difference_type   = std::ptrdiff_t;
  using value_type        = T;
  using pointer           = T*;
  using reference         = T&;
  using iterator_category = std::contiguous_iterator_tag;
  
  // TODO: 特化指针类型
  // 思路：指针类型直接使用标准类型，category 为 contiguous_iterator_tag (C++20)
};

// const 指针类型特化（const T*）
template <class T>
struct iterator_traits<const T*> {
  using difference_type   = std::ptrdiff_t;
  using value_type        = T;
  using pointer           = const T*;
  using reference         = const T&;
  using iterator_category = std::contiguous_iterator_tag;
  
  // TODO: 特化 const 指针类型
  // 思路：value_type 保持为 T（非 const），但 pointer 和 reference 为 const
};

}  // namespace mystl

#endif  // MYSTL_ITERATOR_TRAITS_HPP


