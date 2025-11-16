#ifndef MYSTL_MEMORY_ALLOCATION_RESULT_HPP
#define MYSTL_MEMORY_ALLOCATION_RESULT_HPP

#include <cstddef>

namespace mystl {

template <class Pointer, class SizeType = std::size_t>
struct allocation_result {
  Pointer ptr;     // 指向分配的存储的指针
  SizeType count;  // 实际分配的元素数量（至少为请求的数量）

  // 构造函数
  constexpr allocation_result() noexcept : ptr{}, count(0) {}
  constexpr allocation_result(Pointer p, SizeType c) noexcept : ptr(p), count(c) {}
};

}  // namespace mystl

#endif  // MYSTL_MEMORY_ALLOCATION_RESULT_HPP
