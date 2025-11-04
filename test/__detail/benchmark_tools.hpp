//
// MySTL Test - Benchmark Tools
// C++23 compatible
//
// 批量性能测试接口：
// - 性能基准测试框架
// - 批量测试执行
// - 性能数据收集
// - 性能报告生成
//

#ifndef MYSTL_TEST_DETAIL_BENCHMARK_TOOLS_HPP
#define MYSTL_TEST_DETAIL_BENCHMARK_TOOLS_HPP

#include <mystl/config.hpp>
#include "__detail/timer.hpp"
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <functional>

namespace mystl::test::__detail {

// 性能统计信息
struct benchmark_stats {
    double min_time = 0.0;
    double max_time = 0.0;
    double mean_time = 0.0;
    double median_time = 0.0;
    double std_dev = 0.0;
    size_t iterations = 0;
};

// 基准测试运行器
class benchmark {
public:
    explicit benchmark(const std::string& name, size_t iterations = 100)
        : name_(name), iterations_(iterations) {}
    
    template<typename Func>
    benchmark_stats run(Func&& func) {
        std::vector<double> times;
        times.reserve(iterations_);
        
        // 预热
        for (size_t i = 0; i < 10; ++i) {
            func();
        }
        
        // 实际测量
        for (size_t i = 0; i < iterations_; ++i) {
            timer t;
            func();
            times.push_back(t.elapsed_ms());
        }
        
        return calculate_stats(times);
    }
    
    template<typename Func>
    benchmark_stats run_with_setup(Func&& func, std::function<void()> setup) {
        std::vector<double> times;
        times.reserve(iterations_);
        
        // 预热
        for (size_t i = 0; i < 10; ++i) {
            setup();
            func();
        }
        
        // 实际测量
        for (size_t i = 0; i < iterations_; ++i) {
            setup();
            timer t;
            func();
            times.push_back(t.elapsed_ms());
        }
        
        return calculate_stats(times);
    }
    
    void print_stats(const benchmark_stats& stats) const {
        std::cout << "\nBenchmark: " << name_ << std::endl;
        std::cout << "  Iterations: " << stats.iterations << std::endl;
        std::cout << std::fixed << std::setprecision(3);
        std::cout << "  Min:    " << stats.min_time << " ms" << std::endl;
        std::cout << "  Max:    " << stats.max_time << " ms" << std::endl;
        std::cout << "  Mean:   " << stats.mean_time << " ms" << std::endl;
        std::cout << "  Median: " << stats.median_time << " ms" << std::endl;
        std::cout << "  StdDev: " << stats.std_dev << " ms" << std::endl;
    }
    
private:
    benchmark_stats calculate_stats(const std::vector<double>& times) const {
        benchmark_stats stats;
        stats.iterations = times.size();
        
        if (times.empty()) {
            return stats;
        }
        
        auto sorted_times = times;
        std::sort(sorted_times.begin(), sorted_times.end());
        
        stats.min_time = sorted_times.front();
        stats.max_time = sorted_times.back();
        stats.mean_time = std::accumulate(times.begin(), times.end(), 0.0) / times.size();
        stats.median_time = sorted_times[times.size() / 2];
        
        // 计算标准差
        double variance = 0.0;
        for (double time : times) {
            double diff = time - stats.mean_time;
            variance += diff * diff;
        }
        stats.std_dev = std::sqrt(variance / times.size());
        
        return stats;
    }
    
    std::string name_;
    size_t iterations_;
};

} // namespace mystl::test::__detail

// 基准测试宏
#define BENCHMARK(name, iterations) \
    mystl::test::__detail::benchmark MYSTL_UNIQUE_NAME(bench)(name, iterations)

#define BENCHMARK_RUN(func) \
    MYSTL_UNIQUE_NAME(bench).run([&]() { func; })

#define BENCHMARK_RUN_WITH_SETUP(func, setup) \
    MYSTL_UNIQUE_NAME(bench).run_with_setup([&]() { func; }, [&]() { setup; })

#endif // MYSTL_TEST_DETAIL_BENCHMARK_TOOLS_HPP

