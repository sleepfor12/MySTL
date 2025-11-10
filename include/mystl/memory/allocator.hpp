#ifndef MYSTL_MEMORY_ALLOCATOR_HPP
#define MYSTL_MEMORY_ALLOCATOR_HPP

/**
 * @file memory/allocator.hpp
 * @brief 内存分配器接口 (Allocator Interface)
 *
 * 分配器是容器的基础，所有需要动态内存的容器都依赖分配器来管理内存。
 *
 * - allocate() 失败时抛出 std::bad_alloc（而非返回 nullptr）
 * - 与 allocator_traits 配合使用，提供统一的分配器接口抽象
 * - 支持无状态分配器（stateless allocator），可复制、可移动
 *
 * ## 接口要求
 * - allocate(n): 分配 n 个对象的连续内存，返回指向首元素的指针（C++20 起为 constexpr）
 * - allocate_at_least(n): 分配至少 n 个对象的未初始化存储（C++23，constexpr）
 * - deallocate(p, n): 释放由 p 指向的 n 个对象的内存（C++20 起为 constexpr）
 *
 * ## 异常安全
 * - allocate() 失败时抛出 std::bad_alloc（强异常保证）
 * - allocate_at_least() 失败时抛出 std::bad_alloc 或 std::bad_array_new_length（强异常保证）
 * - deallocate() 不抛出异常（nothrow 保证）
 *
 */

#include <cstddef>
#include <limits>
#include <new>
#include <type_traits>

#include "mystl/memory/allocation_result.hpp"

namespace mystl {

/**
 * @brief 默认分配器
 *
 * 根据 cppreference.com/std::allocator
 * 提供标准的内存分配接口
 */
template <class T>
struct allocator {
  // 类型定义
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using propagate_on_container_move_assignment = std::true_type;

  // 注意：已通过 LWG issue 3170 弃用（C++23 中仍存在，C++26 计划移除）
  // 保留以保持兼容性，但建议使用 allocator_traits<allocator<T>>::is_always_equal
  using is_always_equal = std::true_type;

  // 构造函数
  constexpr allocator() noexcept = default;
  constexpr allocator(const allocator&) noexcept = default;
  template <class U>
  constexpr allocator(const allocator<U>&) noexcept {}

  // 析构函数
  constexpr ~allocator() = default;

  // - C++20 起：为了在常量表达式中使用此函数，分配的存储必须在同一表达式的求值过程中被解分配
  constexpr T* allocate(size_type n) {
    if (std::numeric_limits<std::size_t>::max() / sizeof(T) < n) {
      throw std::bad_array_new_length();
    }

    // 计算对齐要求
    constexpr std::size_t align = alignof(T);

    // 如果对齐要求超过默认对齐，使用对齐的 operator new（C++17）
    // 必须与 deallocate 使用的对齐方式匹配
#ifdef __STDCPP_DEFAULT_NEW_ALIGNMENT__
    if constexpr (align > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
      // 使用对齐的 operator new（C++17）
      return static_cast<T*>(::operator new(n * sizeof(T), std::align_val_t{align}));
    } else {
      // 使用默认的 operator new
      return static_cast<T*>(::operator new(n * sizeof(T)));
    }
#else
    // 兼容性处理：如果宏未定义，假设默认对齐为 16
    constexpr std::size_t default_align = 16;
    if constexpr (align > default_align) {
      return static_cast<T*>(::operator new(n * sizeof(T), std::align_val_t{align}));
    } else {
      return static_cast<T*>(::operator new(n * sizeof(T)));
    }
#endif
  }

  constexpr mystl::allocation_result<T*> allocate_at_least(std::size_t n) {
    static_assert(sizeof(T) > 0, "T must be a complete type");

    if (std::numeric_limits<std::size_t>::max() / sizeof(T) < n) {
      throw std::bad_array_new_length();
    }

    constexpr std::size_t align = alignof(T);

    std::size_t count = n;

    // 如果对齐要求超过默认对齐，使用对齐的 operator new（C++17）
    // 默认对齐通常是 16 字节（__STDCPP_DEFAULT_NEW_ALIGNMENT__）
    T* ptr;
#ifdef __STDCPP_DEFAULT_NEW_ALIGNMENT__
    if constexpr (align > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
      ptr = static_cast<T*>(::operator new(count * sizeof(T), std::align_val_t{align}));
    } else {
      ptr = static_cast<T*>(::operator new(count * sizeof(T)));
    }
#else
    constexpr std::size_t default_align = 16;
    if constexpr (align > default_align) {
      ptr = static_cast<T*>(::operator new(count * sizeof(T), std::align_val_t{align}));
    } else {
      ptr = static_cast<T*>(::operator new(count * sizeof(T)));
    }
#endif

    return {ptr, count};
  }

  // - p 必须是通过先前对 allocate() 或 allocate_at_least()的调用所获得的指针
  // - n 必须等于最初产生 p 的 allocate() 调用的第一个参数
  //   或者如果 p 是从返回 {p, count} 的 allocate_at_least(m) 调用获得的，则在范围 [m, count] 内
  // - 调用 ::operator delete(void*) 或 ::operator delete(void*, std::align_val_t)
  // - 在常量表达式求值中，此函数必须解分配在同一表达式求值中分配的存储
  constexpr void deallocate(T* p, size_type n) noexcept {
    // 如果 p 是通过 allocate_at_least 分配的，n 应该在 [m, count] 范围内
    (void)n;  // 避免未使用参数警告

    // 计算对齐要求（与 allocate 和 allocate_at_least 保持一致）
    constexpr std::size_t align = alignof(T);

    // 如果对齐要求超过默认对齐，使用对齐的 operator delete
    // 必须与 allocate/allocate_at_least 使用的对齐方式匹配
#ifdef __STDCPP_DEFAULT_NEW_ALIGNMENT__
    if constexpr (align > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
      ::operator delete(p, std::align_val_t{align});
    } else {
      ::operator delete(p);
    }
#else
    constexpr std::size_t default_align = 16;
    if constexpr (align > default_align) {
      ::operator delete(p, std::align_val_t{align});
    } else {
      ::operator delete(p);
    }
#endif
  }

  constexpr allocator select_on_container_copy_construction() const { return *this; }

  template <class U>
  struct rebind {
    using other = allocator<U>;
  };
};
}  // namespace mystl

#endif  // MYSTL_MEMORY_ALLOCATOR_HPP
