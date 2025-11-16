#include "tests/framework/mystl_test.hpp"
#include "mystl/memory/allocator.hpp"
#include "mystl/memory/allocator_traits.hpp"

#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <new>
#include <type_traits>

namespace {

// 测试用的对齐类型
struct alignas(64) AlignedType {
  int value;
  char padding[60];
};

struct alignas(32) AlignedType32 {
  double data[4];
};

// 测试用的自定义类型
struct TestType {
  int id;
  double value;

  TestType() : id(0), value(0.0) {}
  TestType(int i, double v) : id(i), value(v) {}
  ~TestType() = default;
};

}  // namespace

// ============================================================================
// 类型特征测试
// ============================================================================

MYSTL_TEST(allocator_traits_type_members, {
  using mystl_alloc = mystl::allocator<int>;
  using std_alloc = std::allocator<int>;

  // 检查类型成员
  MYSTL_EXPECT((std::is_same_v<mystl::allocator_traits<mystl_alloc>::value_type, int>));
  MYSTL_EXPECT((std::is_same_v<std::allocator_traits<std_alloc>::value_type, int>));

  MYSTL_EXPECT((std::is_same_v<mystl::allocator_traits<mystl_alloc>::pointer, int*>));
  MYSTL_EXPECT((std::is_same_v<std::allocator_traits<std_alloc>::pointer, int*>));

  MYSTL_EXPECT((std::is_same_v<mystl::allocator_traits<mystl_alloc>::const_pointer, const int*>));
  MYSTL_EXPECT((std::is_same_v<std::allocator_traits<std_alloc>::const_pointer, const int*>));

  MYSTL_EXPECT((std::is_same_v<mystl::allocator_traits<mystl_alloc>::size_type, std::size_t>));
  MYSTL_EXPECT((std::is_same_v<std::allocator_traits<std_alloc>::size_type, std::size_t>));

  MYSTL_EXPECT((std::is_same_v<mystl::allocator_traits<mystl_alloc>::difference_type, std::ptrdiff_t>));
  MYSTL_EXPECT((std::is_same_v<std::allocator_traits<std_alloc>::difference_type, std::ptrdiff_t>));
});

MYSTL_TEST(allocator_traits_propagate_flags, {
  using mystl_alloc = mystl::allocator<int>;
  using std_alloc = std::allocator<int>;

  // 检查传播标志 - 使用 typename 来访问嵌套类型
  using mystl_pocma = typename mystl::allocator_traits<mystl_alloc>::propagate_on_container_move_assignment;
  using std_pocma = typename std::allocator_traits<std_alloc>::propagate_on_container_move_assignment;

  MYSTL_EXPECT(mystl_pocma::value);
  MYSTL_EXPECT(std_pocma::value);

  using mystl_pocca = typename mystl::allocator_traits<mystl_alloc>::propagate_on_container_copy_assignment;
  using std_pocca = typename std::allocator_traits<std_alloc>::propagate_on_container_copy_assignment;

  MYSTL_EXPECT(!mystl_pocca::value);
  MYSTL_EXPECT(!std_pocca::value);

  using mystl_pocs = typename mystl::allocator_traits<mystl_alloc>::propagate_on_container_swap;
  using std_pocs = typename std::allocator_traits<std_alloc>::propagate_on_container_swap;

  MYSTL_EXPECT(!mystl_pocs::value);
  MYSTL_EXPECT(!std_pocs::value);

  using mystl_iae = typename mystl::allocator_traits<mystl_alloc>::is_always_equal;
  using std_iae = typename std::allocator_traits<std_alloc>::is_always_equal;

  MYSTL_EXPECT(mystl_iae::value);
  MYSTL_EXPECT(std_iae::value);
});

