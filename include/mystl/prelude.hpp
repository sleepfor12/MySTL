//
// MySTL Prelude Header
// C++23 compatible
//
// 通用宏和编译器适配层：
// - 通用宏定义
// - 编译器特性适配
// - 平台抽象
// - 常用类型别名
//

#ifndef MYSTL_PRELUDE_HPP
#define MYSTL_PRELUDE_HPP

#include <mystl/config.hpp>
#include <mystl/version.hpp>

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <cstring>
#include <climits>
#include <cfloat>

namespace mystl {

using size_t = ::size_t;
using ptrdiff_t = ::ptrdiff_t;
using nullptr_t = ::std::nullptr_t;
using int8_t = ::int8_t;
using int16_t = ::int16_t;
using int32_t = ::int32_t;
using int64_t = ::int64_t;
using uint8_t = ::uint8_t;
using uint16_t = ::uint16_t;
using uint32_t = ::uint32_t;
using uint64_t = ::uint64_t;

using max_align_t = ::max_align_t;

} // namespace mystl

#define MYSTL_STRINGIFY(x) #x
#define MYSTL_STRINGIFY_EXPAND(x) MYSTL_STRINGIFY(x)
#define MYSTL_CAT(a, b) MYSTL_CAT_IMPL(a, b)
#define MYSTL_CAT_IMPL(a, b) a##b
#define MYSTL_UNIQUE_NAME(prefix) MYSTL_CAT(prefix, __LINE__)

#define MYSTL_IS_DEFINED(macro) MYSTL_IS_DEFINED_IMPL(macro)
#define MYSTL_IS_DEFINED_IMPL(macro) MYSTL_IS_DEFINED_##macro
#define MYSTL_IS_DEFINED_1 1
#define MYSTL_IS_DEFINED_0 0
#define MYSTL_IF(cond) MYSTL_CAT(MYSTL_IF_, cond)
#define MYSTL_IF_1(...) __VA_ARGS__
#define MYSTL_IF_0(...)

#if MYSTL_CPP_STD_20_ENABLED
    #if MYSTL_COMPILER_GCC || MYSTL_COMPILER_CLANG
        #define MYSTL_LIKELY(expr) __builtin_expect(!!(expr), 1)
        #define MYSTL_UNLIKELY(expr) __builtin_expect(!!(expr), 0)
    #else
        #define MYSTL_LIKELY(expr) (expr)
        #define MYSTL_UNLIKELY(expr) (expr)
    #endif
#else
    #define MYSTL_LIKELY(expr) (expr)
    #define MYSTL_UNLIKELY(expr) (expr)
#endif

#if MYSTL_CPP_STD_17_ENABLED
    #define MYSTL_NODISCARD [[nodiscard]]
#else
    #if MYSTL_COMPILER_GCC || MYSTL_COMPILER_CLANG
        #define MYSTL_NODISCARD __attribute__((warn_unused_result))
    #elif MYSTL_COMPILER_MSVC
        #define MYSTL_NODISCARD _Check_return_
    #else
        #define MYSTL_NODISCARD
    #endif
#endif

#if MYSTL_CPP_STD_17_ENABLED
    #define MYSTL_MAYBE_UNUSED [[maybe_unused]]
#else
    #if MYSTL_COMPILER_GCC || MYSTL_COMPILER_CLANG
        #define MYSTL_MAYBE_UNUSED __attribute__((unused))
    #else
        #define MYSTL_MAYBE_UNUSED
    #endif
#endif

#if MYSTL_CPP_STD_17_ENABLED
    #define MYSTL_FALLTHROUGH [[fallthrough]]
#else
    #if MYSTL_COMPILER_GCC || MYSTL_COMPILER_CLANG
        #define MYSTL_FALLTHROUGH __attribute__((fallthrough))
    #else
        #define MYSTL_FALLTHROUGH
    #endif
#endif

#if MYSTL_COMPILER_MSVC
    #define MYSTL_ALIGNAS(n) __declspec(align(n))
    #define MYSTL_ALIGNOF(type) __alignof(type)
