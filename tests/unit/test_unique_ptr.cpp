#include "tests/framework/mystl_test.hpp"

#include "mystl/memory/unique_ptr.hpp"

#include <cstddef>
#include <type_traits>
#include <functional>
#include <memory>
#include <utility>

namespace {

struct Counted {
  static int constructions;
  static int destructions;
  int value{};

  Counted(int v = 0) : value(v) { ++constructions; }
  ~Counted() { ++destructions; }

  Counted(const Counted&) = delete;
  Counted& operator=(const Counted&) = delete;
};

int Counted::constructions = 0;
int Counted::destructions = 0;

inline void reset_counted() {
  Counted::constructions = 0;
  Counted::destructions = 0;
}

struct CountingDeleter {
  int* calls;

  void operator()(int* ptr) const {
    ++(*calls);
    delete ptr;
  }
};

struct CountedDeleter {
  int* calls;

  void operator()(Counted* ptr) const {
    ++(*calls);
    delete ptr;
  }
};

struct CountedArrayDeleter {
  int* calls;

  void operator()(Counted* ptr) const {
    ++(*calls);
    delete[] ptr;
  }
};

using UniqueIntCountingPtr = mystl::unique_ptr<int, CountingDeleter>;
using UniqueCountedRefPtr = mystl::unique_ptr<Counted, CountedDeleter&>;
using UniqueCountedArrayPtr = mystl::unique_ptr<Counted[], CountedArrayDeleter>;

struct Base {
  virtual ~Base() = default;
  int tag = 0;
};

struct Derived : Base {
  explicit Derived(int v) { tag = v; }
};

struct FancyDeleter {
  using pointer = Counted*;

  void operator()(pointer ptr) const { delete ptr; }
};

static_assert(std::is_same_v<typename mystl::unique_ptr<Counted, FancyDeleter>::pointer, Counted*>);
static_assert(std::is_same_v<typename mystl::unique_ptr<int>::pointer, typename std::unique_ptr<int>::pointer>);
static_assert(std::is_same_v<typename mystl::unique_ptr<Counted[], CountedArrayDeleter>::pointer,
                             typename std::unique_ptr<Counted[], CountedArrayDeleter>::pointer>);

}  // namespace

MYSTL_TEST(unique_ptr_default_ctor, {
  mystl::unique_ptr<int> p;
  MYSTL_EXPECT(!p);
  MYSTL_EXPECT_EQ(p.get(), nullptr);
  MYSTL_EXPECT_EQ(static_cast<bool>(p), false);
});

MYSTL_TEST(unique_ptr_basic_make_unique, {
  reset_counted();
  {
    auto ptr = mystl::make_unique<Counted>(42);
    MYSTL_EXPECT(static_cast<bool>(ptr));
    MYSTL_EXPECT_EQ(ptr->value, 42);
    MYSTL_EXPECT_EQ(Counted::constructions, 1);
    MYSTL_EXPECT_EQ(Counted::destructions, 0);
  }
  MYSTL_EXPECT_EQ(Counted::destructions, 1);
});

MYSTL_TEST(unique_ptr_move_semantics, {
  auto original = mystl::unique_ptr<int>(new int(7));
  MYSTL_EXPECT(static_cast<bool>(original));

  auto moved = mystl::unique_ptr<int>(std::move(original));
  MYSTL_EXPECT(!original);
  MYSTL_EXPECT(static_cast<bool>(moved));
  MYSTL_EXPECT_EQ(*moved, 7);

  auto target = mystl::unique_ptr<int>(new int(9));
  target = std::move(moved);
  MYSTL_EXPECT(!moved);
  MYSTL_EXPECT(static_cast<bool>(target));
  MYSTL_EXPECT_EQ(*target, 7);
});

MYSTL_TEST(unique_ptr_release_and_reset, {
  auto ptr = mystl::unique_ptr<int>(new int(5));
  int* raw = ptr.release();
  MYSTL_EXPECT(!ptr);
  MYSTL_EXPECT_EQ(ptr.get(), nullptr);
  MYSTL_EXPECT_EQ(*raw, 5);

  ptr.reset(raw);
  MYSTL_EXPECT(static_cast<bool>(ptr));
  MYSTL_EXPECT_EQ(*ptr, 5);

  ptr.reset();
  MYSTL_EXPECT(!ptr);
});

MYSTL_TEST(unique_ptr_custom_deleter, {
  int calls = 0;
  {
    UniqueIntCountingPtr ptr(new int(3), CountingDeleter{&calls});
    MYSTL_EXPECT(static_cast<bool>(ptr));
    MYSTL_EXPECT_EQ(*ptr, 3);

    auto& deleter = ptr.get_deleter();
    MYSTL_EXPECT_EQ(deleter.calls, &calls);
  }
  MYSTL_EXPECT_EQ(calls, 1);
});

MYSTL_TEST(unique_ptr_array_management, {
  reset_counted();
  {
    mystl::unique_ptr<Counted[]> arr(new Counted[3]);
    MYSTL_EXPECT(static_cast<bool>(arr));
    MYSTL_EXPECT_EQ(Counted::constructions, 3);
    arr[0].value = 10;
    arr[1].value = 20;
    arr[2].value = 30;
    MYSTL_EXPECT_EQ(arr[1].value, 20);
  }
  MYSTL_EXPECT_EQ(Counted::destructions, 3);
});

