#ifndef MYSTL_TEST_FRAMEWORK_PROP_HPP
#define MYSTL_TEST_FRAMEWORK_PROP_HPP

#include <functional>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>

namespace mystl_test {

struct PropConfig {
  int num_cases = 1000;
  unsigned int seed = 0;  // 0 => use random_device
};

inline int run_property(const char* name, const std::function<void(std::mt19937_64&)>& prop,
                        const PropConfig& cfg = {}) {
  unsigned int seed = cfg.seed == 0 ? std::random_device{}() : cfg.seed;
  std::mt19937_64 rng(seed);
  try {
    for (int i = 0; i < cfg.num_cases; ++i) {
      prop(rng);
    }
    std::cout << "[ OK ] PROP " << name << " cases=" << cfg.num_cases << " seed=" << seed << "\n";
    return 0;
  } catch (const std::exception& e) {
    std::cout << "[FAIL] PROP " << name << ": " << e.what() << " (seed=" << seed << ")\n";
    return 1;
  }
}

}  // namespace mystl_test

#define MYSTL_PROP(name, body) \
  static int mystl_prop_fn_##name() { \
    return ::mystl_test::run_property(#name, [](std::mt19937_64& rng) body ); \
  }

#endif  // MYSTL_TEST_FRAMEWORK_PROP_HPP