#else
    #define MYSTL_ALIGNAS(n) __attribute__((aligned(n)))
    #define MYSTL_ALIGNOF(type) __alignof__(type)
#endif

#if MYSTL_COMPILER_GCC || MYSTL_COMPILER_CLANG
    #define MYSTL_MEMORY_BARRIER() __sync_synchronize()
#elif MYSTL_COMPILER_MSVC
    #include <intrin.h>
    #define MYSTL_MEMORY_BARRIER() _MemoryBarrier()
#else
    #define MYSTL_MEMORY_BARRIER() __asm__ __volatile__("" ::: "memory")
#endif

#define MYSTL_IS_SAME(T, U) __is_same(T, U)
#define MYSTL_SIZE_OF(type) sizeof(type)
#define MYSTL_ALIGN_OF(type) MYSTL_ALIGNOF(type)

#if MYSTL_COMPILER_GCC || MYSTL_COMPILER_CLANG
    #define MYSTL_PURE __attribute__((pure))
    #define MYSTL_CONST __attribute__((const))
#else
    #define MYSTL_PURE
    #define MYSTL_CONST
#endif

#define MYSTL_NOEXCEPT_FUNC MYSTL_NOEXCEPT

#if defined(DEBUG) || defined(_DEBUG) || !defined(NDEBUG)
    #define MYSTL_DEBUG 1
#else
    #define MYSTL_DEBUG 0
#endif

#if MYSTL_DEBUG
    #define MYSTL_DEBUG_ASSERT(expr) MYSTL_ASSERT(expr)
#else
    #define MYSTL_DEBUG_ASSERT(expr) ((void)0)
#endif
#if MYSTL_COMPILER_GCC || MYSTL_COMPILER_CLANG
    #define MYSTL_PREFETCH(addr, rw, locality) __builtin_prefetch(addr, rw, locality)
#else
    #define MYSTL_PREFETCH(addr, rw, locality) ((void)0)
#endif

#define MYSTL_EXPECT_TRUE(expr) MYSTL_LIKELY(expr)
#define MYSTL_EXPECT_FALSE(expr) MYSTL_UNLIKELY(expr)

#define MYSTL_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#if MYSTL_COMPILER_MSVC
    #define MYSTL_OFFSETOF(type, member) offsetof(type, member)
#else
    #define MYSTL_OFFSETOF(type, member) __builtin_offsetof(type, member)
#endif

#define MYSTL_MIN(a, b) ((a) < (b) ? (a) : (b))
#define MYSTL_MAX(a, b) ((a) > (b) ? (a) : (b))

#define MYSTL_BEGIN_NAMESPACE namespace mystl {
#define MYSTL_END_NAMESPACE }

#if defined(__cplusplus) && __cplusplus >= 201103L
    #define MYSTL_INLINE_NAMESPACE(v) inline namespace v
#else
    #define MYSTL_INLINE_NAMESPACE(v) namespace v
#endif

#define MYSTL_NAMESPACE mystl
#define MYSTL_NAMESPACE_BEGIN MYSTL_BEGIN_NAMESPACE
#define MYSTL_NAMESPACE_END MYSTL_END_NAMESPACE

#if MYSTL_MODULE_CONTAINER
    #define MYSTL_CONTAINER_STRONG_GUARANTEE 1
    #define MYSTL_CONTAINER_BASIC_GUARANTEE 1
    #define MYSTL_VECTOR_GROWTH_FACTOR 2
    #define MYSTL_DEQUE_BLOCK_SIZE 512
#endif

#if MYSTL_MODULE_ALGORITHM
    #define MYSTL_SORT_INSERTION_THRESHOLD 16
    #define MYSTL_SORT_QUICKSORT_THRESHOLD 64
    #if defined(_OPENMP)
        #define MYSTL_HAS_PARALLEL_ALGORITHMS 1
    #else
        #define MYSTL_HAS_PARALLEL_ALGORITHMS 0
    #endif
#endif

#if MYSTL_MODULE_MEMORY
    #define MYSTL_SHARED_PTR_CONTROL_BLOCK_SIZE 24
    #define MYSTL_SBO_SIZE 16
