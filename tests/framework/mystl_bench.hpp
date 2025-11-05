#ifndef MYSTL_TEST_FRAMEWORK_BENCH_HPP
#define MYSTL_TEST_FRAMEWORK_BENCH_HPP

#include <chrono>
#include <functional>
#include <iostream>
#include <string>

namespace mystl_bench {

struct BenchConfig {
  int warmup_iters = 3;
  int measure_iters = 10;
};

inline void run(const char* name, const std::function<void()>& func, BenchConfig cfg = {}) {
  using clock = std::chrono::steady_clock;
  for (int i = 0; i < cfg.warmup_iters; ++i) func();
  std::chrono::nanoseconds total{0};
  for (int i = 0; i < cfg.measure_iters; ++i) {
    auto t0 = clock::now();
    func();
    auto t1 = clock::now();
    total += std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0);
  }
  auto avg = total.count() / (cfg.measure_iters > 0 ? cfg.measure_iters : 1);
  std::cout << "[BENCH] " << name << " avg(ns): " << avg << "\n";
}

}  // namespace mystl_bench

#define MYSTL_BENCH(name, body) \
  ::mystl_bench::run(#name, [] body )

#endif  // MYSTL_TEST_FRAMEWORK_BENCH_HPP


