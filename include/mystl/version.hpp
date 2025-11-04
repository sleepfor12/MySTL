#ifndef MYSTL_VERSION_HPP
#define MYSTL_VERSION_HPP

#include <mystl/config.hpp>

#define MYSTL_VERSION_MAJOR 1
#define MYSTL_VERSION_MINOR 0
#define MYSTL_VERSION_PATCH 0
#define MYSTL_VERSION (MYSTL_VERSION_MAJOR * 10000 + MYSTL_VERSION_MINOR * 100 + MYSTL_VERSION_PATCH)
#define MYSTL_VERSION_STRING "1.0.0"
#define MYSTL_VERSION_NAME "Initial Release"

#define MYSTL_REQUIRED_CPP_STD MYSTL_CPP_STD_17
#define MYSTL_RECOMMENDED_CPP_STD MYSTL_CPP_STD_23
#if MYSTL_CPP_STD < MYSTL_REQUIRED_CPP_STD
    #error "MySTL requires at least C++17. Please use -std=c++17 or higher."
#endif

#if MYSTL_COMPILER_GCC && MYSTL_GCC_VERSION < 130000
    #error "MySTL requires GCC 13.0 or later for full C++23 support"
#endif

#if MYSTL_COMPILER_CLANG && MYSTL_CLANG_VERSION < 170000
    #error "MySTL requires Clang 17.0 or later for full C++23 support"
#endif

#if MYSTL_COMPILER_MSVC && MYSTL_MSVC_VERSION < 1930
    #error "MySTL requires MSVC 19.30 (VS 2022) or later for full C++23 support"
#endif

#if MYSTL_CPP_STD_23_ENABLED && MYSTL_HAS_CONCEPTS && MYSTL_HAS_RANGES
    #define MYSTL_FULL_CXX23_SUPPORT 1
#else
    #define MYSTL_FULL_CXX23_SUPPORT 0
    #if !MYSTL_CPP_STD_23_ENABLED
        #warning "MySTL: C++23 standard is not fully enabled. Some features may be unavailable."
    #endif
    #if !MYSTL_HAS_CONCEPTS
        #warning "MySTL: Concepts are not supported. Some template features may be limited."
    #endif
    #if !MYSTL_HAS_RANGES
        #warning "MySTL: Ranges are not supported. Ranges module will be unavailable."
    #endif
#endif

#if MYSTL_CPP_STD_20_ENABLED
namespace mystl {
    struct version_info {
        static constexpr int major = MYSTL_VERSION_MAJOR;
        static constexpr int minor = MYSTL_VERSION_MINOR;
        static constexpr int patch = MYSTL_VERSION_PATCH;
        static constexpr int version = MYSTL_VERSION;
        static constexpr const char* string = MYSTL_VERSION_STRING;
        static constexpr const char* name = MYSTL_VERSION_NAME;
    };

    MYSTL_CONSTEXPR int version() noexcept {
        return MYSTL_VERSION;
    }

    MYSTL_CONSTEXPR const char* version_string() noexcept {
        return MYSTL_VERSION_STRING;
    }

    MYSTL_CONSTEXPR const char* version_name() noexcept {
        return MYSTL_VERSION_NAME;
    }

    MYSTL_CONSTEXPR bool has_cxx23() noexcept {
        return MYSTL_CPP_STD_23_ENABLED != 0;
    }

    MYSTL_CONSTEXPR bool has_full_cxx23_support() noexcept {
        return MYSTL_FULL_CXX23_SUPPORT != 0;
    }
} // namespace mystl
#endif // MYSTL_CPP_STD_20_ENABLED

#define MYSTL_CHECK_VERSION(major, minor, patch) \
    (MYSTL_VERSION >= ((major) * 10000 + (minor) * 100 + (patch)))

#define MYSTL_CHECK_MAJOR_VERSION(major) \
    (MYSTL_VERSION_MAJOR >= (major))

#define MYSTL_HAS_FEATURE(feature) MYSTL_HAS_##feature
#define MYSTL_HAS_STD(version) (MYSTL_CPP_STD >= MYSTL_CPP_STD_##version)

#define MYSTL_MODULE_AVAILABLE(module) (MYSTL_MODULE_##module)
#define MYSTL_FEATURE_AVAILABLE(feature) (MYSTL_HAS_##feature)

#if MYSTL_CPP_STD_20_ENABLED
namespace mystl {
    MYSTL_CONSTEXPR bool has_core() noexcept {
        return MYSTL_MODULE_CORE != 0;
    }

    MYSTL_CONSTEXPR bool has_memory() noexcept {
        return MYSTL_MODULE_MEMORY != 0;
    }

    MYSTL_CONSTEXPR bool has_iterator() noexcept {
        return MYSTL_MODULE_ITERATOR != 0;
    }

    MYSTL_CONSTEXPR bool has_container() noexcept {
        return MYSTL_MODULE_CONTAINER != 0;
    }

    MYSTL_CONSTEXPR bool has_algorithm() noexcept {
        return MYSTL_MODULE_ALGORITHM != 0;
    }

    MYSTL_CONSTEXPR bool has_ranges() noexcept {
        return MYSTL_MODULE_RANGES != 0;
    }
} // namespace mystl
#endif // MYSTL_CPP_STD_20_ENABLED

#endif // MYSTL_VERSION_HPP

