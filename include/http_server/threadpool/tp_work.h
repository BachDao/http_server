//
// Created by Bach Dao.
//

#ifndef HTTP_SERVER_TP_WORK_H
#define HTTP_SERVER_TP_WORK_H
#include "http_server/util.h"
#include <functional>

namespace http_simple::detail {
class tp_work {
  std::function<void()> fn_;

public:
  template <typename Fn> tp_work(Fn&& fn);
  tp_work() = default;
  tp_work(const tp_work&) = default;
  tp_work(tp_work&&) = default;
  tp_work& operator=(const tp_work& rhs);
  tp_work& operator=(tp_work&& rhs);

  void operator()();
  explicit operator bool();
};

template <typename Fn> tp_work::tp_work(Fn&& fn) : fn_(std::forward<Fn>(fn)) {
  static_assert(std::is_invocable_v<Fn>);
  static_assert(!std::is_same_v<remove_cvref<Fn>, tp_work>);
}
} // namespace http_simple::detail
#endif // HTTP_SERVER_TP_WORK_H
