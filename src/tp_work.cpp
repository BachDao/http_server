//
// Created by Bach Dao.
//

#include "http_server/threadpool/tp_work.h"
#include <cassert>

namespace http_simple::detail {
void tp_work::operator()() {
  assert(fn_);
  fn_();
}

tp_work::operator bool() { return bool(fn_); }

tp_work& tp_work::operator=(const tp_work& rhs) {
  fn_ = rhs.fn_;
  return *this;
}

tp_work& tp_work::operator=(tp_work&& rhs) {
  fn_ = std::move(rhs.fn_);
  return *this;
}
} // namespace http_simple::detail