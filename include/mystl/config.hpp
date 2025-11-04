//
// MySTL Configuration Header
// C++23 compatible
//
// 库配置和特性检测：
// - C++ 标准版本检测
// - 编译器特性检测
// - constexpr 和 noexcept 控制宏
// - 平台相关配置
//

#ifndef MYSTL_CONFIG_HPP
#define MYSTL_CONFIG_HPP

#if defined(__GNUC__) && !defined(__clang__)
    #define MYSTL_COMPILER_GCC 1
    #define MYSTL_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
    #if MYSTL_GCC_VERSION < 130000
        #error "MySTL requires GCC 13 or later for C++23 support"
    #endif
#else
    #define MYSTL_COMPILER_GCC 0
    #define MYSTL_GCC_VERSION 0
#endif

#if defined(__clang__)
    #define MYSTL_COMPILER_CLANG 1
    #define MYSTL_CLANG_VERSION (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
    #if MYSTL_CLANG_VERSION < 170000
        #error "MySTL requires Clang 17 or later for C++23 support"
    #endif
#else
    #define MYSTL_COMPILER_CLANG 0
    #define MYSTL_CLANG_VERSION 0
#endif

#if defined(_MSC_VER) && !defined(__clang__)
    #define MYSTL_COMPILER_MSVC 1
    #define MYSTL_MSVC_VERSION _MSC_VER
    #if MYSTL_MSVC_VERSION < 1930
        #error "MySTL requires MSVC 19.30 (VS 2022) or later for C++23 support"
    #endif
#else
    #define MYSTL_COMPILER_MSVC 0
    #define MYSTL_MSVC_VERSION 0
#endif

#if !MYSTL_COMPILER_GCC && !MYSTL_COMPILER_CLANG && !MYSTL_COMPILER_MSVC
    #warning "MySTL: Unknown compiler detected. C++23 support may be limited. \
              Please ensure your compiler supports C++23 features."
#endif

#define MYSTL_CPP_STD_98  199711L
#define MYSTL_CPP_STD_03  199711L
#define MYSTL_CPP_STD_11  201103L
#define MYSTL_CPP_STD_14  201402L
#define MYSTL_CPP_STD_17  201703L
#define MYSTL_CPP_STD_20  202002L
#define MYSTL_CPP_STD_23  202302L

#if !defined(__cplusplus)
    #error "MySTL requires a C++ compiler"
#elif __cplusplus < MYSTL_CPP_STD_17
    #error "MySTL requires C++17 or later"
#elif __cplusplus >= MYSTL_CPP_STD_23
    #define MYSTL_CPP_STD MYSTL_CPP_STD_23
    #define MYSTL_CPP_STD_23_ENABLED 1
    #define MYSTL_CPP_STD_20_ENABLED 1
    #define MYSTL_CPP_STD_17_ENABLED 1
#elif __cplusplus >= MYSTL_CPP_STD_20
    #define MYSTL_CPP_STD MYSTL_CPP_STD_20
    #define MYSTL_CPP_STD_23_ENABLED 0
    #define MYSTL_CPP_STD_20_ENABLED 1
    #define MYSTL_CPP_STD_17_ENABLED 1
#else
    #define MYSTL_CPP_STD MYSTL_CPP_STD_17
    #define MYSTL_CPP_STD_23_ENABLED 0
    #define MYSTL_CPP_STD_20_ENABLED 0
    #define MYSTL_CPP_STD_17_ENABLED 1
#endif

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__)
    #define MYSTL_PLATFORM_WINDOWS 1
    #define MYSTL_PLATFORM_POSIX 0
    #ifndef MYSTL_PLATFORM_NAME
        #define MYSTL_PLATFORM_NAME "Windows"
    #endif
#elif defined(__linux__) || defined(__linux)
    #define MYSTL_PLATFORM_WINDOWS 0
    #define MYSTL_PLATFORM_POSIX 1
    #define MYSTL_PLATFORM_LINUX 1
    #ifndef MYSTL_PLATFORM_NAME
        #define MYSTL_PLATFORM_NAME "Linux"
    #endif
#elif defined(__APPLE__) && defined(__MACH__)
    #define MYSTL_PLATFORM_WINDOWS 0
    #define MYSTL_PLATFORM_POSIX 1
    #define MYSTL_PLATFORM_MACOS 1
    #ifndef MYSTL_PLATFORM_NAME
        #define MYSTL_PLATFORM_NAME "macOS"
    #endif
#elif defined(__unix__)
    #define MYSTL_PLATFORM_WINDOWS 0
    #define MYSTL_PLATFORM_POSIX 1
    #ifndef MYSTL_PLATFORM_NAME
        #define MYSTL_PLATFORM_NAME "Unix"
    #endif
