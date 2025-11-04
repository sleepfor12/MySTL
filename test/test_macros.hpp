//
// MySTL Test Macros
// C++23 compatible
//
// 测试宏定义：
// - TEST, TEST_F 等测试用例宏
// - ASSERT_*, EXPECT_* 等断言宏
// - 测试辅助宏
//

#ifndef MYSTL_TEST_MACROS_HPP
#define MYSTL_TEST_MACROS_HPP

#include <mystl/config.hpp>
#include <mystl/prelude.hpp>
#include <sstream>
#include <string>
#include <functional>

#include "test_framework.hpp"

// 测试用例注册宏

#define TEST(suite_name, test_name) \
    static void MYSTL_UNIQUE_NAME(test_func)(); \
    namespace { \
        struct MYSTL_UNIQUE_NAME(test_registrar) { \
            MYSTL_UNIQUE_NAME(test_registrar)() { \
                mystl::test::test_runner::instance().register_test( \
                    #suite_name, #test_name, MYSTL_UNIQUE_NAME(test_func)); \
            } \
        } MYSTL_UNIQUE_NAME(test_registrar_instance); \
    } \
    static void MYSTL_UNIQUE_NAME(test_func)()

// 断言宏（失败时抛出异常）

#define ASSERT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            std::ostringstream oss; \
            oss << "Assertion failed: " << #condition << " is false"; \
            throw mystl::test::test_failure(oss.str()); \
        } \
    } while (0)

#define ASSERT_FALSE(condition) \
    do { \
        if (condition) { \
            std::ostringstream oss; \
            oss << "Assertion failed: " << #condition << " is true"; \
            throw mystl::test::test_failure(oss.str()); \
        } \
    } while (0)

#define ASSERT_EQ(expected, actual) \
    do { \
        auto _expected = (expected); \
        auto _actual = (actual); \
        if (_expected != _actual) { \
            std::ostringstream oss; \
            oss << "Assertion failed: " << #expected << " == " << #actual \
                << "\n  Expected: " << _expected << "\n  Actual: " << _actual; \
            throw mystl::test::test_failure(oss.str()); \
        } \
    } while (0)

#define ASSERT_NE(expected, actual) \
    do { \
        auto _expected = (expected); \
        auto _actual = (actual); \
        if (_expected == _actual) { \
            std::ostringstream oss; \
            oss << "Assertion failed: " << #expected << " != " << #actual \
                << "\n  Both values are: " << _expected; \
            throw mystl::test::test_failure(oss.str()); \
        } \
    } while (0)

#define ASSERT_LT(lhs, rhs) \
    do { \
        auto _lhs = (lhs); \
        auto _rhs = (rhs); \
        if (!(_lhs < _rhs)) { \
            std::ostringstream oss; \
            oss << "Assertion failed: " << #lhs << " < " << #rhs \
                << "\n  " << #lhs << " = " << _lhs \
                << "\n  " << #rhs << " = " << _rhs; \
            throw mystl::test::test_failure(oss.str()); \
        } \
    } while (0)

#define ASSERT_LE(lhs, rhs) \
    do { \
        auto _lhs = (lhs); \
        auto _rhs = (rhs); \
        if (!(_lhs <= _rhs)) { \
            std::ostringstream oss; \
            oss << "Assertion failed: " << #lhs << " <= " << #rhs \
                << "\n  " << #lhs << " = " << _lhs \
                << "\n  " << #rhs << " = " << _rhs; \
            throw mystl::test::test_failure(oss.str()); \
        } \
    } while (0)

#define ASSERT_GT(lhs, rhs) \
    do { \
        auto _lhs = (lhs); \
        auto _rhs = (rhs); \
        if (!(_lhs > _rhs)) { \
            std::ostringstream oss; \
            oss << "Assertion failed: " << #lhs << " > " << #rhs \
                << "\n  " << #lhs << " = " << _lhs \
                << "\n  " << #rhs << " = " << _rhs; \
            throw mystl::test::test_failure(oss.str()); \
        } \
    } while (0)

#define ASSERT_GE(lhs, rhs) \
    do { \
        auto _lhs = (lhs); \
        auto _rhs = (rhs); \
        if (!(_lhs >= _rhs)) { \
            std::ostringstream oss; \
            oss << "Assertion failed: " << #lhs << " >= " << #rhs \
                << "\n  " << #lhs << " = " << _lhs \
                << "\n  " << #rhs << " = " << _rhs; \
            throw mystl::test::test_failure(oss.str()); \
        } \
    } while (0)

