//
// Created by Bach Dao.
//

#ifndef HTTP_SERVER_UTIL_H
#define HTTP_SERVER_UTIL_H
#include <functional>
#include <type_traits>

// Traits
namespace http_simple::detail {
template <class T> struct remove_cvref {
  using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

template <typename Fn> void repeat(size_t times, Fn&& fn) {
  static_assert(std::is_invocable_v<Fn>);
  while (times--) {
    std::invoke(fn);
  }
}

} // namespace http_simple::detail

#endif // HTTP_SERVER_UTIL_H
