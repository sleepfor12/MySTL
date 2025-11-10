#ifndef MYSTL_CONFIG_PLATFORM_HPP
#define MYSTL_CONFIG_PLATFORM_HPP

// Platform detection

#if defined(_WIN32) || defined(_WIN64)
#define MYSTL_PLATFORM_WINDOWS 1
#else
#define MYSTL_PLATFORM_WINDOWS 0
#endif

#if defined(__APPLE__)
#define MYSTL_PLATFORM_APPLE 1
#else
#define MYSTL_PLATFORM_APPLE 0
#endif

#if defined(__linux__)
#define MYSTL_PLATFORM_LINUX 1
#else
#define MYSTL_PLATFORM_LINUX 0
#endif

#endif  // MYSTL_CONFIG_PLATFORM_HPP