MYSTL_TEST(unique_ptr_array_reset_overload, {
  reset_counted();
  mystl::unique_ptr<Counted[]> arr(new Counted[2]);
  Counted* raw = arr.get();
  arr.reset(raw);
  MYSTL_EXPECT(static_cast<bool>(arr));
  MYSTL_EXPECT_EQ(arr.get(), raw);
  arr.reset();
  MYSTL_EXPECT(!arr);
  MYSTL_EXPECT_EQ(Counted::destructions, 2);
});

MYSTL_TEST(unique_ptr_swap_and_bool, {
  mystl::unique_ptr<int> a(new int(1));
  mystl::unique_ptr<int> b;

  swap(a, b);
  MYSTL_EXPECT(!a);
  MYSTL_EXPECT(static_cast<bool>(b));
  MYSTL_EXPECT_EQ(*b, 1);

  b.swap(a);
  MYSTL_EXPECT(static_cast<bool>(a));
  MYSTL_EXPECT(!b);
});

MYSTL_TEST(unique_ptr_comparisons, {
  mystl::unique_ptr<int> a;
  mystl::unique_ptr<int> b;
  MYSTL_EXPECT(a == b);
  MYSTL_EXPECT(!(a < b));
  MYSTL_EXPECT(a == nullptr);

  auto c = mystl::unique_ptr<int>(new int(1));
  MYSTL_EXPECT(c != nullptr);
  MYSTL_EXPECT(c > a);
});

MYSTL_TEST(make_unique_array_value_initialization, {
  auto arr = mystl::make_unique<int[]>(4);
  for (std::size_t i = 0; i < 4; ++i) {
    MYSTL_EXPECT_EQ(arr[i], 0);
    arr[i] = static_cast<int>(i + 1);
  }
  for (std::size_t i = 0; i < 4; ++i) {
    MYSTL_EXPECT_EQ(arr[i], static_cast<int>(i + 1));
  }
});

MYSTL_TEST(make_unique_for_overwrite_object, {
  reset_counted();
  auto obj = mystl::make_unique_for_overwrite<Counted>();
  MYSTL_EXPECT(static_cast<bool>(obj));
  MYSTL_EXPECT_EQ(Counted::constructions, 1);
  obj->value = 77;
  MYSTL_EXPECT_EQ(obj->value, 77);
});

MYSTL_TEST(make_unique_for_overwrite_array, {
  reset_counted();
  auto arr = mystl::make_unique_for_overwrite<Counted[]>(3);
  MYSTL_EXPECT(static_cast<bool>(arr));
  MYSTL_EXPECT_EQ(Counted::constructions, 3);
  for (std::size_t i = 0; i < 3; ++i) {
    arr[i].value = static_cast<int>(i * 2);
  }
  for (std::size_t i = 0; i < 3; ++i) {
    MYSTL_EXPECT_EQ(arr[i].value, static_cast<int>(i * 2));
  }
});

MYSTL_TEST(unique_ptr_reference_deleter_lifetime, {
  reset_counted();
  int calls = 0;
  CountedDeleter deleter{&calls};
  {
    UniqueCountedRefPtr ptr(new Counted(13), deleter);
    MYSTL_EXPECT(static_cast<bool>(ptr));
    MYSTL_EXPECT_EQ(ptr->value, 13);
    auto& stored = ptr.get_deleter();
    MYSTL_EXPECT_EQ(&stored, &deleter);
  }
  MYSTL_EXPECT_EQ(calls, 1);
  MYSTL_EXPECT_EQ(Counted::destructions, 1);
});

MYSTL_TEST(unique_ptr_converting_constructor, {
  auto derived = mystl::unique_ptr<Derived>(new Derived(88));
  MYSTL_EXPECT(static_cast<bool>(derived));
  mystl::unique_ptr<Base> base(std::move(derived));
  MYSTL_EXPECT(!derived);
  MYSTL_EXPECT(static_cast<bool>(base));
  auto* dyn = dynamic_cast<Derived*>(base.get());
  MYSTL_EXPECT(dyn != nullptr);
  MYSTL_EXPECT_EQ(dyn->tag, 88);
});

MYSTL_TEST(unique_ptr_array_nullptr_ctor, {
  mystl::unique_ptr<int[]> arr(nullptr);
  MYSTL_EXPECT(!arr);
  arr.reset(new int[3]{});
  MYSTL_EXPECT(static_cast<bool>(arr));
  MYSTL_EXPECT_EQ(arr[0], 0);
});

MYSTL_TEST(unique_ptr_array_release_manual_delete, {
  reset_counted();
  Counted* raw = nullptr;
  {
    mystl::unique_ptr<Counted[]> arr(new Counted[2]);
    raw = arr.release();
    MYSTL_EXPECT(!arr);
  }
  MYSTL_EXPECT_EQ(Counted::destructions, 0);
  delete[] raw;
  MYSTL_EXPECT_EQ(Counted::destructions, 2);
});

