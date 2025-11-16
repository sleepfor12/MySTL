/**
 * 为所有类似指针的类型（原生指针、智能指针、自定义指针等）
 * 提供统一的接口，使得泛型编程可以统一处理各种指针类型。
 *
 * ## 核心作用
 *
 * ### 1. 统一指针接口
 * pointer_traits 允许算法和容器不关心指针的具体类型，通过统一的接口操作：
 * - 原生指针：T*
 * - 智能指针：unique_ptr<T>、shared_ptr<T>
 * - 自定义指针：分配器返回的指针类型
 * - 其他类似指针的类型
 *
 * ### 2. 类型提取
 * 从指针类型中提取：
 * - element_type: 指针指向的元素类型
 * - difference_type: 两个指针之间的距离类型（通常是 ptrdiff_t）
 * - pointer: 指针类型本身
 * - rebind<U>: 将当前指针类型重新绑定到类型 U，生成新的指针类型
 *
 * ### 3. 指针操作
 * 提供静态函数：
 * - pointer_to(): 获取指向给定对象的指针（对于非原生指针类型）
 * - to_address(): 获取指针指向的原始对象的地址（对于非原生指针类型）
 * 不涉及运行时操作，无异常安全问题。
 * 1. 基础模板定义（提取自定义指针类型的成员）
 * 2. 原生指针特化（T*、const T*、volatile T* 等）
 * 3. rebind 机制实现
 * 4. pointer_to 静态函数实现
 * 5. to_address 静态函数实现
 */

#ifndef MYSTL_MEMORY_POINTER_TRAITS_HPP
#define MYSTL_MEMORY_POINTER_TRAITS_HPP

#include <cstddef>
#include <memory>
#include <type_traits>

namespace mystl {

#if __cplusplus >= 201703L
using std::void_t;
#else
template <class...>
using void_t = void;
#endif

template <class T, class = void>
struct has_element_type : std::false_type {};

template <class T>
struct has_element_type<T, void_t<typename T::element_type>> : std::true_type {
  using type = typename T::element_type;
};

template <class T, class = void>
struct has_difference_type : std::false_type {};

template <class T>
struct has_difference_type<T, void_t<typename T::difference_type>> : std::true_type {
  using type = typename T::difference_type;
};

template <class T, class U, class = void>
struct has_rebind : std::false_type {};

template <class T, class U>
struct has_rebind<T, U, void_t<typename T::template rebind<U>>> : std::true_type {
  using type = typename T::template rebind<U>;
};

template <class Ptr, class U, class = void>
struct rebind_from_template {
  // 默认情况：无法从模板特化推断，导致替换失败
  using type = void;
};

template <template <class> class Template, class T, class U>
struct rebind_from_template<Template<T>, U, void_t<Template<U>>> {
  using type = Template<U>;
};

template <template <class, class...> class Template, class T, class... Args, class U>
struct rebind_from_template<Template<T, Args...>, U, void_t<Template<U, Args...>>> {
  using type = Template<U, Args...>;
};

template <class T, class Elem, class = void>
struct has_pointer_to : std::false_type {};

template <class T, class Elem>
struct has_pointer_to<T, Elem,
                      void_t<decltype(T::pointer_to(std::declval<typename std::add_lvalue_reference<Elem>::type>()))>>
    : std::true_type {};

template <class T, class = void>
struct has_to_address : std::false_type {};

template <class T>
struct has_to_address<T, void_t<decltype(T::to_address(std::declval<T>()))>> : std::true_type {
  using return_type = decltype(T::to_address(std::declval<T>()));
};

/**
 * - 如果 Ptr 有 element_type 成员，使用它；否则需要特化
 * - 如果 Ptr 有 difference_type 成员，使用它；否则使用 ptrdiff_t
 * - 如果 Ptr 有 rebind<U> 成员模板，使用它；否则需要特化
 * - 如果 Ptr 有 pointer_to 静态成员函数，使用它；否则需要特化处理
 */
template <class Ptr>
struct pointer_traits {
  // pointer: 直接等于模板参数本身
  using pointer = Ptr;

