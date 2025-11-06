#ifndef MYSTL_MEMORY_UNIQUE_PTR_HPP
#define MYSTL_MEMORY_UNIQUE_PTR_HPP

/**
 * @file memory/unique_ptr.hpp
 * @brief 唯一所有权智能指针 (Unique Ownership Smart Pointer)
 *
 * 本文件实现 mystl::unique_ptr，提供独占所有权的智能指针。
 * unique_ptr 是零开销的智能指针，移动操作为 noexcept，适合用于资源管理。
 *
 * ## 功能目标 (W2)
 * - 提供与 std::unique_ptr 兼容的接口和行为
 * - 支持单对象版本（unique_ptr<T>）和数组版本（unique_ptr<T[]>）
 * - 支持自定义删除器（deleter），可指定如何释放资源
 * - 移动构造和移动赋值（noexcept），禁止复制
 * - 提供 get()、release()、reset() 等核心接口
 * - 支持 operator*、operator->、operator[]（数组版本）
 *
 * ## 设计原则
 * - 遵循《Effective C++》：使用智能指针管理资源
 * - 零开销抽象：运行时开销与裸指针相同
 * - 移动操作为 noexcept，确保异常安全
 * - 禁止复制，强制唯一所有权语义
 * - 析构时自动释放资源（通过删除器）
 *
 * ## 异常安全保证
 * - 移动构造和移动赋值：noexcept（不抛出异常）
 * - 构造：如果对象构造失败，资源由删除器释放（强异常保证）
 * - reset(): 如果新对象构造失败，旧对象保持不变（强异常保证）
 * - 析构：不抛出异常（除非删除器抛出，但通常不推荐）
 *
 * ## 使用场景
 * - 容器内部资源管理（如 vector 的缓冲区）
 * - 工厂函数返回动态分配的对象
 * - RAII 资源管理（文件、锁等）
 *
 * ## 实现计划
 * 1. 基础模板类定义（单对象版本）
 * 2. 数组版本特化（unique_ptr<T[]>）
 * 3. 核心接口实现（get、release、reset、operator*、operator->）
 * 4. 移动构造和移动赋值（noexcept）
 * 5. 自定义删除器支持（通过模板参数）
 * 6. 辅助函数 make_unique()（C++14，可选）
 * 7. 与 std::unique_ptr 的互操作性（类型转换）
 */

#include <cstddef>
#include <utility>
#include <type_traits>

namespace mystl {

// 默认删除器
template <class T>
struct default_delete {
  constexpr default_delete() noexcept = default;
  template <class U>
  default_delete(const default_delete<U>&) noexcept{}
  
  void operator()(T* ptr) const{
    delete ptr;
  }
};

// 数组特化的默认删除器
template <class T>
struct default_delete<T[]> {
  constexpr default_delete() noexcept = default;
  template <class U>
  default_delete(const default_delete<U[]>&) noexcept;
  
  template <class U>
  void operator()(U* ptr) const {
    delete[] ptr;
  }
};

/**
 * @brief 唯一所有权智能指针（单对象版本）
 * 
 * 根据 cppreference.com/std::unique_ptr
 */
template <class T, class Deleter = default_delete<T>>
class unique_ptr {
public:
  // 类型定义
  using element_type = T;
  using deleter_type = Deleter;
  using pointer      = typename std::pointer_traits<typename Deleter::pointer>::element_type*;

  // 构造函数
  constexpr unique_ptr() noexcept;
  constexpr unique_ptr(std::nullptr_t) noexcept;
  explicit unique_ptr(pointer p) noexcept;
  unique_ptr(pointer p, const Deleter& d) noexcept;
  unique_ptr(pointer p, Deleter&& d) noexcept;
  template <class U, class E>
  unique_ptr(unique_ptr<U, E>&& u) noexcept;
  
  // TODO: 默认构造：p = nullptr
  // TODO: 从指针构造：存储 p 和删除器
  // TODO: 移动构造：转移所有权，u 变为 nullptr

  // 析构函数
  ~unique_ptr();
  
  // TODO: 如果 p 不为 nullptr，调用删除器释放资源

  // 赋值运算符
  unique_ptr& operator=(unique_ptr&& r) noexcept;
  template <class U, class E>
  unique_ptr& operator=(unique_ptr<U, E>&& r) noexcept;
  unique_ptr& operator=(std::nullptr_t) noexcept;
  
  // TODO: 移动赋值：先释放当前资源，再转移 r 的所有权
  // TODO: nullptr 赋值：释放当前资源

  // 删除复制构造和复制赋值
  unique_ptr(const unique_ptr&) = delete;
  unique_ptr& operator=(const unique_ptr&) = delete;