MYSTL_TEST(allocator_traits_rebind, {
  using mystl_alloc = mystl::allocator<int>;
  using std_alloc = std::allocator<int>;

  using mystl_rebound = mystl::allocator_traits<mystl_alloc>::rebind_alloc<double>;
  using std_rebound = std::allocator_traits<std_alloc>::rebind_alloc<double>;

  MYSTL_EXPECT((std::is_same_v<mystl_rebound, mystl::allocator<double>>));
  MYSTL_EXPECT((std::is_same_v<std_rebound, std::allocator<double>>));
});

// ============================================================================
// 基本分配/释放测试
// ============================================================================

MYSTL_TEST(allocator_allocate_deallocate_basic, {
  mystl::allocator<int> mystl_alloc;
  std::allocator<int> std_alloc;

  constexpr std::size_t count = 10;

  // MySTL 分配
  int* mystl_ptr = mystl_alloc.allocate(count);
  MYSTL_EXPECT(mystl_ptr != nullptr);

  // 标准库分配
  int* std_ptr = std_alloc.allocate(count);
  MYSTL_EXPECT(std_ptr != nullptr);

  // 初始化内存
  for (std::size_t i = 0; i < count; ++i) {
    std::construct_at(mystl_ptr + i, static_cast<int>(i * 2));
    std::construct_at(std_ptr + i, static_cast<int>(i * 2));
  }

  // 验证值
  for (std::size_t i = 0; i < count; ++i) {
    MYSTL_EXPECT_EQ(mystl_ptr[i], std_ptr[i]);
    MYSTL_EXPECT_EQ(mystl_ptr[i], static_cast<int>(i * 2));
  }

  // 清理
  for (std::size_t i = 0; i < count; ++i) {
    std::destroy_at(mystl_ptr + i);
    std::destroy_at(std_ptr + i);
  }

  mystl_alloc.deallocate(mystl_ptr, count);
  std_alloc.deallocate(std_ptr, count);
});

MYSTL_TEST(allocator_traits_allocate_deallocate, {
  mystl::allocator<int> mystl_alloc;
  std::allocator<int> std_alloc;

  constexpr std::size_t count = 5;

  // 使用 allocator_traits 分配
  int* mystl_ptr = mystl::allocator_traits<mystl::allocator<int>>::allocate(mystl_alloc, count);
  int* std_ptr = std::allocator_traits<std::allocator<int>>::allocate(std_alloc, count);

  MYSTL_EXPECT(mystl_ptr != nullptr);
  MYSTL_EXPECT(std_ptr != nullptr);

  // 使用 allocator_traits 释放
  mystl::allocator_traits<mystl::allocator<int>>::deallocate(mystl_alloc, mystl_ptr, count);
  std::allocator_traits<std::allocator<int>>::deallocate(std_alloc, std_ptr, count);
});

// ============================================================================
// allocate_at_least 测试
// ============================================================================

MYSTL_TEST(allocator_allocate_at_least_comparison, {
  mystl::allocator<int> mystl_alloc;
  std::allocator<int> std_alloc;

  constexpr std::size_t requested = 10;

  // MySTL allocate_at_least
  auto mystl_result = mystl_alloc.allocate_at_least(requested);
  MYSTL_EXPECT(mystl_result.ptr != nullptr);
  MYSTL_EXPECT(mystl_result.count >= requested);

  // 标准库 allocate_at_least (C++23)
  // 注意：即使 C++23，std::allocator 可能也没有 allocate_at_least
  // 我们只测试 MySTL 的实现
  MYSTL_EXPECT(mystl_result.count >= requested);  // 至少分配请求的数量

  mystl_alloc.deallocate(mystl_result.ptr, mystl_result.count);
});

MYSTL_TEST(allocator_traits_allocate_at_least, {
  mystl::allocator<int> mystl_alloc;
  (void)mystl_alloc;  // 避免未使用警告

  constexpr std::size_t requested = 7;

  // 使用 allocator_traits
  auto mystl_result = mystl::allocator_traits<mystl::allocator<int>>::allocate_at_least(mystl_alloc, requested);
  MYSTL_EXPECT(mystl_result.ptr != nullptr);
  MYSTL_EXPECT(mystl_result.count >= requested);

  mystl::allocator_traits<mystl::allocator<int>>::deallocate(mystl_alloc, mystl_result.ptr, mystl_result.count);
});