MYSTL_TEST(unique_ptr_array_custom_deleter, {
  reset_counted();
  int calls = 0;
  {
    UniqueCountedArrayPtr arr(new Counted[4], CountedArrayDeleter{&calls});
    MYSTL_EXPECT(static_cast<bool>(arr));
    arr[3].value = 42;
    MYSTL_EXPECT_EQ(arr[3].value, 42);
  }
  MYSTL_EXPECT_EQ(calls, 1);
  MYSTL_EXPECT_EQ(Counted::destructions, 4);
});

MYSTL_TEST(unique_ptr_hash_support, {
  mystl::unique_ptr<int> value(new int(21));
  std::hash<mystl::unique_ptr<int>> hasher;
  auto h1 = hasher(value);
  auto h2 = hasher(value);
  MYSTL_EXPECT_EQ(h1, h2);

  int* raw = value.release();
  mystl::unique_ptr<int> second(raw);
  auto h3 = hasher(second);
  MYSTL_EXPECT_EQ(h1, h3);
});

MYSTL_TEST(unique_ptr_std_scalar_equivalence, {
  auto mystl_ptr = mystl::unique_ptr<int>(new int(42));
  auto std_ptr = std::unique_ptr<int>(new int(42));

  MYSTL_EXPECT_EQ(*mystl_ptr, *std_ptr);
  MYSTL_EXPECT_EQ(static_cast<bool>(mystl_ptr), static_cast<bool>(std_ptr));

  int* mystl_raw = mystl_ptr.release();
  int* std_raw = std_ptr.release();
  MYSTL_EXPECT_EQ(static_cast<bool>(mystl_ptr), static_cast<bool>(std_ptr));
  MYSTL_EXPECT(mystl_raw != nullptr);
  MYSTL_EXPECT(std_raw != nullptr);

  *mystl_raw = 99;
  *std_raw = 99;

  mystl_ptr.reset(mystl_raw);
  std_ptr.reset(std_raw);
  MYSTL_EXPECT_EQ(*mystl_ptr, *std_ptr);
  MYSTL_EXPECT_EQ(static_cast<bool>(mystl_ptr), static_cast<bool>(std_ptr));

  mystl_ptr.reset();
  std_ptr.reset();
  MYSTL_EXPECT_EQ(static_cast<bool>(mystl_ptr), static_cast<bool>(std_ptr));
  MYSTL_EXPECT_EQ(mystl_ptr.get(), nullptr);
  MYSTL_EXPECT_EQ(std_ptr.get(), nullptr);
});

MYSTL_TEST(unique_ptr_std_comparison_equivalence, {
  mystl::unique_ptr<int> mystl_a;
  mystl::unique_ptr<int> mystl_b(new int(7));
  std::unique_ptr<int> std_a;
  std::unique_ptr<int> std_b(new int(7));

  MYSTL_EXPECT_EQ(mystl_a == mystl_b, std_a == std_b);
  MYSTL_EXPECT_EQ(mystl_a != mystl_b, std_a != std_b);
  MYSTL_EXPECT_EQ(mystl_b == nullptr, std_b == nullptr);
  MYSTL_EXPECT_EQ(nullptr == mystl_a, nullptr == std_a);
  MYSTL_EXPECT_EQ(mystl_b > mystl_a, std_b > std_a);
  MYSTL_EXPECT_EQ(mystl_a < mystl_b, std_a < std_b);

  mystl::unique_ptr<int> mystl_c(new int(3));
  std::unique_ptr<int> std_c(new int(3));
  mystl_c.swap(mystl_b);
  std_c.swap(std_b);
  MYSTL_EXPECT_EQ(*mystl_b, *std_b);
  MYSTL_EXPECT_EQ(*mystl_c, *std_c);
});

MYSTL_TEST(unique_ptr_std_array_equivalence, {
  auto mystl_arr = mystl::make_unique<int[]>(3);
  auto std_arr = std::make_unique<int[]>(3);

  for (std::size_t i = 0; i < 3; ++i) {
    mystl_arr[i] = static_cast<int>(i + 1);
    std_arr[i] = static_cast<int>(i + 1);
    MYSTL_EXPECT_EQ(mystl_arr[i], std_arr[i]);
  }

  int* mystl_raw = mystl_arr.release();
  int* std_raw = std_arr.release();
  MYSTL_EXPECT(mystl_raw != nullptr);
  MYSTL_EXPECT(std_raw != nullptr);
  MYSTL_EXPECT_EQ(static_cast<bool>(mystl_arr), static_cast<bool>(std_arr));

  mystl_arr.reset(mystl_raw);
  std_arr.reset(std_raw);
  MYSTL_EXPECT_EQ(static_cast<bool>(mystl_arr), static_cast<bool>(std_arr));
  MYSTL_EXPECT_EQ(mystl_arr[2], std_arr[2]);

  mystl_arr.reset();
  std_arr.reset();
  MYSTL_EXPECT_EQ(static_cast<bool>(mystl_arr), static_cast<bool>(std_arr));
});


