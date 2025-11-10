#ifndef MYSTL_CONFIG_CONFIG_HPP
#define MYSTL_CONFIG_CONFIG_HPP

// Centralized feature toggles and compiler/workaround switches
// Keep minimal; prefer standard library facilities when available.

#include <version>

#include "compiler.hpp"
#include "platform.hpp"

// C++ standard feature detection
#ifdef __cpp_lib_constexpr_vector
#define MYSTL_HAS_CONSTEXPR_VECTOR 1
#else
#define MYSTL_HAS_CONSTEXPR_VECTOR 0
#endif

#ifdef __cpp_lib_span
#define MYSTL_HAS_STD_SPAN 1
#else
#define MYSTL_HAS_STD_SPAN 0
#endif

#ifdef __cpp_lib_mdspan
#define MYSTL_HAS_STD_MDSPAN 1
#else
#define MYSTL_HAS_STD_MDSPAN 0
#endif

// Inline keyword
#define MYSTL_INLINE inline

// Force inline (use sparingly)
#if MYSTL_COMPILER_MSVC
#define MYSTL_FORCE_INLINE __forceinline
#elif MYSTL_COMPILER_GCC || MYSTL_COMPILER_CLANG
#define MYSTL_FORCE_INLINE inline __attribute__((always_inline))
#else
#define MYSTL_FORCE_INLINE inline
#endif

// Noexcept helper
#define MYSTL_NOEXCEPT noexcept
#define MYSTL_NOEXCEPT_IF(cond) noexcept(cond)

// Constexpr helper
#define MYSTL_CONSTEXPR constexpr
#define MYSTL_CONSTEXPR_IF_CPP20 constexpr

// Likely/unlikely hints (C++20)
#if __cplusplus >= 202002L
#define MYSTL_LIKELY [[likely]]
#define MYSTL_UNLIKELY [[unlikely]]
#else
#define MYSTL_LIKELY
#define MYSTL_UNLIKELY
#endif

// MSVC standards conformance switches
#if MYSTL_COMPILER_MSVC
// Ensure __cplusplus is set correctly with /Zc:__cplusplus (configured via CMake for tests)
#ifndef _SCL_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS 1
#endif
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#endif
#endif

#endif  // MYSTL_CONFIG_CONFIG_HPP
