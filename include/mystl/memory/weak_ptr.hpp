#ifndef MYSTL_MEMORY_WEAK_PTR_HPP
#define MYSTL_MEMORY_WEAK_PTR_HPP

/**
 * @file memory/weak_ptr.hpp
 * @brief 弱引用智能指针 (Weak Reference Smart Pointer)
 *
 * 本文件实现 mystl::weak_ptr，提供对 shared_ptr 管理的对象的弱引用。
 * weak_ptr 不增加引用计数，当对象被销毁时自动失效，用于打破循环引用。
 *
 * ## 功能目标 (W2 - 接口设计，完整实现可延后)
 * - 提供与 std::weak_ptr 兼容的接口和行为
 * - 弱引用计数管理，不阻止对象销毁
 * - 提供 lock() 方法，尝试获取 shared_ptr（如果对象仍存在）
 * - 提供 expired() 方法，检查对象是否已销毁
 * - 支持 use_count() 查询弱引用计数
 * - 与 shared_ptr 的控制块集成
 *
 * ## 设计要点
 * - 弱引用计数存储在 shared_ptr 的控制块中
 * - 当最后一个 shared_ptr 销毁时，对象被销毁，但控制块保留（直到最后一个 weak_ptr 销毁）
 * - lock() 返回 shared_ptr，如果对象已销毁则返回空指针
 * - 不直接访问对象，必须通过 lock() 获取 shared_ptr
 *
 * ## 异常安全保证
 * - 构造和赋值：可能抛出异常（如果控制块操作失败）
 * - lock(): 不抛出异常（返回空指针或有效的 shared_ptr）
 * - expired(): 不抛出异常
 * - 析构：不抛出异常
 *
 * ## 使用场景
 * - 打破循环引用：两个对象相互持有 shared_ptr 时，使用 weak_ptr 打破循环
 * - 缓存系统：缓存对象使用 weak_ptr，允许对象在不再使用时被回收
 * - 观察者模式：观察者使用 weak_ptr 持有被观察对象，避免阻止对象销毁
 *
 * ## 实现计划（W2 完成接口设计，完整实现可延后到后续阶段）
 * 1. 基础模板类定义
 * 2. 核心接口实现（lock、expired、use_count、reset）
 * 3. 与 shared_ptr 控制块的集成
 * 4. 弱引用计数管理
 * 5. 线程安全保证（与 shared_ptr 一致）
 *
 * ## 注意事项
 * - weak_ptr 不能直接解引用，必须通过 lock() 获取 shared_ptr
 * - 使用 weak_ptr 时需要注意 expired() 检查，避免使用已销毁的对象
 * - 弱引用计数也会带来性能开销，但通常可接受
 */

#include <cstddef>
#include <memory>

namespace mystl {

// 前向声明
template <class T> class shared_ptr;

/**
 * @brief 弱引用智能指针
 * 
 * 根据 cppreference.com/std::weak_ptr
 * 注意：W2 阶段完成接口设计，完整实现可延后
 */
template <class T>
class weak_ptr {
public:
  // 类型定义
  using element_type = T;

  // 构造函数
  constexpr weak_ptr() noexcept;
  weak_ptr(const weak_ptr& r) noexcept;
  template <class Y>
  weak_ptr(const weak_ptr<Y>& r) noexcept;
  template <class Y>
  weak_ptr(const shared_ptr<Y>& r) noexcept;
  weak_ptr(weak_ptr&& r) noexcept;
  template <class Y>
  weak_ptr(weak_ptr<Y>&& r) noexcept;
  
  // TODO: 默认构造：弱引用计数为 0
  // TODO: 拷贝构造：增加弱引用计数
  // TODO: 从 shared_ptr 构造：增加弱引用计数
  // TODO: 移动构造：转移所有权

  // 析构函数
  ~weak_ptr();
  
  // TODO: 减少弱引用计数，如果为 0 则销毁控制块

  // 赋值运算符
  weak_ptr& operator=(const weak_ptr& r) noexcept;
  template <class Y>
  weak_ptr& operator=(const weak_ptr<Y>& r) noexcept;
  template <class Y>
  weak_ptr& operator=(const shared_ptr<Y>& r) noexcept;
  weak_ptr& operator=(weak_ptr&& r) noexcept;
  template <class Y>
  weak_ptr& operator=(weak_ptr<Y>&& r) noexcept;
  
  // TODO: 先释放当前资源，再赋值

  // 修改器
  void reset() noexcept;
  void swap(weak_ptr& r) noexcept;
  
  // TODO: reset() 释放当前资源
  // TODO: swap() 交换指针和控制块

  // 观察器
  long use_count() const noexcept;
  bool expired() const noexcept;
  shared_ptr<T> lock() const noexcept;
  
  // TODO: use_count() 返回共享引用计数
  // TODO: expired() 返回 use_count() == 0
  // TODO: lock() 如果对象存在则返回 shared_ptr，否则返回空

private:
  T* ptr_;  // 指向对象的指针（可能已失效）
  // TODO: 控制块指针（__details 中实现）
};

// 非成员函数

// 交换
template <class T>
void swap(weak_ptr<T>& a, weak_ptr<T>& b) noexcept;

// TODO: 调用 a.swap(b)

}  // namespace mystl

#endif  // MYSTL_MEMORY_WEAK_PTR_HPP


