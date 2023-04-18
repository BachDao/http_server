//
// Created by Bach Dao.
//

#ifndef HTTP_SERVER_TP_WORKER_H
#define HTTP_SERVER_TP_WORKER_H

#include "http_server/forward_declarations.h"
#include "http_server/threadpool/tp_work.h"
#include <deque>
#include <thread>

namespace http_simple::detail {
class tp_worker {
  std::deque<tp_work> works_;
  std::thread thread_;

public:
  explicit tp_worker(thread_pool* threadPool);
  tp_worker(tp_worker&&) noexcept = delete;
  tp_worker(const tp_worker&) = delete;

  ~tp_worker();
  void submit(tp_work&& work);
  void do_work(thread_pool* tp);
};
} // namespace http_simple::detail

#endif // HTTP_SERVER_TP_WORKER_H
