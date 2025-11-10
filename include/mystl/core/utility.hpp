#ifndef MYSTL_CORE_UTILITY_HPP
#define MYSTL_CORE_UTILITY_HPP

// Utility functions and helpers for MySTL
// Reuse standard library when possible

#include <type_traits>
#include <utility>

namespace mystl {

// Type traits helpers
using std::conditional_t;
using std::enable_if_t;
using std::is_integral_v;
using std::is_pointer_v;
using std::is_reference_v;
using std::is_same_v;
using std::remove_const_t;
using std::remove_cv_t;
using std::remove_reference_t;

// Move and forward
using std::forward;
using std::move;
using std::swap;

// Pair and tuple
using std::get;
using std::make_pair;
using std::pair;

// Helper for SFINAE
template <bool B>
using bool_constant = std::integral_constant<bool, B>;

// Helper for conditional compilation
template <class T>
using decay_t = std::decay_t<T>;

}  // namespace mystl

#endif  // MYSTL_CORE_UTILITY_HPP
