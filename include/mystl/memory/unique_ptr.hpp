#ifndef MYSTL_MEMORY_UNIQUE_PTR_HPP
#define MYSTL_MEMORY_UNIQUE_PTR_HPP

/**
 * @file memory/unique_ptr.hpp
 * @brief 唯一所有权智能指针 (Unique Ownership Smart Pointer)
 */

#include <cstddef>
#include <functional>
#include <ostream>
#include <type_traits>
#include <utility>

#include "mystl/memory/pointer_traits.hpp"

namespace mystl {

namespace detail {

template <class Deleter, class T, class = void>
struct unique_ptr_pointer {
  using type = T*;
};

template <class Deleter, class T>
struct unique_ptr_pointer<Deleter, T, std::void_t<typename std::remove_reference_t<Deleter>::pointer>> {
  using type = typename std::remove_reference_t<Deleter>::pointer;
};

template <class Stream, class T, class = void>
struct is_streamable : std::false_type {};

template <class Stream, class T>
struct is_streamable<Stream, T, std::void_t<decltype(std::declval<Stream>() << std::declval<T>())>> : std::true_type {};

template <class Stream, class T>
inline constexpr bool is_streamable_v = is_streamable<Stream, T>::value;

template <class Deleter>
constexpr decltype(auto) unique_ptr_forward_deleter(Deleter& deleter) noexcept {
  if constexpr (std::is_reference_v<Deleter>) {
    return deleter;
  } else {
    return std::move(deleter);
  }
}

template <class Pointer, class U, class Element>
constexpr bool unique_ptr_array_pointer_acceptable_impl() {
  if constexpr (std::is_same_v<Pointer, Element*> && std::is_pointer_v<U>) {
    using value_type = std::remove_pointer_t<U>;
    return std::is_convertible_v<value_type(*)[], Element(*)[]>;
  } else {
    return false;
  }
}

template <class Pointer, class U, class Element>
inline constexpr bool unique_ptr_array_pointer_acceptable =
    std::is_same_v<U, Pointer> || std::is_same_v<U, std::nullptr_t> ||
    unique_ptr_array_pointer_acceptable_impl<Pointer, U, Element>();

template <class Pointer, class U>
constexpr Pointer unique_ptr_array_pointer_cast(U&& p) noexcept {
  if constexpr (std::is_same_v<std::decay_t<U>, std::nullptr_t>) {
    return Pointer{};
  } else {
    return static_cast<Pointer>(std::forward<U>(p));
  }
}

}  // namespace detail

template <class T>
struct default_delete {
  constexpr default_delete() noexcept = default;
  template <class U, std::enable_if_t<std::is_convertible_v<U*, T*>, int> = 0>
  constexpr default_delete(const default_delete<U>&) noexcept {}

  constexpr void operator()(T* ptr) const { delete ptr; }
};

// 数组特化的默认删除器
template <class T>
struct default_delete<T[]> {
  constexpr default_delete() noexcept = default;
  template <class U, std::enable_if_t<std::is_convertible_v<U (*)[], T (*)[]>, int> = 0>
  constexpr default_delete(const default_delete<U[]>&) noexcept {}

  template <class U>
  constexpr void operator()(U* ptr) const {
    static_assert(sizeof(U) > 0, "default_delete requires a complete type");
    delete[] ptr;
  }
};

template <class T, class Deleter = default_delete<T>>
class unique_ptr {
public:
  // 类型定义
  using element_type = T;
  using deleter_type = Deleter;
  using pointer = typename detail::unique_ptr_pointer<Deleter, element_type>::type;

  template <class, class>
  friend class unique_ptr;

  constexpr unique_ptr() noexcept(std::is_nothrow_default_constructible_v<deleter_type>)
    requires(std::is_default_constructible_v<deleter_type> && !std::is_pointer_v<deleter_type>)
      : p_(pointer{}), d_() {}

  constexpr unique_ptr(std::nullptr_t) noexcept(std::is_nothrow_default_constructible_v<deleter_type>)
    requires(std::is_default_constructible_v<deleter_type> && !std::is_pointer_v<deleter_type>)
      : p_(pointer{}), d_() {}

  constexpr explicit unique_ptr(pointer p) noexcept(std::is_nothrow_default_constructible_v<deleter_type>)
    requires(std::is_default_constructible_v<deleter_type> && !std::is_pointer_v<deleter_type>)
      : p_(p), d_() {}

