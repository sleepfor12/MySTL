#ifndef MYSTL_ITERATOR_ADAPTERS_MOVE_ITERATOR_HPP
#define MYSTL_ITERATOR_ADAPTERS_MOVE_ITERATOR_HPP

/**
 * @file iterator/adapters/move_iterator.hpp
 * @brief 移动迭代器适配器 (Move Iterator Adapter)
 *
 * 本文件实现 mystl::move_iterator，用于将普通迭代器转换为移动迭代器。
 * 当解引用移动迭代器时，返回右值引用，使得算法可以移动元素而非复制。
 *
 * ## 功能目标 (W2)
 * - 将输入迭代器适配为移动迭代器
 * - operator* 返回 std::move(*base())，触发移动语义
 * - 支持所有标准迭代器操作（++、--、*、->、[]、+、-、比较等）
 * - 与 std::move_iterator 兼容的接口和行为
 * - 用于优化容器操作（如 vector::insert 的移动版本）
 *
 * ## 设计要点
 * - 内部存储 base() 迭代器，所有操作委托给 base()
 * - operator* 返回 std::move(*base())，将左值转换为右值引用
 * - 支持从 move_iterator 获取原始迭代器（通过 base() 成员函数）
 * - 与 move 算法结合使用，实现高效的移动操作
 *
 * ## 异常安全
 * - 所有操作遵循底层迭代器的异常保证
 * - operator* 的移动操作可能抛出（如果移动构造函数抛出）
 * - 遵循《Effective C++》：异常仅用于不可恢复错误
 *
 * ## 使用示例
 * @code
 * mystl::vector<std::string> src = {"a", "b", "c"};
 * mystl::vector<std::string> dst;
 * mystl::move_iterator<decltype(src)::iterator> mbegin(src.begin());
 * mystl::move_iterator<decltype(src)::iterator> mend(src.end());
 * dst.insert(dst.end(), mbegin, mend);  // 移动而非复制
 * @endcode
 *
 * ## 实现计划
 * 1. 基础模板类定义，存储 base() 迭代器
 * 2. 实现所有迭代器操作符，operator* 返回 std::move(*base())
 * 3. 与 iterator_traits 集成，正确提取值类型和引用类型
 * 4. 辅助函数 make_move_iterator() 简化构造
 */

#include "mystl/iterator/traits.hpp"
#include <iterator>
#include <utility>

namespace mystl {

/**
 * @brief 移动迭代器适配器
 * 
 * 根据 cppreference.com/std::move_iterator
 * 将迭代器转换为移动迭代器，解引用时返回右值引用
 */
template <class Iterator>
class move_iterator {
public:
  // 类型定义
  using iterator_type = Iterator;
  using iterator_category = typename iterator_traits<Iterator>::iterator_category;
  using value_type = typename iterator_traits<Iterator>::value_type;
  using difference_type = typename iterator_traits<Iterator>::difference_type;
  using pointer = Iterator;  // 注意：移动迭代器的 pointer 类型是迭代器本身
  using reference = value_type&&;  // 返回右值引用

  // 构造函数
  move_iterator();
  explicit move_iterator(Iterator i);
  template <class U>
  move_iterator(const move_iterator<U>& other);
  
  // TODO: 默认构造：base() 为默认构造的迭代器
  // TODO: 显式构造：存储 i 作为 base()
  // TODO: 转换构造：允许从兼容的 move_iterator 构造

  // 赋值运算符
  template <class U>
  move_iterator& operator=(const move_iterator<U>& other);
  
  // TODO: 赋值：更新 base() 迭代器

  // 访问底层迭代器
  iterator_type base() const;
  const iterator_type& base() const&;
  
  // TODO: 返回存储的 base() 迭代器

  // 解引用
  reference operator*() const;
  pointer operator->() const;
  
  // TODO: operator* 返回 std::move(*base())
  // TODO: operator-> 返回 base()（如果 base() 是指针类型）

  // 下标访问（仅随机访问迭代器）
  reference operator[](difference_type n) const;
  
  // TODO: 返回 std::move(base()[n])

  // 递增/递减
  move_iterator& operator++();
  move_iterator operator++(int);
  move_iterator& operator--();
  move_iterator operator--(int);
  
  // TODO: operator++ 递增 base()
  // TODO: operator-- 递减 base()
  // TODO: 前置版本返回引用，后置版本返回副本

  // 算术运算（仅随机访问迭代器）
  move_iterator operator+(difference_type n) const;
  move_iterator& operator+=(difference_type n);
  move_iterator operator-(difference_type n) const;
  move_iterator& operator-=(difference_type n);
  
  // TODO: operator+ 返回 base() + n 构造的 move_iterator
  // TODO: operator- 返回 base() - n 构造的 move_iterator
  // TODO: operator+= 修改 base() 为 base() + n
  // TODO: operator-= 修改 base() 为 base() - n

private:
  Iterator current;  // 存储的底层迭代器
};

// 非成员函数

// 算术运算
template <class Iterator>
move_iterator<Iterator> operator+(
    typename move_iterator<Iterator>::difference_type n,
    const move_iterator<Iterator>& it);

// TODO: 返回 it + n

template <class Iterator>
typename move_iterator<Iterator>::difference_type operator-(
    const move_iterator<Iterator>& lhs,
    const move_iterator<Iterator>& rhs);

// TODO: 返回 lhs.base() - rhs.base()

// 比较运算符
template <class Iterator1, class Iterator2>
bool operator==(const move_iterator<Iterator1>& lhs,
                const move_iterator<Iterator2>& rhs);

template <class Iterator1, class Iterator2>
bool operator!=(const move_iterator<Iterator1>& lhs,
                const move_iterator<Iterator2>& rhs);

template <class Iterator1, class Iterator2>
bool operator<(const move_iterator<Iterator1>& lhs,
                const move_iterator<Iterator2>& rhs);

template <class Iterator1, class Iterator2>
bool operator<=(const move_iterator<Iterator1>& lhs,
                 const move_iterator<Iterator2>& rhs);

template <class Iterator1, class Iterator2>
bool operator>(const move_iterator<Iterator1>& lhs,
                const move_iterator<Iterator2>& rhs);

template <class Iterator1, class Iterator2>
bool operator>=(const move_iterator<Iterator1>& lhs,
                 const move_iterator<Iterator2>& rhs);

// TODO: 所有比较运算符都基于 base() 的比较

// 辅助函数
template <class Iterator>
move_iterator<Iterator> make_move_iterator(Iterator it);

// TODO: 返回 move_iterator<Iterator>(it)

}  // namespace mystl

#endif  // MYSTL_ITERATOR_ADAPTERS_MOVE_ITERATOR_HPP


