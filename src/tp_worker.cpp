//
// Created by Bach Dao.
//

#include "http_server/threadpool/tp_worker.h"
#include "http_server/log.h"
#include "http_server/threadpool/thread_helper.h"
#include "http_server/threadpool/threadpool.h"

namespace http_simple::detail {
thread_local worker_state workerState;

tp_worker::tp_worker(thread_pool* threadPool) {
  thread_ = std::thread(
      [=](thread_pool* tp) {
        workerState.ptrCurrentWorker = this;
        do_work(tp);
      },
      threadPool);
}

void tp_worker::submit(tp_work&& work) { works_.push_back(std::move(work)); }

void tp_worker::do_work(thread_pool* tp) {
  while (!tp->stopRequested_) {
    // drain local queue
    while (!works_.empty()) {
      auto&& w = works_.front();
      w();
      works_.pop_front();
    }
    auto w = tp->get_tp_work();
    if (w)
      w();
  }
}

tp_worker::~tp_worker() {
  if (thread_.joinable())
    thread_.join();
}
} // namespace http_simple::detail