#endif

#if MYSTL_MODULE_RANGES
    #define MYSTL_RANGE_ADAPTOR_CHAIN_LIMIT 10
    #define MYSTL_VIEW_CACHE_ENABLED 1
#endif

#if MYSTL_HAS_BUILTIN_IS_CONSTANT_EVALUATED
    #define MYSTL_IS_CONSTANT_EVALUATED() __builtin_is_constant_evaluated()
#elif MYSTL_HAS_IS_CONSTANT_EVALUATED
    #include <type_traits>
    #define MYSTL_IS_CONSTANT_EVALUATED() std::is_constant_evaluated()
#else
    #define MYSTL_IS_CONSTANT_EVALUATED() false
#endif

#if MYSTL_HAS_IS_CONSTANT_EVALUATED
    #define MYSTL_IF_CONSTEXPR(cond) if constexpr (cond)
    #define MYSTL_IF_RUNTIME(cond) if (!MYSTL_IS_CONSTANT_EVALUATED() && (cond))
#else
    #define MYSTL_IF_CONSTEXPR(cond) if constexpr (cond)
    #define MYSTL_IF_RUNTIME(cond) if (cond)
#endif

#define MYSTL_DISPATCH(compile_time_expr, runtime_expr) \
    (MYSTL_IS_CONSTANT_EVALUATED() ? (compile_time_expr) : (runtime_expr))

#if MYSTL_COMPILER_GCC || MYSTL_COMPILER_CLANG
    #define MYSTL_IS_TRIVIAL(T) __is_trivial(T)
    #define MYSTL_IS_TRIVIALLY_COPYABLE(T) __is_trivially_copyable(T)
    #define MYSTL_IS_POD(T) __is_pod(T)
    #define MYSTL_IS_EMPTY(T) __is_empty(T)
    #define MYSTL_IS_POLYMORPHIC(T) __is_polymorphic(T)
    #define MYSTL_IS_ABSTRACT(T) __is_abstract(T)
    #define MYSTL_IS_FINAL(T) __is_final(T)
    #define MYSTL_IS_STANDARD_LAYOUT(T) __is_standard_layout(T)
    #define MYSTL_IS_BASE_OF(Base, Derived) __is_base_of(Base, Derived)
#else
    #define MYSTL_IS_TRIVIAL(T) false
    #define MYSTL_IS_TRIVIALLY_COPYABLE(T) false
    #define MYSTL_IS_POD(T) false
    #define MYSTL_IS_EMPTY(T) false
    #define MYSTL_IS_POLYMORPHIC(T) false
    #define MYSTL_IS_ABSTRACT(T) false
    #define MYSTL_IS_FINAL(T) false
    #define MYSTL_IS_STANDARD_LAYOUT(T) false
    #define MYSTL_IS_BASE_OF(Base, Derived) false
#endif

#if MYSTL_DEBUG
    #define MYSTL_DEBUG_CONTAINER 1
    #define MYSTL_DEBUG_ALGORITHM 1
    #define MYSTL_DEBUG_MEMORY 1
#else
    #define MYSTL_DEBUG_CONTAINER 0
    #define MYSTL_DEBUG_ALGORITHM 0
    #define MYSTL_DEBUG_MEMORY 0
#endif

#if MYSTL_DEBUG_CONTAINER
    #define MYSTL_CONTAINER_ASSERT(expr) MYSTL_ASSERT(expr)
#else
    #define MYSTL_CONTAINER_ASSERT(expr) ((void)0)
#endif

#if MYSTL_DEBUG_ALGORITHM
    #define MYSTL_ALGORITHM_ASSERT(expr) MYSTL_ASSERT(expr)
#else
    #define MYSTL_ALGORITHM_ASSERT(expr) ((void)0)
#endif

#if MYSTL_DEBUG_MEMORY
    #define MYSTL_MEMORY_ASSERT(expr) MYSTL_ASSERT(expr)
#else
    #define MYSTL_MEMORY_ASSERT(expr) ((void)0)
#endif

#endif // MYSTL_PRELUDE_HPP

