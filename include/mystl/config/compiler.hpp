#ifndef MYSTL_CONFIG_COMPILER_HPP
#define MYSTL_CONFIG_COMPILER_HPP

// Compiler detection

#if defined(__clang__)
#define MYSTL_COMPILER_CLANG 1
#else
#define MYSTL_COMPILER_CLANG 0
#endif

#if defined(__GNUC__) && !defined(__clang__)
#define MYSTL_COMPILER_GCC 1
#else
#define MYSTL_COMPILER_GCC 0
#endif

#if defined(_MSC_VER)
#define MYSTL_COMPILER_MSVC 1
#else
#define MYSTL_COMPILER_MSVC 0
#endif

// Attributes and assumptions
#if MYSTL_COMPILER_MSVC
#define MYSTL_ASSUME(cond) __assume(cond)
#define MYSTL_UNREACHABLE() __assume(0)
#define MYSTL_MAYBE_UNUSED [[maybe_unused]]
#elif MYSTL_COMPILER_GCC || MYSTL_COMPILER_CLANG
#define MYSTL_ASSUME(cond) ((cond) ? static_cast<void>(0) : __builtin_unreachable())
#define MYSTL_UNREACHABLE() __builtin_unreachable()
#define MYSTL_MAYBE_UNUSED [[maybe_unused]]
#else
#define MYSTL_ASSUME(cond) ((void)0)
#define MYSTL_UNREACHABLE() ((void)0)
#define MYSTL_MAYBE_UNUSED
#endif

#endif  // MYSTL_CONFIG_COMPILER_HPP