#else
    #define MYSTL_PLATFORM_WINDOWS 0
    #define MYSTL_PLATFORM_POSIX 0
    #ifndef MYSTL_PLATFORM_NAME
        #define MYSTL_PLATFORM_NAME "Unknown"
    #endif
#endif

#if MYSTL_CPP_STD_20_ENABLED && defined(__cpp_concepts)
    #define MYSTL_HAS_CONCEPTS 1
    #define MYSTL_CONCEPTS __cpp_concepts
#else
    #define MYSTL_HAS_CONCEPTS 0
    #define MYSTL_CONCEPTS 0
#endif

#if MYSTL_CPP_STD_20_ENABLED && defined(__cpp_ranges)
    #define MYSTL_HAS_RANGES 1
    #define MYSTL_RANGES __cpp_ranges
#else
    #define MYSTL_HAS_RANGES 0
    #define MYSTL_RANGES 0
#endif

#if MYSTL_CPP_STD_20_ENABLED && defined(__cpp_coroutines)
    #define MYSTL_HAS_COROUTINES 1
    #define MYSTL_COROUTINES __cpp_coroutines
#else
    #define MYSTL_HAS_COROUTINES 0
    #define MYSTL_COROUTINES 0
#endif

#if MYSTL_CPP_STD_20_ENABLED && defined(__cpp_modules)
    #define MYSTL_HAS_MODULES 1
    #define MYSTL_MODULES __cpp_modules
#else
    #define MYSTL_HAS_MODULES 0
    #define MYSTL_MODULES 0
#endif

#if MYSTL_CPP_STD_20_ENABLED && defined(__cpp_constexpr_destructor)
    #define MYSTL_HAS_CONSTEXPR_DESTRUCTOR 1
#else
    #define MYSTL_HAS_CONSTEXPR_DESTRUCTOR 0
#endif

#if MYSTL_CPP_STD_20_ENABLED && defined(__cpp_consteval)
    #define MYSTL_HAS_CONSTEVAL 1
#else
    #define MYSTL_HAS_CONSTEVAL 0
#endif

#if MYSTL_CPP_STD_20_ENABLED && defined(__cpp_constinit)
    #define MYSTL_HAS_CONSTINIT 1
#else
    #define MYSTL_HAS_CONSTINIT 0
#endif

#if MYSTL_CPP_STD_20_ENABLED
    #define MYSTL_HAS_FORMAT 1
    #define MYSTL_FORMAT 202110L
#else
    #define MYSTL_HAS_FORMAT 0
    #define MYSTL_FORMAT 0
#endif

#if MYSTL_CPP_STD_23_ENABLED
    #define MYSTL_HAS_EXPECTED 1
    #define MYSTL_EXPECTED 202211L
#else
    #define MYSTL_HAS_EXPECTED 0
    #define MYSTL_EXPECTED 0
#endif

#if MYSTL_CPP_STD_23_ENABLED && MYSTL_HAS_RANGES
    #define MYSTL_HAS_RANGES_TO 1
    #define MYSTL_RANGES_TO 202202L
#else
    #define MYSTL_HAS_RANGES_TO 0
    #define MYSTL_RANGES_TO 0
#endif

#if MYSTL_COMPILER_GCC
    #if __GNUC__ >= 13
        #define MYSTL_HAS_GCC_CONSTEXPR_IMPROVEMENTS 1
    #else
        #define MYSTL_HAS_GCC_CONSTEXPR_IMPROVEMENTS 0
    #endif
    #define MYSTL_HAS_BUILTIN_AVAILABLE 1
#endif

#if MYSTL_COMPILER_CLANG
    #define MYSTL_HAS_BUILTIN_AVAILABLE 1
#endif

#if MYSTL_COMPILER_MSVC
    #if _MSC_VER >= 1930
        #define MYSTL_HAS_MSVC_CONSTEXPR_IMPROVEMENTS 1
    #else
        #define MYSTL_HAS_MSVC_CONSTEXPR_IMPROVEMENTS 0
    #endif
    #define MYSTL_HAS_BUILTIN_AVAILABLE 0
#endif
#ifndef MYSTL_HAS_BUILTIN_AVAILABLE
    #define MYSTL_HAS_BUILTIN_AVAILABLE 0
#endif

#if MYSTL_CPP_STD_11_ENABLED
    #define MYSTL_MODULE_MEMORY 1
    #define MYSTL_HAS_UNIQUE_PTR 1
    #define MYSTL_HAS_SHARED_PTR 1
#else
    #define MYSTL_MODULE_MEMORY 0
    #define MYSTL_HAS_UNIQUE_PTR 0
    #define MYSTL_HAS_SHARED_PTR 0
#endif

#if MYSTL_CPP_STD_17_ENABLED
    #define MYSTL_MODULE_ITERATOR 1
#else
    #define MYSTL_MODULE_ITERATOR 0
#endif