  // 解引用
  typename std::add_lvalue_reference<T>::type operator*() const;
  pointer operator->() const noexcept;
  
  // TODO: operator* 返回 *p
  // TODO: operator-> 返回 p

  // 获取指针
  pointer get() const noexcept;
  
  // TODO: 返回存储的指针 p

  // 获取删除器
  deleter_type& get_deleter() noexcept;
  const deleter_type& get_deleter() const noexcept;
  
  // TODO: 返回删除器引用

  // 检查是否为空
  explicit operator bool() const noexcept;
  
  // TODO: 返回 p != nullptr

  // 释放所有权
  pointer release() noexcept;
  
  // TODO: 返回 p 并设置 p = nullptr（不调用删除器）

  // 重置
  void reset(pointer p = pointer()) noexcept;
  
  // TODO: 释放当前资源（如果有），设置 p = p

  // 交换
  void swap(unique_ptr& other) noexcept;
  
  // TODO: 交换 p 和删除器

private:
  pointer p_;  // 存储的指针
  deleter_type d_;  // 删除器
};

// 数组特化
template <class T, class Deleter>
class unique_ptr<T[], Deleter> {
public:
  // 类型定义
  using element_type = T;
  using deleter_type = Deleter;
  using pointer      = typename std::pointer_traits<typename Deleter::pointer>::element_type*;

  // 构造函数
  constexpr unique_ptr() noexcept;
  explicit unique_ptr(pointer p) noexcept;
  unique_ptr(pointer p, const Deleter& d) noexcept;
  unique_ptr(pointer p, Deleter&& d) noexcept;
  template <class U, class E>
  unique_ptr(unique_ptr<U[], E>&& u) noexcept;
  
  // TODO: 同单对象版本

  // 析构函数
  ~unique_ptr();
  
  // TODO: 调用 delete[] p

  // 赋值运算符
  unique_ptr& operator=(unique_ptr&& r) noexcept;
  template <class U, class E>
  unique_ptr& operator=(unique_ptr<U[], E>&& r) noexcept;
  unique_ptr& operator=(std::nullptr_t) noexcept;
  
  // TODO: 同单对象版本

  // 删除复制构造和复制赋值
  unique_ptr(const unique_ptr&) = delete;
  unique_ptr& operator=(const unique_ptr&) = delete;

  // 下标访问
  T& operator[](std::size_t i) const;
  
  // TODO: 返回 p[i]

  // 其他成员同单对象版本
  pointer get() const noexcept;
  deleter_type& get_deleter() noexcept;
  const deleter_type& get_deleter() const noexcept;
  explicit operator bool() const noexcept;
  pointer release() noexcept;
  void reset(pointer p = pointer()) noexcept;
  void swap(unique_ptr& other) noexcept;

private:
  pointer p_;
  deleter_type d_;
};

// 非成员函数

// 比较运算符
template <class T1, class D1, class T2, class D2>
bool operator==(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);

template <class T1, class D1, class T2, class D2>
bool operator!=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);

template <class T1, class D1, class T2, class D2>
bool operator<(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);

template <class T1, class D1, class T2, class D2>
bool operator<=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);

template <class T1, class D1, class T2, class D2>
bool operator>(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);

template <class T1, class D1, class T2, class D2>
bool operator>=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);

// TODO: 比较基于 x.get() 和 y.get()

// 与 nullptr 比较
template <class T, class D>
bool operator==(const unique_ptr<T, D>& x, std::nullptr_t) noexcept;

template <class T, class D>
bool operator!=(const unique_ptr<T, D>& x, std::nullptr_t) noexcept;

template <class T, class D>
bool operator<(const unique_ptr<T, D>& x, std::nullptr_t);

template <class T, class D>
bool operator<=(const unique_ptr<T, D>& x, std::nullptr_t);

template <class T, class D>
bool operator>(const unique_ptr<T, D>& x, std::nullptr_t);

template <class T, class D>
bool operator>=(const unique_ptr<T, D>& x, std::nullptr_t);

// TODO: 比较 x.get() 和 nullptr

// 交换
template <class T, class D>
void swap(unique_ptr<T, D>& x, unique_ptr<T, D>& y) noexcept;

// TODO: 调用 x.swap(y)

// 辅助函数 make_unique (C++14)
template <class T, class... Args>
std::unique_ptr<T> make_unique(Args&&... args);

template <class T>
std::unique_ptr<T> make_unique(std::size_t size);

// TODO: 分配内存并构造对象，返回 unique_ptr
// TODO: 数组版本分配数组

}  // namespace mystl

#endif  // MYSTL_MEMORY_UNIQUE_PTR_HPP


