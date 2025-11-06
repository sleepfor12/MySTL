#include "tests/framework/mystl_test.hpp"
#include "mystl/memory/pointer_traits.hpp"
#include <type_traits>

// 测试原生指针特化
MYSTL_TEST(pointer_traits_native_pointer, {
  using int_ptr_traits = mystl::pointer_traits<int*>;
  
  // 类型提取
  static_assert(std::is_same_v<int_ptr_traits::pointer, int*>);
  static_assert(std::is_same_v<int_ptr_traits::element_type, int>);
  static_assert(std::is_same_v<int_ptr_traits::difference_type, std::ptrdiff_t>);
  
  // rebind
  using char_ptr = int_ptr_traits::rebind<char>;
  static_assert(std::is_same_v<char_ptr, char*>);
  
  // pointer_to
  int x = 42;
  int* p = int_ptr_traits::pointer_to(x);
  MYSTL_EXPECT_EQ(p, &x);
  MYSTL_EXPECT_EQ(*p, 42);
});

// 测试 const 指针特化
MYSTL_TEST(pointer_traits_const_pointer, {
  using const_int_ptr_traits = mystl::pointer_traits<const int*>;
  
  static_assert(std::is_same_v<const_int_ptr_traits::pointer, const int*>);
  static_assert(std::is_same_v<const_int_ptr_traits::element_type, int>);  // 注意：element_type 不是 const
  static_assert(std::is_same_v<const_int_ptr_traits::difference_type, std::ptrdiff_t>);
  
  // rebind
  using const_char_ptr = const_int_ptr_traits::rebind<char>;
  static_assert(std::is_same_v<const_char_ptr, const char*>);
  
  // pointer_to
  const int x = 42;
  const int* p = const_int_ptr_traits::pointer_to(x);
  MYSTL_EXPECT_EQ(p, &x);
  MYSTL_EXPECT_EQ(*p, 42);
});

// 模拟一个自定义指针类型（用于测试）
struct custom_ptr {
  using element_type = int;
  using difference_type = long;
  
  template <class U>
  using rebind = custom_ptr;  // 简化：实际应该是 rebind 到 U
  
  int* ptr;
  
  static custom_ptr pointer_to(int& r) noexcept {
    custom_ptr p;
    p.ptr = &r;
    return p;
  }
};

// 测试自定义指针类型（模拟分配器返回的指针）
MYSTL_TEST(pointer_traits_custom_pointer, {
  using custom_traits = mystl::pointer_traits<custom_ptr>;
  
  static_assert(std::is_same_v<custom_traits::pointer, custom_ptr>);
  static_assert(std::is_same_v<custom_traits::element_type, int>);
  static_assert(std::is_same_v<custom_traits::difference_type, long>);
  
  // pointer_to
  int x = 42;
  custom_ptr p = custom_traits::pointer_to(x);
  MYSTL_EXPECT_EQ(p.ptr, &x);
  MYSTL_EXPECT_EQ(*p.ptr, 42);
});

// 测试 rebind 功能
MYSTL_TEST(pointer_traits_rebind, {
  // int* -> char*
  using int_ptr = int*;
  using char_ptr = mystl::pointer_traits<int_ptr>::rebind<char>;
  static_assert(std::is_same_v<char_ptr, char*>);
  
  // const int* -> const char*
  using const_int_ptr = const int*;
  using const_char_ptr = mystl::pointer_traits<const_int_ptr>::rebind<char>;
  static_assert(std::is_same_v<const_char_ptr, const char*>);
});

// 测试重载了 operator& 的类型
struct overloaded_address {
  int value;
  overloaded_address* operator&() { return nullptr; }  // 重载 operator&
};

// 测试 pointer_to 与 std::addressof 的一致性
MYSTL_TEST(pointer_traits_pointer_to_addressof, {
  int x = 42;
  int* p1 = mystl::pointer_traits<int*>::pointer_to(x);
  int* p2 = std::addressof(x);
  MYSTL_EXPECT_EQ(p1, p2);
  
  // 测试重载了 operator& 的类型
  overloaded_address obj{42};
  overloaded_address* p3 = mystl::pointer_traits<overloaded_address*>::pointer_to(obj);
  overloaded_address* p4 = std::addressof(obj);
  MYSTL_EXPECT_EQ(p3, p4);  // 应该使用 addressof，而不是 operator&
  MYSTL_EXPECT(p3 != &obj);  // 证明使用了 addressof
});

// 测试 to_address（C++20）
MYSTL_TEST(pointer_traits_to_address, {
  // 原生指针
  int x = 42;
  int* p = &x;
  int* addr = mystl::pointer_traits<int*>::to_address(p);
  MYSTL_EXPECT_EQ(addr, p);
  MYSTL_EXPECT_EQ(addr, &x);
  
  // const 指针
  const int* cp = &x;
  const int* caddr = mystl::pointer_traits<const int*>::to_address(cp);
  MYSTL_EXPECT_EQ(caddr, cp);
  
  // nullptr
  int* null_ptr = nullptr;
  int* null_addr = mystl::pointer_traits<int*>::to_address(null_ptr);
  MYSTL_EXPECT_EQ(null_addr, nullptr);
});

// 测试自定义指针类型的 to_address
struct custom_ptr_with_to_address {
  using element_type = int;
  using difference_type = long;
  
  template <class U>
  using rebind = custom_ptr_with_to_address;
  
  int* ptr;
  
  static custom_ptr_with_to_address pointer_to(int& r) noexcept {
    custom_ptr_with_to_address p;
    p.ptr = &r;
    return p;
  }
  
  // 自定义 to_address
  static int* to_address(const custom_ptr_with_to_address& p) noexcept {
    return p.ptr;
  }
};

MYSTL_TEST(pointer_traits_custom_to_address, {
  int x = 42;
  custom_ptr_with_to_address cp;
  cp.ptr = &x;
  
  // 应该使用自定义的 to_address
  int* addr = mystl::pointer_traits<custom_ptr_with_to_address>::to_address(cp);
  MYSTL_EXPECT_EQ(addr, &x);
  MYSTL_EXPECT_EQ(*addr, 42);
});