#if MYSTL_MODULE_MEMORY && MYSTL_MODULE_ITERATOR
    #define MYSTL_MODULE_CONTAINER 1
    #define MYSTL_HAS_VECTOR 1
    #define MYSTL_HAS_ARRAY 1
    #define MYSTL_HAS_LIST 1
    #define MYSTL_HAS_DEQUE 1
    #define MYSTL_HAS_MAP 1
    #define MYSTL_HAS_UNORDERED_MAP 1
#else
    #define MYSTL_MODULE_CONTAINER 0
    #define MYSTL_HAS_VECTOR 0
    #define MYSTL_HAS_ARRAY 0
    #define MYSTL_HAS_LIST 0
    #define MYSTL_HAS_DEQUE 0
    #define MYSTL_HAS_MAP 0
    #define MYSTL_HAS_UNORDERED_MAP 0
#endif

#if MYSTL_MODULE_ITERATOR
    #define MYSTL_MODULE_ALGORITHM 1
#else
    #define MYSTL_MODULE_ALGORITHM 0
#endif

#if MYSTL_HAS_RANGES && MYSTL_MODULE_ITERATOR && MYSTL_MODULE_ALGORITHM
    #define MYSTL_MODULE_RANGES 1
    #define MYSTL_HAS_RANGE_VIEWS 1
    #define MYSTL_HAS_RANGE_ADAPTORS 1
#else
    #define MYSTL_MODULE_RANGES 0
    #define MYSTL_HAS_RANGE_VIEWS 0
    #define MYSTL_HAS_RANGE_ADAPTORS 0
#endif
#if MYSTL_COMPILER_GCC || MYSTL_COMPILER_CLANG
    #define MYSTL_HAS_BUILTIN_IS_CONSTANT_EVALUATED 1
    #define MYSTL_HAS_BUILTIN_EXPECT 1
    #define MYSTL_HAS_BUILTIN_PREFETCH 1
#elif MYSTL_COMPILER_MSVC && _MSC_VER >= 1920
    #define MYSTL_HAS_BUILTIN_IS_CONSTANT_EVALUATED 1
    #define MYSTL_HAS_BUILTIN_EXPECT 0
    #define MYSTL_HAS_BUILTIN_PREFETCH 0
#else
    #define MYSTL_HAS_BUILTIN_IS_CONSTANT_EVALUATED 0
    #define MYSTL_HAS_BUILTIN_EXPECT 0
    #define MYSTL_HAS_BUILTIN_PREFETCH 0
#endif

#if MYSTL_CPP_STD_20_ENABLED
    #define MYSTL_HAS_IS_CONSTANT_EVALUATED 1
#else
    #define MYSTL_HAS_IS_CONSTANT_EVALUATED 0
#endif
#define MYSTL_INLINE inline
#if MYSTL_COMPILER_MSVC
    #define MYSTL_FORCE_INLINE __forceinline
#elif MYSTL_COMPILER_GCC || MYSTL_COMPILER_CLANG
    #define MYSTL_FORCE_INLINE __attribute__((always_inline)) inline
#else
    #define MYSTL_FORCE_INLINE inline
#endif

#define MYSTL_NOEXCEPT noexcept
#define MYSTL_NOEXCEPT_IF(cond) noexcept(cond)
#define MYSTL_CONSTEXPR constexpr
#if MYSTL_HAS_CONSTEVAL
    #define MYSTL_CONSTEVAL consteval
#else
    #define MYSTL_CONSTEVAL constexpr
#endif

#if MYSTL_HAS_CONSTINIT
    #define MYSTL_CONSTINIT constinit
#else
    #define MYSTL_CONSTINIT
#endif

#define MYSTL_CHECK_FEATURE(feature) (MYSTL_HAS_##feature)

#ifndef MYSTL_ASSERT
    #ifdef NDEBUG
        #define MYSTL_ASSERT(expr) ((void)0)
    #else
        #include <cassert>
        #define MYSTL_ASSERT(expr) assert(expr)
    #endif
#endif

#define MYSTL_STATIC_ASSERT(expr, msg) static_assert(expr, msg)

#if MYSTL_COMPILER_GCC || MYSTL_COMPILER_CLANG
    #define MYSTL_DEPRECATED(msg) __attribute__((deprecated(msg)))
#elif MYSTL_COMPILER_MSVC
    #define MYSTL_DEPRECATED(msg) __declspec(deprecated(msg))
#else
    #define MYSTL_DEPRECATED(msg)
#endif

#if MYSTL_PLATFORM_WINDOWS
    #ifdef MYSTL_BUILDING_LIBRARY
        #define MYSTL_API __declspec(dllexport)
    #else
        #define MYSTL_API __declspec(dllimport)
    #endif
#else
    #if __GNUC__ >= 4
        #define MYSTL_API __attribute__((visibility("default")))
    #else
        #define MYSTL_API
    #endif
#endif

#endif // MYSTL_CONFIG_HPP

