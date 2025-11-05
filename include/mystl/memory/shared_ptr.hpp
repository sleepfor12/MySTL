#ifndef MYSTL_MEMORY_SHARED_PTR_HPP
#define MYSTL_MEMORY_SHARED_PTR_HPP

/**
 * @file memory/shared_ptr.hpp
 * @brief 共享所有权智能指针 (Shared Ownership Smart Pointer)
 *
 * 本文件实现 mystl::shared_ptr，提供共享所有权的智能指针。
 * shared_ptr 使用引用计数管理资源，当最后一个 shared_ptr 销毁时自动释放资源。
 *
 * ## 功能目标 (W2 - 接口设计，完整实现可延后)
 * - 提供与 std::shared_ptr 兼容的接口和行为
 * - 支持引用计数（reference counting）机制
 * - 支持自定义删除器（deleter）和分配器（allocator）
 * - 支持 weak_ptr 的弱引用计数
 * - 提供 get()、reset()、use_count()、unique() 等核心接口
 * - 支持 operator*、operator->、operator[]
 * - 线程安全的引用计数（C++11 要求）
 *
 * ## 设计要点
 * - 控制块（control block）存储引用计数、弱引用计数、删除器等
 * - 控制块与对象可能分离（make_shared 时共享，直接构造时分离）
 * - 引用计数使用原子操作，保证线程安全
 * - 支持别名构造（aliasing constructor），允许指向成员对象
 *
 * ## 异常安全保证
 * - 构造：如果对象构造失败，不分配控制块（强异常保证）
 * - 复制构造和赋值：可能抛出异常（如果控制块分配失败）
 * - 移动构造和移动赋值：noexcept（不抛出异常）
 * - 析构：不抛出异常（除非删除器抛出，但通常不推荐）
 *
 * ## 实现计划（W2 完成接口设计，完整实现可延后到后续阶段）
 * 1. 控制块结构设计（__details 命名空间）
 * 2. 基础模板类定义
 * 3. 核心接口实现（get、reset、use_count、operator*、operator->）
 * 4. 引用计数管理（原子操作）
 * 5. 自定义删除器支持
 * 6. weak_ptr 集成（弱引用计数）
 * 7. 辅助函数 make_shared()（C++11，可选）
 *
 * ## 注意事项
 * - 循环引用问题：shared_ptr 可能导致循环引用，需要使用 weak_ptr 打破循环
 * - 性能开销：引用计数带来额外开销，不适合性能关键场景
 * - 控制块开销：每个 shared_ptr 对象需要额外的控制块内存
 */

#include <cstddef>
#include <memory>
#include <type_traits>

namespace mystl {

// 前向声明
template <class T> class weak_ptr;

/**
 * @brief 共享所有权智能指针
 * 
 * 根据 cppreference.com/std::shared_ptr
 * 注意：W2 阶段完成接口设计，完整实现可延后
 */
template <class T>
class shared_ptr {
public:
  // 类型定义
  using element_type = T;
  using weak_type = weak_ptr<T>;

  // 构造函数
  constexpr shared_ptr() noexcept;
  template <class Y>
  explicit shared_ptr(Y* p);
  template <class Y, class Deleter>
  shared_ptr(Y* p, Deleter d);
  template <class Y, class Deleter, class Alloc>
  shared_ptr(Y* p, Deleter d, Alloc alloc);
  template <class Deleter>
  shared_ptr(std::nullptr_t p, Deleter d);
  template <class Deleter, class Alloc>
  shared_ptr(std::nullptr_t p, Deleter d, Alloc alloc);
  shared_ptr(const shared_ptr& r) noexcept;
  template <class Y>
  shared_ptr(const shared_ptr<Y>& r) noexcept;
  shared_ptr(shared_ptr&& r) noexcept;
  template <class Y>
  shared_ptr(shared_ptr<Y>&& r) noexcept;
  template <class Y>
  explicit shared_ptr(const weak_ptr<Y>& r);
  
  // TODO: 默认构造：引用计数为 0，指针为 nullptr
  // TODO: 从指针构造：创建控制块，引用计数为 1
  // TODO: 拷贝构造：增加引用计数
  // TODO: 移动构造：转移所有权，r 变为空
  // TODO: 从 weak_ptr 构造：如果对象已销毁则抛出 std::bad_weak_ptr

  // 析构函数
  ~shared_ptr();
  
  // TODO: 减少引用计数，如果为 0 则销毁对象和控制块

  // 赋值运算符
  shared_ptr& operator=(const shared_ptr& r) noexcept;
  template <class Y>
  shared_ptr& operator=(const shared_ptr<Y>& r) noexcept;
  shared_ptr& operator=(shared_ptr&& r) noexcept;
  template <class Y>
  shared_ptr& operator=(shared_ptr<Y>&& r) noexcept;
  
  // TODO: 先释放当前资源，再赋值

  // 修改器
  void reset() noexcept;
  template <class Y>
  void reset(Y* p);
  template <class Y, class Deleter>
  void reset(Y* p, Deleter d);
  template <class Y, class Deleter, class Alloc>
  void reset(Y* p, Deleter d, Alloc alloc);
  
  // TODO: 释放当前资源，设置新的指针和控制块

  void swap(shared_ptr& r) noexcept;
  
  // TODO: 交换指针和控制块

  // 观察器
  T* get() const noexcept;
  T& operator*() const noexcept;
  T* operator->() const noexcept;
  long use_count() const noexcept;
  bool unique() const noexcept;
  explicit operator bool() const noexcept;
  
  // TODO: get() 返回存储的指针
  // TODO: operator* 返回 *get()
  // TODO: operator-> 返回 get()
  // TODO: use_count() 返回引用计数
  // TODO: unique() 返回 use_count() == 1

private:
  T* ptr_;  // 指向对象的指针
  // TODO: 控制块指针（__details 中实现）
};

// 非成员函数

// 比较运算符
template <class T, class U>
bool operator==(const shared_ptr<T>& a, const shared_ptr<U>& b) noexcept;

template <class T, class U>
bool operator!=(const shared_ptr<T>& a, const shared_ptr<U>& b) noexcept;

template <class T, class U>
bool operator<(const shared_ptr<T>& a, const shared_ptr<U>& b) noexcept;

// TODO: 比较基于 a.get() 和 b.get()

// 与 nullptr 比较
template <class T>
bool operator==(const shared_ptr<T>& a, std::nullptr_t) noexcept;

template <class T>
bool operator!=(const shared_ptr<T>& a, std::nullptr_t) noexcept;

// 交换
template <class T>
void swap(shared_ptr<T>& a, shared_ptr<T>& b) noexcept;

// 辅助函数 make_shared (C++11)
template <class T, class... Args>
shared_ptr<T> make_shared(Args&&... args);

template <class T, class Alloc, class... Args>
shared_ptr<T> allocate_shared(const Alloc& alloc, Args&&... args);

// TODO: 分配对象和控制块（共享内存优化）
// TODO: 使用完美转发构造对象

// 类型转换
template <class T, class U>
shared_ptr<T> static_pointer_cast(const shared_ptr<U>& r) noexcept;

template <class T, class U>
shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U>& r) noexcept;

template <class T, class U>
shared_ptr<T> const_pointer_cast(const shared_ptr<U>& r) noexcept;

template <class T, class U>
shared_ptr<T> reinterpret_pointer_cast(const shared_ptr<U>& r) noexcept;

// TODO: 执行类型转换，保持引用计数

}  // namespace mystl

#endif  // MYSTL_MEMORY_SHARED_PTR_HPP