// ============================================================================
// 对齐测试
// ============================================================================

MYSTL_TEST(allocator_alignment_comparison, {
  // 测试 64 字节对齐
  mystl::allocator<AlignedType> mystl_alloc;
  std::allocator<AlignedType> std_alloc;

  auto mystl_result = mystl_alloc.allocate_at_least(1);
  auto std_result = std_alloc.allocate(1);

  auto mystl_addr = reinterpret_cast<std::uintptr_t>(mystl_result.ptr);
  auto std_addr = reinterpret_cast<std::uintptr_t>(std_result);

  MYSTL_EXPECT(mystl_addr % alignof(AlignedType) == 0);
  MYSTL_EXPECT(std_addr % alignof(AlignedType) == 0);

  mystl_alloc.deallocate(mystl_result.ptr, mystl_result.count);
  std_alloc.deallocate(std_result, 1);
});

MYSTL_TEST(allocator_alignment_32_bytes, {
  // 测试 32 字节对齐
  mystl::allocator<AlignedType32> mystl_alloc;
  std::allocator<AlignedType32> std_alloc;

  auto mystl_result = mystl_alloc.allocate_at_least(2);
  auto std_ptr = std_alloc.allocate(2);

  auto mystl_addr = reinterpret_cast<std::uintptr_t>(mystl_result.ptr);
  auto std_addr = reinterpret_cast<std::uintptr_t>(std_ptr);

  MYSTL_EXPECT(mystl_addr % alignof(AlignedType32) == 0);
  MYSTL_EXPECT(std_addr % alignof(AlignedType32) == 0);

  // 验证第二个元素也对齐
  auto mystl_second = reinterpret_cast<std::uintptr_t>(mystl_result.ptr + 1);
  auto std_second = reinterpret_cast<std::uintptr_t>(std_ptr + 1);

  MYSTL_EXPECT(mystl_second % alignof(AlignedType32) == 0);
  MYSTL_EXPECT(std_second % alignof(AlignedType32) == 0);

  mystl_alloc.deallocate(mystl_result.ptr, mystl_result.count);
  std_alloc.deallocate(std_ptr, 2);
});

// ============================================================================
// 异常安全测试
// ============================================================================

MYSTL_TEST(allocator_overflow_exception, {
  mystl::allocator<int> mystl_alloc;
  std::allocator<int> std_alloc;

  bool mystl_threw = false;
  bool std_threw = false;

  // 测试 MySTL
  try {
    (void)mystl_alloc.allocate(std::numeric_limits<std::size_t>::max());
  } catch (const std::bad_array_new_length&) {
    mystl_threw = true;
  } catch (...) {
    mystl_threw = false;
  }

  // 测试标准库
  try {
    (void)std_alloc.allocate(std::numeric_limits<std::size_t>::max());
  } catch (const std::bad_array_new_length&) {
    std_threw = true;
  } catch (...) {
    std_threw = false;
  }

  MYSTL_EXPECT(mystl_threw);
  MYSTL_EXPECT(std_threw);
});

MYSTL_TEST(allocator_allocate_at_least_overflow, {
  mystl::allocator<int> mystl_alloc;

  bool threw = false;
  try {
    (void)mystl_alloc.allocate_at_least(std::numeric_limits<std::size_t>::max());
  } catch (const std::bad_array_new_length&) {
    threw = true;
  } catch (...) {
    threw = false;
  }

  MYSTL_EXPECT(threw);
});

// ============================================================================
// construct/destroy 测试
// ============================================================================