  constexpr unique_ptr(pointer p, const deleter_type& d) noexcept(std::is_nothrow_copy_constructible_v<deleter_type>)
    requires(!std::is_reference_v<deleter_type> && std::is_copy_constructible_v<deleter_type>)
      : p_(p), d_(d) {}

  constexpr unique_ptr(pointer p, deleter_type&& d) noexcept(std::is_nothrow_move_constructible_v<deleter_type>)
    requires(!std::is_reference_v<deleter_type> && std::is_move_constructible_v<deleter_type>)
      : p_(p), d_(std::move(d)) {}

  constexpr unique_ptr(pointer p, deleter_type& d) noexcept
    requires std::is_lvalue_reference_v<deleter_type>
      : p_(p), d_(d) {}

  constexpr unique_ptr(pointer p, std::remove_reference_t<deleter_type>&& d)
    requires std::is_lvalue_reference_v<deleter_type>
  = delete;

  unique_ptr(const unique_ptr&) = delete;

  constexpr unique_ptr(unique_ptr&& other) noexcept
    requires std::is_move_constructible_v<deleter_type>
      : p_(other.release()), d_(detail::unique_ptr_forward_deleter<deleter_type>(other.d_)) {}
  template <class U, class E>
  constexpr unique_ptr(unique_ptr<U, E>&& u) noexcept(std::is_nothrow_constructible_v<deleter_type, E&> ||
                                                      std::is_nothrow_constructible_v<deleter_type, E&&>)
    requires(std::is_convertible_v<typename unique_ptr<U, E>::pointer, pointer> && !std::is_array_v<U> &&
             ((std::is_reference_v<deleter_type> && std::is_same_v<E, deleter_type>) ||
              (!std::is_reference_v<deleter_type> && std::is_convertible_v<E, deleter_type>)))
      : p_(static_cast<pointer>(u.release())), d_([&] {
          if constexpr (std::is_reference_v<deleter_type>) {
            return u.get_deleter();
          } else {
            return std::move(u.get_deleter());
          }
        }()) {
    u.p_ = nullptr;
  }

  constexpr ~unique_ptr() {
    pointer ptr = p_;
    if (ptr == nullptr) {
      return;
    }

    static_assert(sizeof(element_type) > 0, "unique_ptr requires a complete type at destruction");

    d_(ptr);
  }

  unique_ptr& operator=(const unique_ptr&) = delete;

  constexpr unique_ptr& operator=(unique_ptr&& r) noexcept {
    static_assert(std::is_move_assignable_v<deleter_type>,
                  "unique_ptr deleter must be MoveAssignable for move assignment");
    if (this != &r) {
      pointer new_ptr = r.p_;
      r.p_ = nullptr;
      reset(new_ptr);
      get_deleter() = std::forward<deleter_type>(r.d_);
    }
    return *this;
  }

  template <class U, class E>
  constexpr unique_ptr& operator=(unique_ptr<U, E>&& r) noexcept {
    static_assert(std::is_assignable_v<deleter_type&, E&&>,
                  "unique_ptr deleter must be assignable from E&& for converting move assignment");
    using other_pointer = typename unique_ptr<U, E>::pointer;
    static_assert(std::is_convertible_v<other_pointer, pointer>,
                  "unique_ptr converting assignment requires convertible pointer");
    pointer new_ptr = static_cast<pointer>(r.p_);
    r.p_ = nullptr;
    reset(new_ptr);
    get_deleter() = std::forward<E>(r.d_);
    return *this;
  }

  constexpr unique_ptr& operator=(std::nullptr_t) noexcept {
    reset();
    return *this;
  }

  // 解引用
  constexpr typename std::add_lvalue_reference<T>::type operator*() const noexcept(noexcept(*std::declval<pointer>())) {
#if defined(__cpp_lib_reference_from_temporary)
    static_assert(
        !std::reference_converts_from_temporary_v<std::add_lvalue_reference_t<T>, decltype(*std::declval<pointer>())>,
        "unique_ptr::operator* would bind to a temporary; program is ill-formed");
#endif
    return *get();
  }
  constexpr pointer operator->() const noexcept { return get(); }

  // 获取指针
  constexpr pointer get() const noexcept { return p_ ? p_ : nullptr; }

  // 获取删除器
  constexpr deleter_type& get_deleter() noexcept { return d_; }

  constexpr const deleter_type& get_deleter() const noexcept { return d_; }

  constexpr explicit operator bool() const noexcept { return get() != nullptr; }

