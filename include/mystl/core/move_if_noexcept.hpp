#ifndef MYSTL_CORE_MOVE_IF_NOEXCEPT_HPP
#define MYSTL_CORE_MOVE_IF_NOEXCEPT_HPP

#include <type_traits>
#include <utility>

namespace mystl {

// Move if noexcept, otherwise copy
// Used in container operations to provide strong exception safety
template <class T>
constexpr typename std::conditional<
  std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>,
  std::add_rvalue_reference_t<T>,
  std::add_lvalue_reference_t<T>
>::type move_if_noexcept(T& x) noexcept {
  return std::move(x);
}

}  // namespace mystl

#endif  // MYSTL_CORE_MOVE_IF_NOEXCEPT_HPP