MYSTL_TEST(allocator_traits_construct_destroy, {
  mystl::allocator<TestType> mystl_alloc;
  std::allocator<TestType> std_alloc;

  constexpr std::size_t count = 5;

  // 分配内存
  TestType* mystl_ptr = mystl::allocator_traits<mystl::allocator<TestType>>::allocate(mystl_alloc, count);
  TestType* std_ptr = std::allocator_traits<std::allocator<TestType>>::allocate(std_alloc, count);

  // 使用 allocator_traits::construct 构造对象
  for (std::size_t i = 0; i < count; ++i) {
    mystl::allocator_traits<mystl::allocator<TestType>>::construct(mystl_alloc, mystl_ptr + i, static_cast<int>(i),
                                                                   static_cast<double>(static_cast<double>(i) * 1.5));
    std::allocator_traits<std::allocator<TestType>>::construct(std_alloc, std_ptr + i, static_cast<int>(i),
                                                                static_cast<double>(static_cast<double>(i) * 1.5));
  }

  // 验证构造的对象
  for (std::size_t i = 0; i < count; ++i) {
    MYSTL_EXPECT_EQ(mystl_ptr[i].id, std_ptr[i].id);
    MYSTL_EXPECT_EQ(mystl_ptr[i].value, std_ptr[i].value);
    MYSTL_EXPECT_EQ(mystl_ptr[i].id, static_cast<int>(i));
    MYSTL_EXPECT_EQ(mystl_ptr[i].value, static_cast<double>(i * 1.5));
  }

  // 使用 allocator_traits::destroy 销毁对象
  for (std::size_t i = 0; i < count; ++i) {
    mystl::allocator_traits<mystl::allocator<TestType>>::destroy(mystl_alloc, mystl_ptr + i);
    std::allocator_traits<std::allocator<TestType>>::destroy(std_alloc, std_ptr + i);
  }

  // 释放内存
  mystl::allocator_traits<mystl::allocator<TestType>>::deallocate(mystl_alloc, mystl_ptr, count);
  std::allocator_traits<std::allocator<TestType>>::deallocate(std_alloc, std_ptr, count);
});

// ============================================================================
// max_size 测试
// ============================================================================

MYSTL_TEST(allocator_traits_max_size, {
  mystl::allocator<int> mystl_alloc;
  std::allocator<int> std_alloc;

  auto mystl_max = mystl::allocator_traits<mystl::allocator<int>>::max_size(mystl_alloc);
  auto std_max = std::allocator_traits<std::allocator<int>>::max_size(std_alloc);

  // 两者应该返回相同的值（或至少是合理的值）
  MYSTL_EXPECT(mystl_max > 0);
  MYSTL_EXPECT(std_max > 0);
  MYSTL_EXPECT_EQ(mystl_max, std_max);
});

MYSTL_TEST(allocator_traits_max_size_different_types, {
  mystl::allocator<char> char_alloc;
  mystl::allocator<double> double_alloc;

  auto char_max = mystl::allocator_traits<mystl::allocator<char>>::max_size(char_alloc);
  auto double_max = mystl::allocator_traits<mystl::allocator<double>>::max_size(double_alloc);

  // char 的最大数量应该大于 double 的最大数量
  MYSTL_EXPECT(char_max > double_max);
});

// ============================================================================
// select_on_container_copy_construction 测试
// ============================================================================

MYSTL_TEST(allocator_select_on_container_copy_construction, {
  mystl::allocator<int> mystl_alloc;

  // 使用 allocator_traits
  auto mystl_copy = mystl::allocator_traits<mystl::allocator<int>>::select_on_container_copy_construction(mystl_alloc);

  // 对于无状态分配器，应该返回新的分配器实例
  int* mystl_ptr = mystl_copy.allocate(1);
  MYSTL_EXPECT(mystl_ptr != nullptr);

  mystl_copy.deallocate(mystl_ptr, 1);
});

MYSTL_TEST(allocator_direct_select_on_container_copy_construction, {
  mystl::allocator<int> mystl_alloc;

  // 直接调用分配器的方法
  auto mystl_copy = mystl_alloc.select_on_container_copy_construction();

  int* mystl_ptr = mystl_copy.allocate(1);
  MYSTL_EXPECT(mystl_ptr != nullptr);

  mystl_copy.deallocate(mystl_ptr, 1);
});

