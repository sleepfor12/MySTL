#include "tests/framework/mystl_test.hpp"
#include "mystl/core/utility.hpp"
#include "mystl/core/move_if_noexcept.hpp"
#include <type_traits>

// 测试 utility 中的类型萃取
MYSTL_TEST(core_utility_type_traits, {
  static_assert(mystl::is_same_v<int, int>);
  static_assert(!mystl::is_same_v<int, long>);
  static_assert(mystl::is_integral_v<int>);
  static_assert(!mystl::is_integral_v<float>);
  
  using T1 = mystl::remove_reference_t<int&>;
  static_assert(std::is_same_v<T1, int>);
  
  using T2 = mystl::remove_const_t<const int>;
  static_assert(std::is_same_v<T2, int>);
});

// 测试类型（用于 move_if_noexcept 测试）
struct Movable {
  int value;
  Movable(int v) : value(v) {}
  Movable(Movable&&) noexcept = default;
  Movable(const Movable&) = default;
};

struct NonMovable {
  int value;
  NonMovable(int v) : value(v) {}
  NonMovable(NonMovable&&) = default;  // 可能抛出
  NonMovable(const NonMovable&) = default;
};

// 测试 move_if_noexcept
MYSTL_TEST(core_move_if_noexcept, {
  Movable m1(42);
  auto&& r1 = mystl::move_if_noexcept(m1);
  // Movable 有 noexcept 移动构造，应该返回右值引用
  MYSTL_EXPECT(std::is_rvalue_reference_v<decltype(r1)>);
  
  NonMovable m2(42);
  auto&& r2 = mystl::move_if_noexcept(m2);
  // NonMovable 的移动构造可能抛出，应该返回左值引用（复制）
  MYSTL_EXPECT(std::is_lvalue_reference_v<decltype(r2)>);
});