  // element_type: 从 Ptr::element_type 提取
  // 根据 LWG 3545，如果 Ptr 没有 element_type，应该是 SFINAE 友好的
  // 如果 has_element_type 为 false，使用依赖 false 的类型导致替换失败
  using element_type = typename std::conditional_t<
      has_element_type<Ptr>::value, has_element_type<Ptr>,
      std::enable_if<has_element_type<Ptr>::value, void>  // 依赖 false，导致替换失败（SFINAE 友好）
      >::type;

  // difference_type: 从 Ptr::difference_type 提取，如果不存在则使用 ptrdiff_t
  using difference_type =
      std::conditional_t<has_difference_type<Ptr>::value, typename has_difference_type<Ptr>::type, std::ptrdiff_t>;

  // 1. 如果存在 Ptr::rebind<U>，则为其
  // 2. 否则，如果 Ptr 是模板特化 Template<T, Args...>，则为 Template<U, Args...>
  template <class U>
  using rebind =
      typename std::conditional_t<has_rebind<Ptr, U>::value, has_rebind<Ptr, U>, rebind_from_template<Ptr, U>>::type;

  // pointer_to: 获取指向对象的指针
  // 如果 Ptr 有 pointer_to 静态成员函数，调用它；否则需要特化处理
  static typename std::enable_if_t<has_pointer_to<Ptr, element_type>::value, pointer> pointer_to(
      typename std::add_lvalue_reference<element_type>::type r) noexcept {
    return Ptr::pointer_to(r);
  }

  // 如果 Ptr 有 to_address 静态成员函数，调用它
  // 否则，对于原生指针，直接返回；对于智能指针，通过 operator-> 获取
  template <class P = Ptr>
  static constexpr typename std::enable_if_t<has_to_address<P>::value, typename has_to_address<P>::return_type>
  to_address(const P& p) noexcept {
    return P::to_address(p);
  }

  // 默认实现：对于原生指针或可通过 operator-> 获取的类型
  template <class P = Ptr>
  static constexpr typename std::enable_if_t<!has_to_address<P>::value && std::is_pointer_v<P>, P> to_address(
      const P& p) noexcept {
    return p;
  }

  // 对于智能指针等，通过 std::to_address 获取原始指针
  template <class P = Ptr>
  static constexpr typename std::enable_if_t<!has_to_address<P>::value && !std::is_pointer_v<P>,
                                             decltype(std::to_address(std::declval<const P&>()))>
  to_address(const P& p) noexcept {
    return std::to_address(p);
  }
};

template <class T>
struct pointer_traits<T*> {
  using pointer = T*;
  using element_type = T;
  using difference_type = std::ptrdiff_t;

  template <class U>
  using rebind = U*;

  static constexpr pointer pointer_to(typename std::add_lvalue_reference<T>::type r) noexcept {
    return std::addressof(r);
  }

  static constexpr pointer to_address(pointer p) noexcept { return p; }
};

// const 指针特化（const T*）
template <class T>
struct pointer_traits<const T*> {
  using pointer = const T*;
  using element_type = T;  // 注意：element_type 不是 const
  using difference_type = std::ptrdiff_t;

  template <class U>
  using rebind = const U*;

  static constexpr pointer pointer_to(typename std::add_lvalue_reference<const T>::type r) noexcept {
    return std::addressof(r);
  }

  static constexpr pointer to_address(pointer p) noexcept { return p; }
};

template <class T>
struct pointer_traits<volatile T*> {
  using pointer = volatile T*;
  using element_type = T;
  using difference_type = std::ptrdiff_t;

  template <class U>
  using rebind = volatile U*;

  static constexpr pointer pointer_to(typename std::add_lvalue_reference<volatile T>::type r) noexcept {
    return std::addressof(r);
  }

  static constexpr pointer to_address(pointer p) noexcept { return p; }
};

template <class T>
struct pointer_traits<const volatile T*> {
  using pointer = const volatile T*;
  using element_type = T;
  using difference_type = std::ptrdiff_t;

  template <class U>
  using rebind = const volatile U*;

  static constexpr pointer pointer_to(typename std::add_lvalue_reference<const volatile T>::type r) noexcept {
    return std::addressof(r);
  }

  static constexpr pointer to_address(pointer p) noexcept { return p; }
};

}  // namespace mystl

#endif  // MYSTL_MEMORY_POINTER_TRAITS_HPP