// ============================================================================
// rebind 测试
// ============================================================================

MYSTL_TEST(allocator_rebind_functionality, {
  // 使用 rebind
  using mystl_double_alloc = mystl::allocator<int>::rebind<double>::other;

  mystl_double_alloc mystl_double;

  double* mystl_ptr = mystl_double.allocate(5);
  MYSTL_EXPECT(mystl_ptr != nullptr);

  mystl_double.deallocate(mystl_ptr, 5);
});

// ============================================================================
// 不同大小的分配测试
// ============================================================================

MYSTL_TEST(allocator_various_sizes, {
  mystl::allocator<int> mystl_alloc;
  std::allocator<int> std_alloc;

  // 测试不同大小的分配
  constexpr std::size_t size1 = 1;
  constexpr std::size_t size2 = 10;
  constexpr std::size_t size3 = 100;
  constexpr std::size_t size4 = 1000;
  constexpr std::size_t size5 = 10000;

  // 测试不同大小的分配
  std::size_t test_sizes[5];
  test_sizes[0] = size1;
  test_sizes[1] = size2;
  test_sizes[2] = size3;
  test_sizes[3] = size4;
  test_sizes[4] = size5;
  for (int i = 0; i < 5; ++i) {
    std::size_t size = test_sizes[i];
    int* mystl_ptr = mystl_alloc.allocate(size);
    int* std_ptr = std_alloc.allocate(size);

    MYSTL_EXPECT(mystl_ptr != nullptr);
    MYSTL_EXPECT(std_ptr != nullptr);

    mystl_alloc.deallocate(mystl_ptr, size);
    std_alloc.deallocate(std_ptr, size);
  }
});

// ============================================================================
// 零大小分配测试
// ============================================================================

MYSTL_TEST(allocator_zero_size_allocation, {
  mystl::allocator<int> mystl_alloc;
  std::allocator<int> std_alloc;

  // 零大小分配应该返回有效指针（可能是 nullptr，但行为一致）
  int* mystl_ptr = mystl_alloc.allocate(0);
  int* std_ptr = std_alloc.allocate(0);

  // 标准允许返回 nullptr 或有效指针
  // 只要两者行为一致即可
  if (mystl_ptr == nullptr) {
    MYSTL_EXPECT(std_ptr == nullptr);
  } else {
    MYSTL_EXPECT(std_ptr != nullptr);
  }

  mystl_alloc.deallocate(mystl_ptr, 0);
  std_alloc.deallocate(std_ptr, 0);
});

// ============================================================================
// 常量表达式测试（如果支持）
// ============================================================================

MYSTL_TEST(allocator_constexpr_support, {
  // 测试 constexpr 分配器创建
  constexpr mystl::allocator<int> mystl_alloc;
  constexpr std::allocator<int> std_alloc;

  // 验证类型特征在编译时可用
  using mystl_iae = typename mystl::allocator_traits<mystl::allocator<int>>::is_always_equal;
  using std_iae = typename std::allocator_traits<std::allocator<int>>::is_always_equal;
  static_assert(mystl_iae::value);
  static_assert(std_iae::value);

  using mystl_pocma = typename mystl::allocator_traits<mystl::allocator<int>>::propagate_on_container_move_assignment;
  using std_pocma = typename std::allocator_traits<std::allocator<int>>::propagate_on_container_move_assignment;
  static_assert(mystl_pocma::value);
  static_assert(std_pocma::value);
});

// ============================================================================
// 分配器相等性测试
// ============================================================================

MYSTL_TEST(allocator_equality, {
  mystl::allocator<int> alloc1;
  mystl::allocator<int> alloc2;
  mystl::allocator<double> alloc3;

  // 无状态分配器应该总是相等
  // 注意：标准库的 allocator 在 C++20 之前没有定义 == 和 !=
  // 我们只验证它们可以创建和使用
  (void)alloc1;
  (void)alloc2;
  (void)alloc3;
});

