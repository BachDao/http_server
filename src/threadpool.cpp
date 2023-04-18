//
// Created by Bach Dao.
//

#include "http_server/threadpool/threadpool.h"
#include "http_server/threadpool/thread_helper.h"

namespace http_simple::detail {

tp_work thread_pool::get_tp_work() {
  std::unique_lock lck{mutex_};
  if (globalWorkQueue_.empty()) {
    cv_.wait(lck,
             [this] { return !globalWorkQueue_.empty() || stopRequested_; });
  }

  if (stopRequested_)
    return {};

  auto w = std::move(globalWorkQueue_.back());
  globalWorkQueue_.pop_back();
  return w;
}

void thread_pool::submit(tp_work&& work) {
  bool empty = false;

  // since this code block run in worker thread, we don't need to acquire the
  // lock
  if (workerState.valid()) {
    workerState.ptrCurrentWorker->submit(std::move(work));
  }

  {
    std::lock_guard lck{mutex_};
    empty = globalWorkQueue_.empty();
    globalWorkQueue_.emplace_front(std::move(work));
  }
  if (empty)
    cv_.notify_one();
}

thread_pool::~thread_pool() {
  std::lock_guard lck{mutex_};
  stopRequested_ = true;
  cv_.notify_all();
}

thread_pool::thread_pool(size_t size) {
  if (size == 0) {
    size = std::thread::hardware_concurrency();
  }

  // reserve storage to make sure we don't need call to
  // move constructor of tp_worker when vector resize
  workers_.reserve(size);
  repeat(size,
         [this] { workers_.emplace_back(std::make_unique<tp_worker>(this)); });
}
} // namespace http_simple::detail