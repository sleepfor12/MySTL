#ifndef MYSTL_ITERATOR_ADAPTERS_REVERSE_ITERATOR_HPP
#define MYSTL_ITERATOR_ADAPTERS_REVERSE_ITERATOR_HPP

/**
 * @file iterator/adapters/reverse_iterator.hpp
 * @brief 反向迭代器适配器 (Reverse Iterator Adapter)
 *
 * 本文件实现 mystl::reverse_iterator，用于将双向或随机访问迭代器转换为反向迭代器。
 * 反向迭代器允许从容器末尾向开头遍历，常用于 rbegin()/rend() 的实现。
 *
 * ## 功能目标 (W2)
 * - 将双向/随机访问迭代器适配为反向迭代器
 * - 支持所有标准迭代器操作（++、--、*、->、[]、+、-、比较等）
 * - 与 std::reverse_iterator 兼容的接口和行为
 * - 正确处理边界情况（rend() 指向 begin()-1 的位置）
 *
 * ## 设计要点
 * - 内部存储 base() 迭代器，反向操作通过调整 base() 实现
 * - operator* 返回 *(base() - 1)，因为反向迭代器的逻辑位置领先物理位置一位
 * - 支持从 reverse_iterator 获取原始迭代器（通过 base() 成员函数）
 *
 * ## 异常安全
 * - 所有操作遵循底层迭代器的异常保证
 * - 不额外抛出异常（除非底层迭代器操作失败）
 *
 * ## 使用示例
 * @code
 * mystl::vector<int> v = {1, 2, 3, 4, 5};
 * mystl::reverse_iterator<decltype(v)::iterator> rbegin(v.end());
 * mystl::reverse_iterator<decltype(v)::iterator> rend(v.begin());
 * for (auto it = rbegin; it != rend; ++it) {
 *   std::cout << *it;  // 输出: 5 4 3 2 1
 * }
 * @endcode
 *
 * ## 实现计划
 * 1. 基础模板类定义，存储 base() 迭代器
 * 2. 实现所有迭代器操作符（++、--、*、->、[]、+、-、比较）
 * 3. 与 iterator_traits 集成，自动提取类型信息
 * 4. 边界情况处理（base() 访问和转换）
 */

#include "mystl/iterator/traits.hpp"
#include <iterator>

namespace mystl {

/**
 * @brief 反向迭代器适配器
 * 
 * 根据 cppreference.com/std::reverse_iterator
 * 将双向或随机访问迭代器转换为反向迭代器
 */
template <class Iterator>
class reverse_iterator {
public:
  // 类型定义
  using iterator_type     = Iterator;
  using iterator_category = typename iterator_traits<Iterator>::iterator_category;
  using value_type        = typename iterator_traits<Iterator>::value_type;
  using difference_type   = typename iterator_traits<Iterator>::difference_type;
  using pointer           = typename iterator_traits<Iterator>::pointer;
  using reference         = typename iterator_traits<Iterator>::reference;

  // 构造函数
  reverse_iterator();
  explicit reverse_iterator(iterator_type x);
  template <class U>
  reverse_iterator(const reverse_iterator<U>& other);
  
  // TODO: 默认构造：base() 为默认构造的迭代器
  // TODO: 显式构造：存储 x 作为 base()
  // TODO: 转换构造：允许从兼容的 reverse_iterator 构造

  // 赋值运算符
  template <class U>
  reverse_iterator& operator=(const reverse_iterator<U>& other);
  
  // TODO: 赋值：更新 base() 迭代器

  // 访问底层迭代器
  iterator_type base() const;
  
  // TODO: 返回存储的 base() 迭代器

  // 解引用
  reference operator*() const;
  pointer operator->() const;
  
  // TODO: operator* 返回 *(base() - 1)
  // TODO: operator-> 返回指向 *(base() - 1) 的指针
  // TODO: 注意边界情况：rend() 指向 begin() - 1

  // 下标访问（仅随机访问迭代器）
  reference operator[](difference_type n) const;
  
  // TODO: 返回 *(base() - n - 1)

  // 递增/递减
  reverse_iterator& operator++();
  reverse_iterator operator++(int);
  reverse_iterator& operator--();
  reverse_iterator operator--(int);
  
  // TODO: operator++ 递减 base()
  // TODO: operator-- 递增 base()
  // TODO: 前置版本返回引用，后置版本返回副本

  // 算术运算（仅随机访问迭代器）
  reverse_iterator operator+(difference_type n) const;
  reverse_iterator& operator+=(difference_type n);
  reverse_iterator operator-(difference_type n) const;
  reverse_iterator& operator-=(difference_type n);
  
  // TODO: operator+ 返回 base() - n 构造的 reverse_iterator
  // TODO: operator- 返回 base() + n 构造的 reverse_iterator
  // TODO: operator+= 修改 base() 为 base() - n
  // TODO: operator-= 修改 base() 为 base() + n

private:
  iterator_type current;  // 存储的底层迭代器
};

// 非成员函数

// 算术运算
template <class Iterator>
reverse_iterator<Iterator> operator+(
    typename reverse_iterator<Iterator>::difference_type n,
    const reverse_iterator<Iterator>& it);

// TODO: 返回 it + n

template <class Iterator>
typename reverse_iterator<Iterator>::difference_type operator-(
    const reverse_iterator<Iterator>& lhs,
    const reverse_iterator<Iterator>& rhs);

// TODO: 返回 rhs.base() - lhs.base()

// 比较运算符
template <class Iterator1, class Iterator2>
bool operator==(const reverse_iterator<Iterator1>& lhs,
                const reverse_iterator<Iterator2>& rhs);

template <class Iterator1, class Iterator2>
bool operator!=(const reverse_iterator<Iterator1>& lhs,
                const reverse_iterator<Iterator2>& rhs);

template <class Iterator1, class Iterator2>
bool operator<(const reverse_iterator<Iterator1>& lhs,
                const reverse_iterator<Iterator2>& rhs);

template <class Iterator1, class Iterator2>
bool operator<=(const reverse_iterator<Iterator1>& lhs,
                 const reverse_iterator<Iterator2>& rhs);

template <class Iterator1, class Iterator2>
bool operator>(const reverse_iterator<Iterator1>& lhs,
                const reverse_iterator<Iterator2>& rhs);

template <class Iterator1, class Iterator2>
bool operator>=(const reverse_iterator<Iterator1>& lhs,
                const reverse_iterator<Iterator2>& rhs);

// TODO: 所有比较运算符都基于 base() 的比较
// TODO: 注意：reverse_iterator 的比较是反向的（因为逻辑位置相反）

// 辅助函数
template <class Iterator>
reverse_iterator<Iterator> make_reverse_iterator(Iterator it);

// TODO: 返回 reverse_iterator<Iterator>(it)

}  // namespace mystl

#endif  // MYSTL_ITERATOR_ADAPTERS_REVERSE_ITERATOR_HPP