// EXPECT 宏（失败时记录但继续执行）

#define EXPECT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            std::ostringstream oss; \
            oss << "Expectation failed: " << #condition << " is false"; \
            std::cerr << "  " << oss.str() << std::endl; \
        } \
    } while (0)

#define EXPECT_FALSE(condition) \
    do { \
        if (condition) { \
            std::ostringstream oss; \
            oss << "Expectation failed: " << #condition << " is true"; \
            std::cerr << "  " << oss.str() << std::endl; \
        } \
    } while (0)

#define EXPECT_EQ(expected, actual) \
    do { \
        auto _expected = (expected); \
        auto _actual = (actual); \
        if (_expected != _actual) { \
            std::ostringstream oss; \
            oss << "Expectation failed: " << #expected << " == " << #actual \
                << "\n  Expected: " << _expected << "\n  Actual: " << _actual; \
            std::cerr << "  " << oss.str() << std::endl; \
        } \
    } while (0)

#define EXPECT_NE(expected, actual) \
    do { \
        auto _expected = (expected); \
        auto _actual = (actual); \
        if (_expected == _actual) { \
            std::ostringstream oss; \
            oss << "Expectation failed: " << #expected << " != " << #actual \
                << "\n  Both values are: " << _expected; \
            std::cerr << "  " << oss.str() << std::endl; \
        } \
    } while (0)

#define EXPECT_LT(lhs, rhs) EXPECT_TRUE((lhs) < (rhs))
#define EXPECT_LE(lhs, rhs) EXPECT_TRUE((lhs) <= (rhs))
#define EXPECT_GT(lhs, rhs) EXPECT_TRUE((lhs) > (rhs))
#define EXPECT_GE(lhs, rhs) EXPECT_TRUE((lhs) >= (rhs))

// 字符串比较宏

#define ASSERT_STREQ(expected, actual) \
    do { \
        auto _expected = std::string(expected); \
        auto _actual = std::string(actual); \
        if (_expected != _actual) { \
            std::ostringstream oss; \
            oss << "Assertion failed: " << #expected << " == " << #actual \
                << "\n  Expected: \"" << _expected << "\"" \
                << "\n  Actual: \"" << _actual << "\""; \
            throw mystl::test::test_failure(oss.str()); \
        } \
    } while (0)

#define EXPECT_STREQ(expected, actual) \
    do { \
        auto _expected = std::string(expected); \
        auto _actual = std::string(actual); \
        if (_expected != _actual) { \
            std::ostringstream oss; \
            oss << "Expectation failed: " << #expected << " == " << #actual \
                << "\n  Expected: \"" << _expected << "\"" \
                << "\n  Actual: \"" << _actual << "\""; \
            std::cerr << "  " << oss.str() << std::endl; \
        } \
    } while (0)

// 异常测试宏

#define ASSERT_THROW(statement, exception_type) \
    do { \
        bool _threw = false; \
        try { \
            statement; \
        } catch (const exception_type&) { \
            _threw = true; \
        } catch (...) { \
            std::ostringstream oss; \
            oss << "Assertion failed: " << #statement \
                << " threw different exception type"; \
            throw mystl::test::test_failure(oss.str()); \
        } \
        if (!_threw) { \
            std::ostringstream oss; \
            oss << "Assertion failed: " << #statement \
                << " did not throw " << #exception_type; \
            throw mystl::test::test_failure(oss.str()); \
        } \
    } while (0)

#define ASSERT_NO_THROW(statement) \
    do { \
        try { \
            statement; \
        } catch (const std::exception& e) { \
            std::ostringstream oss; \
            oss << "Assertion failed: " << #statement \
                << " threw exception: " << e.what(); \
            throw mystl::test::test_failure(oss.str()); \
        } catch (...) { \
            std::ostringstream oss; \
            oss << "Assertion failed: " << #statement \
                << " threw unknown exception"; \
            throw mystl::test::test_failure(oss.str()); \
        } \
    } while (0)

// 测试辅助宏

#define SKIP_TEST(reason) \
    do { \
        std::cout << "SKIPPED: " << reason << std::endl; \
        return; \
    } while (0)

#define FAIL_TEST(message) \
    do { \
        std::ostringstream oss; \
        oss << message; \
        throw mystl::test::test_failure(oss.str()); \
    } while (0)

#define SUCCEED_TEST() \
    do { \
        /* 显式标记测试通过，无实际运行时行为 */ \
        /* 测试函数正常返回即视为通过 */ \
    } while (0)

#endif // MYSTL_TEST_MACROS_HPP