  // 释放所有权
  constexpr pointer release() noexcept {
    pointer old_ptr = get();
    p_ = nullptr;
    return old_ptr;
  }

  // 重置
  constexpr void reset(pointer p = pointer()) noexcept {
    pointer old_ptr = p_;
    if (old_ptr == p) {
      return;
    }
    p_ = p;

    if (old_ptr) {
      static_assert(sizeof(element_type) > 0, "unique_ptr requires a complete type at reset");
      get_deleter()(old_ptr);
    }
  }

  // 交换
  void swap(unique_ptr& other) noexcept {
    std::swap(p_, other.p_);
    std::swap(d_, other.d_);
  }

private:
  pointer p_ = pointer{};  // 存储的指针
  deleter_type d_;         // 删除器
};

// 数组特化
template <class T, class Deleter>
class unique_ptr<T[], Deleter> {
public:
  // 类型定义
  using element_type = T;
  using deleter_type = Deleter;
  using pointer = typename detail::unique_ptr_pointer<Deleter, element_type>::type;

  template <class, class>
  friend class unique_ptr;

  // 构造函数（C++23）
  constexpr unique_ptr() noexcept(std::is_nothrow_default_constructible_v<deleter_type>)
    requires(std::is_default_constructible_v<deleter_type> && !std::is_pointer_v<deleter_type>)
      : p_(pointer{}), d_() {}

  constexpr unique_ptr(std::nullptr_t) noexcept(std::is_nothrow_default_constructible_v<deleter_type>)
    requires(std::is_default_constructible_v<deleter_type> && !std::is_pointer_v<deleter_type>)
      : p_(pointer{}), d_() {}

  template <class U>
  constexpr explicit unique_ptr(U p) noexcept(std::is_nothrow_default_constructible_v<deleter_type>)
    requires(std::is_default_constructible_v<deleter_type> && !std::is_pointer_v<deleter_type> &&
             detail::unique_ptr_array_pointer_acceptable<pointer, U, element_type>)
      : p_(detail::unique_ptr_array_pointer_cast<pointer>(std::forward<U>(p))), d_() {}

  template <class U>
  constexpr unique_ptr(U p, const deleter_type& d) noexcept(std::is_nothrow_copy_constructible_v<deleter_type>)
    requires(!std::is_reference_v<deleter_type> && std::is_copy_constructible_v<deleter_type> &&
             detail::unique_ptr_array_pointer_acceptable<pointer, U, element_type>)
      : p_(detail::unique_ptr_array_pointer_cast<pointer>(std::forward<U>(p))), d_(d) {}

  template <class U>
  constexpr unique_ptr(U p, deleter_type&& d) noexcept(std::is_nothrow_move_constructible_v<deleter_type>)
    requires(!std::is_reference_v<deleter_type> && std::is_move_constructible_v<deleter_type> &&
             detail::unique_ptr_array_pointer_acceptable<pointer, U, element_type>)
      : p_(detail::unique_ptr_array_pointer_cast<pointer>(std::forward<U>(p))), d_(std::move(d)) {}

  template <class U>
  constexpr unique_ptr(U p, deleter_type& d) noexcept
    requires(std::is_lvalue_reference_v<deleter_type> &&
             detail::unique_ptr_array_pointer_acceptable<pointer, U, element_type>)
      : p_(detail::unique_ptr_array_pointer_cast<pointer>(std::forward<U>(p))), d_(d) {}

  template <class U>
  constexpr unique_ptr(U, std::remove_reference_t<deleter_type>&&)
    requires(std::is_lvalue_reference_v<deleter_type> &&
             detail::unique_ptr_array_pointer_acceptable<pointer, U, element_type>)
  = delete;

  unique_ptr(unique_ptr&& other) noexcept
    requires std::is_move_constructible_v<deleter_type>
      : p_(other.release()), d_(detail::unique_ptr_forward_deleter<deleter_type>(other.d_)) {}
  unique_ptr(const unique_ptr&) = delete;
  template <class U, class E>
  constexpr unique_ptr(unique_ptr<U[], E>&& u) noexcept(std::is_nothrow_constructible_v<deleter_type, E&> ||
                                                        std::is_nothrow_constructible_v<deleter_type, E&&>)
    requires(std::is_convertible_v<typename unique_ptr<U, E>::pointer, pointer> && std::is_array_v<U> &&
             ((std::is_reference_v<deleter_type> && std::is_same_v<E, deleter_type>) ||
              (!std::is_reference_v<deleter_type> && std::is_convertible_v<E, deleter_type>)))
      : p_(detail::unique_ptr_array_pointer_cast<pointer>(u.release())), d_([&] {
          if constexpr (std::is_reference_v<deleter_type>) {
            return u.get_deleter();
          } else {
            return std::move(u.get_deleter());
          }
        }()) {
    u.p_ = nullptr;
  }

