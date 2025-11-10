#include "tests/framework/mystl_test.hpp"
#include "mystl/memory/allocator.hpp"

#include <cstdint>
#include <limits>
#include <memory>

namespace {

struct alignas(64) AlignedPayload {
  int value;
};

}  // namespace

MYSTL_TEST(allocator_allocate_basic, {
  mystl::allocator<int> alloc;
  constexpr std::size_t count = 8;

  int* ptr = alloc.allocate(count);
  MYSTL_EXPECT(ptr != nullptr);

  for (std::size_t i = 0; i < count; ++i) {
    std::construct_at(ptr + i, static_cast<int>(i));
  }

  for (std::size_t i = 0; i < count; ++i) {
    MYSTL_EXPECT_EQ(ptr[i], static_cast<int>(i));
    std::destroy_at(ptr + i);
  }

  alloc.deallocate(ptr, count);
});

MYSTL_TEST(allocator_allocate_at_least, {
  mystl::allocator<int> alloc;

  auto result = alloc.allocate_at_least(5);
  MYSTL_EXPECT(result.ptr != nullptr);
  MYSTL_EXPECT(result.count >= 5);

  alloc.deallocate(result.ptr, result.count);
});

MYSTL_TEST(allocator_alignment_respected, {
  mystl::allocator<AlignedPayload> alloc;
  auto result = alloc.allocate_at_least(1);

  auto address = reinterpret_cast<std::uintptr_t>(result.ptr);
  MYSTL_EXPECT(address % alignof(AlignedPayload) == 0);

  alloc.deallocate(result.ptr, result.count);
});

MYSTL_TEST(allocator_overflow_throws, {
  mystl::allocator<int> alloc;
  bool threw_length = false;

  try {
    (void)alloc.allocate(std::numeric_limits<std::size_t>::max());
  } catch (const std::bad_array_new_length&) {
    threw_length = true;
  } catch (...) {
    threw_length = false;
  }

  MYSTL_EXPECT(threw_length);
});

MYSTL_TEST(allocator_select_on_container_copy_construction, {
  mystl::allocator<int> alloc;
  auto copy = alloc.select_on_container_copy_construction();

  int* ptr = copy.allocate(1);
  std::construct_at(ptr, 42);
  MYSTL_EXPECT_EQ(*ptr, 42);
  std::destroy_at(ptr);
  copy.deallocate(ptr, 1);
});


