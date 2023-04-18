//
// Created by Bach Dao.
//

#ifndef HTTP_SERVER_EXECUTOR_H
#define HTTP_SERVER_EXECUTOR_H
#include "http_server/threadpool/threadpool.h"
#include "http_server/util.h"
#include <functional>

namespace http_simple::detail {
class executor {
  thread_pool threadPool_;
  void submit_impl(tp_work&& w);

public:
  executor(size_t workers = std::thread::hardware_concurrency());
  template <typename Fn, typename = std::enable_if_t<std::is_invocable_v<Fn>>>
  void submit(Fn&& fn);
};

template <typename Fn, typename> void executor::submit(Fn&& fn) {
  submit_impl(std::forward<Fn>(fn));
}
} // namespace http_simple::detail
#endif // HTTP_SERVER_EXECUTOR_H