  // 析构函数
  constexpr ~unique_ptr() {
    pointer ptr = p_;
    if (ptr == nullptr) {
      return;
    }

    static_assert(sizeof(element_type) > 0, "unique_ptr requires a complete type at destruction");

    d_(ptr);
  }

  unique_ptr& operator=(const unique_ptr&) = delete;

  constexpr unique_ptr& operator=(unique_ptr&& r) noexcept {
    static_assert(std::is_move_assignable_v<deleter_type>,
                  "unique_ptr deleter must be MoveAssignable for move assignment");
    if (this != &r) {
      pointer new_ptr = r.p_;
      r.p_ = nullptr;
      reset(new_ptr);
      get_deleter() = std::forward<deleter_type>(r.d_);
    }
    return *this;
  }

  template <class U, class E>
  constexpr unique_ptr& operator=(unique_ptr<U[], E>&& r) noexcept {
    static_assert(std::is_assignable_v<deleter_type&, E&&>,
                  "unique_ptr deleter must be assignable from E&& for converting move assignment");
    using other_pointer = typename unique_ptr<U, E>::pointer;
    static_assert(std::is_convertible_v<other_pointer, pointer>,
                  "unique_ptr converting assignment requires convertible pointer");
    pointer new_ptr = static_cast<pointer>(r.p_);
    r.p_ = nullptr;
    reset(new_ptr);
    get_deleter() = std::forward<E>(r.d_);
    return *this;
  }

  constexpr unique_ptr& operator=(std::nullptr_t) noexcept {
    reset();
    return *this;
  }

  // 下标访问
  constexpr T& operator[](std::size_t i) const { return get()[i]; }

  // 其他成员同单对象版本
  constexpr pointer get() const noexcept { return p_; }

  constexpr deleter_type& get_deleter() noexcept { return d_; }
  constexpr deleter_type& get_deleter() const noexcept { return d_; }
  constexpr explicit operator bool() const noexcept { return get() != nullptr; }

  constexpr pointer release() noexcept {
    pointer old_ptr = get();
    p_ = nullptr;
    return old_ptr;
  }

  template <class U>
  constexpr void reset(U ptr) noexcept
    requires(std::is_same_v<U, pointer> ||
             (std::is_pointer_v<U> && std::is_convertible_v<std::remove_pointer_t<U> (*)[], element_type (*)[]>))
  {
    pointer new_ptr = [&] {
      if constexpr (std::is_same_v<U, pointer>) {
        return ptr;
      } else {
        return static_cast<pointer>(ptr);
      }
    }();
    pointer old = p_;
    if (old == new_ptr) {
      return;
    }
    p_ = new_ptr;
    if (old) {
      get_deleter()(old);
    }
  }

  constexpr void reset(std::nullptr_t = nullptr) noexcept { reset(pointer{}); }

  void swap(unique_ptr& other) noexcept {
    std::swap(p_, other.p_);
    std::swap(d_, other.d_);
  }

private:
  pointer p_ = pointer{};
  deleter_type d_;
};

// 非成员函数

template <class T1, class D1, class T2, class D2>
constexpr bool operator==(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y) {
  return x.get() == y.get();
}

template <class T1, class D1, class T2, class D2>
bool operator<(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y) {
  using CT =
      typename std::common_type<typename unique_ptr<T1, D1>::pointer, typename unique_ptr<T2, D2>::pointer>::type;
  return std::less<CT>()(x.get(), y.get());
}

template <class T1, class D1, class T2, class D2>
bool operator<=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y) {
  return !(y < x);
}

template <class T1, class D1, class T2, class D2>
bool operator>(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y) {
  return y < x;
}

template <class T1, class D1, class T2, class D2>
bool operator>=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y) {
  return !(x < y);
}

template <class T1, class D1, class T2, class D2>
  requires std::three_way_comparable_with<typename unique_ptr<T1, D1>::pointer, typename unique_ptr<T2, D2>::pointer>
