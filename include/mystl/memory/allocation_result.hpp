#ifndef MYSTL_MEMORY_ALLOCATION_RESULT_HPP
#define MYSTL_MEMORY_ALLOCATION_RESULT_HPP

/**
 * @file memory/allocation_result.hpp
 * @brief 分配结果类型 (Allocation Result)
 *
 * 本文件实现 mystl::allocation_result，用于记录 allocate_at_least 分配的存储地址和实际大小。
 * 这是 C++23 引入的类型，用于支持 allocate_at_least 函数。
 *
 * 根据 cppreference.com/std::allocation_result
 * 当标准库未提供时，提供兼容实现。
 */

#include <cstddef>

namespace mystl {

/**
 * @brief 分配结果类型（C++23）
 *
 * 记录 allocate_at_least 分配的存储地址和实际大小
 */
template <class Pointer>
struct allocation_result {
  Pointer ptr;        // 指向分配的存储的指针
  std::size_t count;  // 实际分配的元素数量（至少为请求的数量）

  // 构造函数
  constexpr allocation_result() noexcept : ptr{}, count(0) {}
  constexpr allocation_result(Pointer p, std::size_t c) noexcept : ptr(p), count(c) {}
};

}  // namespace mystl

#endif  // MYSTL_MEMORY_ALLOCATION_RESULT_HPP
