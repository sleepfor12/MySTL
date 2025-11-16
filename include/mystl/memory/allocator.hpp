#ifndef MYSTL_MEMORY_ALLOCATOR_HPP
#define MYSTL_MEMORY_ALLOCATOR_HPP

#include <cstddef>
#include <limits>
#include <new>
#include <type_traits>

#include "mystl/memory/allocation_result.hpp"

namespace mystl {

template <class T>
struct allocator {
  // 类型定义
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using propagate_on_container_move_assignment = std::true_type;

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

  // 可选的 allocate 重载：带 hint 参数（用于优化，但当前实现忽略 hint）
  // 根据标准，这是可选的，如果分配器不提供此重载，allocator_traits 会回退到单参数版本
  constexpr T* allocate(size_type n, const void* hint) {
    (void)hint;  // hint 在当前实现中未使用，但保留接口以符合标准
    return allocate(n);
  }

  constexpr mystl::allocation_result<T*> allocate_at_least(std::size_t n) {
    static_assert(sizeof(T) > 0, "T must be a complete type");

    if (std::numeric_limits<std::size_t>::max() / sizeof(T) < n) {
      throw std::bad_array_new_length();
    }

    constexpr std::size_t align = alignof(T);

    // allocate_at_least 允许分配至少 n 个元素，可能分配更多
    // 当前实现分配恰好 n 个元素，这是符合标准的（至少分配了 n 个）
    // 未来可以优化为分配更多元素以减少重新分配次数
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
  // - C++20 起：此函数为 constexpr
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