std::compare_three_way_result_t<typename unique_ptr<T1, D1>::pointer, typename unique_ptr<T2, D2>::pointer> operator<=>(
    const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y) {
  return std::compare_three_way{}(x.get(), y.get());
}

template <class T, class D>
constexpr bool operator==(const unique_ptr<T, D>& x, std::nullptr_t) noexcept {
  return !x;
}

template <class T, class D>
constexpr bool operator<(const unique_ptr<T, D>& x, std::nullptr_t) noexcept {
  return std::less<typename unique_ptr<T, D>::pointer>()(x.get(), nullptr);
}
template <class T, class D>
constexpr bool operator<(std::nullptr_t, const unique_ptr<T, D>& y) noexcept {
  return std::less<typename unique_ptr<T, D>::pointer>()(nullptr, y.get());
}

template <class T, class D>
bool operator<=(const unique_ptr<T, D>& x, std::nullptr_t) {
  return !(nullptr < x);
}
template <class T, class D>
bool operator<=(std::nullptr_t, const unique_ptr<T, D>& y) {
  return !(y < nullptr);
}

template <class T, class D>
bool operator>(const unique_ptr<T, D>& x, std::nullptr_t) {
  return nullptr < x;
}
template <class T, class D>
bool operator>(std::nullptr_t, const unique_ptr<T, D>& y) {
  return y < nullptr;
}

template <class T, class D>
bool operator>=(const unique_ptr<T, D>& x, std::nullptr_t) {
  return !(nullptr < x);
}
template <class T, class D>
bool operator>=(std::nullptr_t, const unique_ptr<T, D>& y) {
  return !(y < nullptr);
}

template <class T, class D>
  requires std::three_way_comparable<typename unique_ptr<T, D>::pointer>
constexpr std::compare_three_way_result_t<typename unique_ptr<T, D>::pointer, std::nullptr_t> operator<=>(
    const unique_ptr<T, D>& x, std::nullptr_t) {
  return std::compare_three_way{}(x.get(), nullptr);
}

template <class CharT, class Traits, class Y, class D,
          class = std::enable_if_t<
              detail::is_streamable_v<std::basic_ostream<CharT, Traits>&, typename unique_ptr<Y, D>::pointer>>>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const unique_ptr<Y, D>& p) {
  os << p.get();
  return os;
}

template <class T, class D>
constexpr void swap(unique_ptr<T, D>& lhs, unique_ptr<T, D>& rhs) noexcept {
  lhs.swap(rhs);
}

template <class T, class... Args>
  requires(!std::is_array_v<T>)
constexpr unique_ptr<T> make_unique(Args&&... args) {
  return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <class T>
  requires(std::is_array_v<T> && std::extent_v<T> == 0)
constexpr unique_ptr<T> make_unique(std::size_t size) {
  using element = std::remove_extent_t<T>;
  return unique_ptr<T>(new element[size]());
}

template <class T, class... Args>
  requires(std::extent_v<T> != 0)
void make_unique(Args&&...) = delete;

template <class T>
  requires(!std::is_array_v<T>)
constexpr unique_ptr<T> make_unique_for_overwrite() {
  return unique_ptr<T>(new T);
}

template <class T>
  requires(std::is_array_v<T> && std::extent_v<T> == 0)
constexpr unique_ptr<T> make_unique_for_overwrite(std::size_t size) {
  using element = std::remove_extent_t<T>;
  return unique_ptr<T>(new element[size]);
}

template <class T, class... Args>
  requires(std::extent_v<T> != 0)
void make_unique_for_overwrite(Args&&...) = delete;

}  // namespace mystl

namespace std {

template <class T, class Deleter>
struct hash<mystl::unique_ptr<T, Deleter>>
    : std::enable_if_t<std::is_invocable_v<std::hash<typename mystl::unique_ptr<T, Deleter>::pointer>&,
                                           typename mystl::unique_ptr<T, Deleter>::pointer>,
                       std::hash<typename mystl::unique_ptr<T, Deleter>::pointer>> {
  using pointer = typename mystl::unique_ptr<T, Deleter>::pointer;
  using base = std::hash<pointer>;

  size_t operator()(const mystl::unique_ptr<T, Deleter>& p) const
      noexcept(noexcept(std::declval<const base&>()(p.get()))) {
    const base& hasher = *this;
    return hasher(p.get());
  }
};

}  // namespace std

#endif  // MYSTL_MEMORY_UNIQUE_PTR_HPP
