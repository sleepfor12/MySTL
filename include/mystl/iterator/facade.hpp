#ifndef MYSTL_ITERATOR_FACADE_HPP
#define MYSTL_ITERATOR_FACADE_HPP

/**
 * @file iterator/facade.hpp
 * @brief 迭代器外观模式基类 (Iterator Facade - CRTP)
 *
 * 本文件实现 mystl::iterator_facade，使用 CRTP (Curiously Recurring Template Pattern)
 * 来简化自定义迭代器的实现。通过继承 iterator_facade，只需实现少量核心操作，
 * 即可自动获得完整的迭代器接口（operator++, operator--, operator*, operator-> 等）。
 *
 * ## 功能目标 (W2)
 * - 提供类型安全的 CRTP 基类，减少迭代器实现代码量
 * - 自动生成标准迭代器操作符（++、--、*、->、[]、+、-、比较等）
 * - 支持所有迭代器类别：input、forward、bidirectional、random_access
 * - 与 iterator_traits 集成，自动提取类型信息
 *
 * ## 设计模式
 * - CRTP：Derived 类继承 iterator_facade<Derived, ...>
 * - 派生类只需实现核心操作：
 *   - increment() / decrement() / advance(n) / distance_to(other)
 *   - dereference() / equal(other)
 * - 基类自动生成 operator++、operator* 等标准接口
 *
 * ## 使用示例
 * @code
 * class MyIterator : public iterator_facade<MyIterator, int, random_access_iterator_tag> {
 *   // 只需实现核心操作
 *   void increment() { ++ptr_; }
 *   int& dereference() { return *ptr_; }
 *   bool equal(const MyIterator& other) const { return ptr_ == other.ptr_; }
 *   // ... 其他必要操作
 * };
 * @endcode
 *
 * ## 异常安全
 * - 所有生成的操作符遵循派生类的异常保证
 * - 不抛出异常（除非派生类的操作抛出）
 *
 * ## 实现计划
 * 1. 基础 CRTP 模板类定义
 * 2. 为不同迭代器类别特化操作符生成
 * 3. 与 iterator_traits 的集成
 */

namespace mystl {
// template <class Derived, class Value, class Category, class Reference, class Difference>
// class iterator_facade;  // to be defined
}

#endif  // MYSTL_ITERATOR_FACADE_HPP


