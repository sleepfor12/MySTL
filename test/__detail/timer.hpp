//
// MySTL Test - Timer Implementation
// C++23 compatible
//
// 高精度计时（chrono 封装）：
// - 计时器实现
// - 性能测量工具
// - 时间统计
// - 基准测试支持
//

#ifndef MYSTL_TEST_DETAIL_TIMER_HPP
#define MYSTL_TEST_DETAIL_TIMER_HPP

#include <mystl/config.hpp>
#include <chrono>
#include <string>
#include <iostream>

namespace mystl::test::__detail {

// 高精度计时器
class timer {
public:
    using clock = std::chrono::high_resolution_clock;
    using time_point = clock::time_point;
    using duration = clock::duration;
    
    timer() : start_(clock::now()) {}
    
    void reset() {
        start_ = clock::now();
    }
    
    duration elapsed() const {
        return clock::now() - start_;
    }
    
    template<typename Rep = double, typename Period = std::milli>
    Rep elapsed_as() const {
        return std::chrono::duration_cast<std::chrono::duration<Rep, Period>>(elapsed()).count();
    }
    
    double elapsed_ms() const {
        return elapsed_as<double, std::milli>();
    }
    
    double elapsed_us() const {
        return elapsed_as<double, std::micro>();
    }
    
    double elapsed_ns() const {
        return elapsed_as<double, std::nano>();
    }
    
private:
    time_point start_;
};

// RAII 计时器（自动输出）
class scoped_timer {
public:
    explicit scoped_timer(const std::string& name) 
        : name_(name), timer_() {}
    
    ~scoped_timer() {
        auto elapsed = timer_.elapsed_ms();
        std::cout << "[" << name_ << "] elapsed: " << elapsed << " ms" << std::endl;
    }
    
private:
    std::string name_;
    timer timer_;
};

} // namespace mystl::test::__detail

// 计时宏
#define TIMER_START(name) \
    mystl::test::__detail::timer MYSTL_UNIQUE_NAME(timer)()

#define TIMER_ELAPSED_MS(name) \
    MYSTL_UNIQUE_NAME(timer).elapsed_ms()

#define TIMER_SCOPE(name) \
    mystl::test::__detail::scoped_timer MYSTL_UNIQUE_NAME(scoped_timer)(name)

#endif // MYSTL_TEST_DETAIL_TIMER_HPP

