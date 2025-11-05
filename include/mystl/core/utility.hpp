#ifndef MYSTL_CORE_UTILITY_HPP
#define MYSTL_CORE_UTILITY_HPP

// Utility functions and helpers for MySTL
// Reuse standard library when possible

#include <utility>
#include <type_traits>

namespace mystl {

// Type traits helpers
using std::enable_if_t;
using std::conditional_t;
using std::is_same_v;
using std::is_integral_v;
using std::is_pointer_v;
using std::is_reference_v;
using std::remove_reference_t;
using std::remove_const_t;
using std::remove_cv_t;

// Move and forward
using std::move;
using std::forward;
using std::swap;

// Pair and tuple
using std::pair;
using std::make_pair;
using std::get;

// Helper for SFINAE
template <bool B>
using bool_constant = std::integral_constant<bool, B>;

// Helper for conditional compilation
template <class T>
using decay_t = std::decay_t<T>;

}  // namespace mystl

#endif  // MYSTL_CORE_UTILITY_HPP

