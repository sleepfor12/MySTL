#ifndef MYSTL_TEST_FRAMEWORK_HPP
#define MYSTL_TEST_FRAMEWORK_HPP

// minimal custom testing framework for MySTL
// Usage:
//   MYSTL_TEST("suite.case", []{
//     MYSTL_EXPECT_EQ(1, 1);
//   });

#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace mystl_test {

struct TestCase {
  std::string name;
  std::function<void()> fn;
};

inline std::vector<TestCase>& registry() {
  static std::vector<TestCase> r;
  return r;
}

struct Registrar {
  Registrar(const char* n, std::function<void()> f) { registry().push_back({n, std::move(f)}); }
};

inline int run_all() {
  int failed = 0;
  for (const auto& t : registry()) {
    try {
      t.fn();
      std::cout << "[ OK ] " << t.name << "\n";
    } catch (const std::exception& e) {
      std::cout << "[FAIL] " << t.name << ": " << e.what() << "\n";
      ++failed;
    } catch (...) {
      std::cout << "[FAIL] " << t.name << ": unknown exception\n";
      ++failed;
    }
  }
  std::cout << "\nTotal: " << registry().size() << ", Failed: " << failed << "\n";
  return failed == 0 ? 0 : 1;
}

// assertions
template <class A, class B>
inline void expect_eq(const A& a, const B& b, const char* ea, const char* eb, const char* file, int line) {
  if (!(a == b)) {
    throw std::runtime_error(std::string(file) + ":" + std::to_string(line) +
                             " EXPECT_EQ failed: " + ea + " == " + eb);
  }
}

inline void expect(bool cond, const char* expr, const char* file, int line) {
  if (!cond) {
    throw std::runtime_error(std::string(file) + ":" + std::to_string(line) +
                             " EXPECT failed: " + expr);
  }
}

}  // namespace mystl_test

#define MYSTL_TEST(name, body) \
  static void mystl_test_fn_##name(); \
  static ::mystl_test::Registrar mystl_registrar_##name(#name, &mystl_test_fn_##name); \
  static void mystl_test_fn_##name() body

#define MYSTL_EXPECT(cond) ::mystl_test::expect((cond), #cond, __FILE__, __LINE__)
#define MYSTL_EXPECT_EQ(a, b) ::mystl_test::expect_eq((a), (b), #a, #b, __FILE__, __LINE__)

#endif  // MYSTL_TEST_FRAMEWORK_HPP


