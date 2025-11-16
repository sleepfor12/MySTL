#ifndef MYSTL_MEMORY_ALLOCATOR_TRAITS_HPP
#define MYSTL_MEMORY_ALLOCATOR_TRAITS_HPP

#include <limits>
#include <type_traits>
#include <utility>

#include "mystl/memory/allocation_result.hpp"
#include "mystl/memory/pointer_traits.hpp"
#include "mystl/memory/allocator.hpp"

namespace mystl {

#if __cplusplus >= 201703L
using std::void_t;
#else
template <class...>
using void_t = void;
#endif

template <class Alloc>
struct allocator_traits {
  using allocator_type = Alloc;
  using value_type = typename Alloc::value_type;

  template <typename T, typename = void>
  struct has_pointer : std::false_type {};
  template <typename T>
  struct has_pointer<T, void_t<typename T::pointer>> : std::true_type {
    using type = typename T::pointer;
  };
  using pointer = typename std::conditional_t<has_pointer<Alloc>::value, has_pointer<Alloc>, std::type_identity<value_type*>>::type;

  template <typename T, typename = void>
  struct has_const_pointer : std::false_type {};
  template <typename T>
  struct has_const_pointer<T, void_t<typename T::const_pointer>> : std::true_type {
    using type = typename T::const_pointer;
  };
  using const_pointer = typename std::conditional_t<has_const_pointer<Alloc>::value, has_const_pointer<Alloc>,
                                                   std::type_identity<typename pointer_traits<pointer>::template rebind<const value_type>>>::type;

  template <typename T, typename = void>
  struct has_void_pointer : std::false_type {};
  template <typename T>
  struct has_void_pointer<T, void_t<typename T::void_pointer>> : std::true_type {
    using type = typename T::void_pointer;
  };
  using void_pointer = typename std::conditional_t<has_void_pointer<Alloc>::value, has_void_pointer<Alloc>,
                                                  std::type_identity<typename pointer_traits<pointer>::template rebind<void>>>::type;

  template <typename T, typename = void>
  struct has_const_void_pointer : std::false_type {};
  template <typename T>
  struct has_const_void_pointer<T, void_t<typename T::const_void_pointer>> : std::true_type {
    using type = typename T::const_void_pointer;
  };
  using const_void_pointer = typename std::conditional_t<has_const_void_pointer<Alloc>::value, has_const_void_pointer<Alloc>,
                                                         std::type_identity<typename pointer_traits<pointer>::template rebind<const void>>>::type;

  template <typename T, typename = void>
  struct has_difference_type : std::false_type {};
  template <typename T>
  struct has_difference_type<T, void_t<typename T::difference_type>> : std::true_type {};
  using difference_type = std::conditional_t<has_difference_type<Alloc>::value, typename Alloc::difference_type,
                                             typename pointer_traits<pointer>::difference_type>;

  template <typename T, typename = void>
  struct has_size_type : std::false_type {};
  template <typename T>
  struct has_size_type<T, void_t<typename T::size_type>> : std::true_type {};
  using size_type = std::conditional_t<has_size_type<Alloc>::value, typename Alloc::size_type,
                                       typename std::make_unsigned<difference_type>::type>;

  template <typename T, typename = void>
  struct has_pocca : std::false_type {};
  template <typename T>
  struct has_pocca<T, void_t<typename T::propagate_on_container_copy_assignment>> : std::true_type {
    using type = typename T::propagate_on_container_copy_assignment;
  };
  using propagate_on_container_copy_assignment =
      typename std::conditional_t<has_pocca<Alloc>::value, has_pocca<Alloc>, std::type_identity<std::false_type>>::type;

  template <typename T, typename = void>
  struct has_pocma : std::false_type {};
  template <typename T>
  struct has_pocma<T, void_t<typename T::propagate_on_container_move_assignment>> : std::true_type {
    using type = typename T::propagate_on_container_move_assignment;
  };
  using propagate_on_container_move_assignment =
      typename std::conditional_t<has_pocma<Alloc>::value, has_pocma<Alloc>, std::type_identity<std::false_type>>::type;

  template <typename T, typename = void>
  struct has_pocs : std::false_type {};
  template <typename T>
  struct has_pocs<T, void_t<typename T::propagate_on_container_swap>> : std::true_type {
    using type = typename T::propagate_on_container_swap;
  };
  using propagate_on_container_swap =
      typename std::conditional_t<has_pocs<Alloc>::value, has_pocs<Alloc>, std::type_identity<std::false_type>>::type;

  template <typename T, typename = void>
  struct has_iae : std::false_type {};
  template <typename T>
  struct has_iae<T, void_t<typename T::is_always_equal>> : std::true_type {
    using type = typename T::is_always_equal;
  };
  using is_always_equal =
      typename std::conditional_t<has_iae<Alloc>::value, has_iae<Alloc>, std::type_identity<typename std::is_empty<Alloc>::type>>::type;

  template <typename T>
  using rebind_alloc = typename Alloc::template rebind<T>::other;

  template <typename T>
  using rebind_traits = allocator_traits<rebind_alloc<T>>;

  constexpr static pointer allocate(Alloc& a, size_type n) { return a.allocate(n); }

  constexpr static pointer allocate(Alloc& a, size_type n, const_void_pointer hint) {
    if constexpr (requires { { a.allocate(n, hint) } -> std::convertible_to<pointer>; }) {
      return a.allocate(n, hint);
    } else {
      return a.allocate(n);
    }
  }

  constexpr static allocation_result<pointer, size_type> allocate_at_least(Alloc& a, size_type n) {
    if constexpr (requires { { a.allocate_at_least(n) } -> std::convertible_to<allocation_result<pointer, size_type>>; }) {
      return a.allocate_at_least(n);
    } else {
      return {a.allocate(n), n};
    }
  }

  constexpr static void deallocate(Alloc& a, pointer p, size_type n) noexcept { a.deallocate(p, n); }

  template <class T, class... Args>
  constexpr static void construct(Alloc& a, T* p, Args&&... args) {
    if constexpr (requires { a.construct(p, std::forward<Args>(args)...); }) {
      a.construct(p, std::forward<Args>(args)...);
    } else {
      std::construct_at(p, std::forward<Args>(args)...);
    }
  }
  template <class T>
  constexpr static void destroy(Alloc& a, T* p) {
    if constexpr (requires { a.destroy(p); }) {
      a.destroy(p);
    } else {
      std::destroy_at(p);
    }
  }

  constexpr static size_type max_size(const Alloc& a) noexcept {
    if constexpr (requires { { a.max_size() } -> std::convertible_to<size_type>; }) {
      return a.max_size();
    } else {
      return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }
  }

  // select_on_container_copy_construction - 容器复制构造时的分配器选择
  constexpr static Alloc select_on_container_copy_construction(const Alloc& a) {
    if constexpr (requires { { a.select_on_container_copy_construction() } -> std::convertible_to<Alloc>; }) {
      return a.select_on_container_copy_construction();
    } else {
      return a;
    }
  }
};

}  // namespace mystl

#endif  // MYSTL_MEMORY_ALLOCATOR_TRAITS_HPP
