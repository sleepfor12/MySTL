//
// MySTL Test - Assert Implementation
// C++23 compatible
//
// 自定义断言宏与错误报告机制：
// - 断言宏定义
// - 错误消息格式化
// - 测试失败报告
// - 异常捕获和报告
//

#ifndef MYSTL_TEST_DETAIL_ASSERT_IMPL_HPP
#define MYSTL_TEST_DETAIL_ASSERT_IMPL_HPP

#include <mystl/config.hpp>
#include <mystl/prelude.hpp>
#include <string>
#include <sstream>
#include <exception>

namespace mystl::test::__detail {

// 断言失败异常
class assertion_failure : public std::exception {
public:
    explicit assertion_failure(const std::string& message) : message_(message) {}
    
    const char* what() const noexcept override {
        return message_.c_str();
    }
    
private:
    std::string message_;
};

// 断言辅助函数
template<typename T, typename U>
MYSTL_CONSTEXPR bool assert_equal(const T& expected, const U& actual) {
    return expected == actual;
}

template<typename T, typename U>
MYSTL_CONSTEXPR bool assert_not_equal(const T& expected, const U& actual) {
    return expected != actual;
}

template<typename T, typename U>
MYSTL_CONSTEXPR bool assert_less(const T& lhs, const U& rhs) {
    return lhs < rhs;
}

template<typename T, typename U>
MYSTL_CONSTEXPR bool assert_less_equal(const T& lhs, const U& rhs) {
    return lhs <= rhs;
}

template<typename T, typename U>
MYSTL_CONSTEXPR bool assert_greater(const T& lhs, const U& rhs) {
    return lhs > rhs;
}

template<typename T, typename U>
MYSTL_CONSTEXPR bool assert_greater_equal(const T& lhs, const U& rhs) {
    return lhs >= rhs;
}

// 格式化错误消息
template<typename T>
std::string format_value(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

template<>
inline std::string format_value<bool>(const bool& value) {
    return value ? "true" : "false";
}

template<>
inline std::string format_value<const char*>(const char* const& value) {
    return std::string("\"") + value + "\"";
}

} // namespace mystl::test::__detail

#endif // MYSTL_TEST_DETAIL_ASSERT_IMPL_HPP

