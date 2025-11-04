//
// MySTL Test Framework
// C++23 compatible
//
// 轻量级测试框架核心接口：
// - 测试用例注册和运行
// - 断言和验证
// - 测试结果报告
// - 异常处理
//

#ifndef MYSTL_TEST_FRAMEWORK_HPP
#define MYSTL_TEST_FRAMEWORK_HPP

#include <mystl/config.hpp>
#include <mystl/prelude.hpp>
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <sstream>
#include <exception>

namespace mystl::test {

// 测试失败异常
class test_failure : public std::exception {
public:
    explicit test_failure(const std::string& message) : message_(message) {}
    
    const char* what() const noexcept override {
        return message_.c_str();
    }
    
private:
    std::string message_;
};

// 测试结果状态
enum class test_result {
    passed,
    failed,
    skipped
};

// 测试用例信息
struct test_case {
    const char* suite_name;
    const char* test_name;
    std::function<void()> test_func;
    bool enabled = true;
    test_result result = test_result::passed;
    std::string failure_message;
    
    test_case(const char* suite, const char* name, std::function<void()> func)
        : suite_name(suite), test_name(name), test_func(std::move(func)) {}
};

// 测试统计信息
struct test_stats {
    size_t total = 0;
    size_t passed = 0;
    size_t failed = 0;
    size_t skipped = 0;
};

// 测试运行器
class test_runner {
public:
    static test_runner& instance() {
        static test_runner runner;
        return runner;
    }
    
    void register_test(const char* suite_name, const char* test_name, std::function<void()> test_func) {
        tests_.emplace_back(suite_name, test_name, std::move(test_func));
    }
    
    int run_all() {
        test_stats stats;
        stats.total = tests_.size();
        
        std::cout << "Running " << stats.total << " test(s)...\n" << std::endl;
        
        for (auto& test : tests_) {
            if (!test.enabled) {
                test.result = test_result::skipped;
                stats.skipped++;
                continue;
            }
            
            std::cout << "[" << test.suite_name << "::" << test.test_name << "] ";
            
            try {
                current_test_ = &test;
                test.test_func();
                test.result = test_result::passed;
                stats.passed++;
                std::cout << "PASSED" << std::endl;
            } catch (const test_failure& e) {
                test.result = test_result::failed;
                test.failure_message = e.what();
                stats.failed++;
                std::cout << "FAILED: " << e.what() << std::endl;
            } catch (const std::exception& e) {
                test.result = test_result::failed;
                test.failure_message = std::string("Unexpected exception: ") + e.what();
                stats.failed++;
                std::cout << "FAILED: " << test.failure_message << std::endl;
            } catch (...) {
                test.result = test_result::failed;
                test.failure_message = "Unknown exception";
                stats.failed++;
                std::cout << "FAILED: " << test.failure_message << std::endl;
            }
            
            current_test_ = nullptr;
        }
        
        std::cout << "\nTest Results:" << std::endl;
        std::cout << "  Total:  " << stats.total << std::endl;
        std::cout << "  Passed: " << stats.passed << std::endl;
        std::cout << "  Failed: " << stats.failed << std::endl;
        std::cout << "  Skipped: " << stats.skipped << std::endl;
        
        if (stats.failed > 0) {
            std::cout << "\nFailed Tests:" << std::endl;
            for (const auto& test : tests_) {
                if (test.result == test_result::failed) {
                    std::cout << "  [" << test.suite_name << "::" << test.test_name 
                              << "] " << test.failure_message << std::endl;
                }
            }
        }
        
        return (stats.failed == 0) ? 0 : 1;
    }
    
    test_case* get_current_test() {
        return current_test_;
    }
    
private:
    std::vector<test_case> tests_;
    test_case* current_test_ = nullptr;
};

} // namespace mystl::test

#endif // MYSTL_TEST_FRAMEWORK_HPP

