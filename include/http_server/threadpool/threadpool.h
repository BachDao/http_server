//
// Created by Bach Dao.
//

#ifndef HTTP_SERVER_THREAD_POOL_H
#define HTTP_SERVER_THREAD_POOL_H

#include "http_server/threadpool/tp_worker.h"
#include <atomic>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>

/**

Implementation of simple thread pool:
- Each worker has its own private task queue. Tasks submitted by the worker
itself go into this queue.
- Any new tasks submitted to the thread pool from outside are added to a shared
global queue.
- Each worker first attempts to complete tasks in its private queue before
checking the global queue for more work.
- If there are no tasks available, the worker will wait or sleep until a new
task is submitted.

There are a few things to keep in mind:
- Current implementation don't have any mechanism to re-balance tasks between
workers.
- The shared global queue is protected by a lock, which could become a
bottleneck if most tasks are submitted from outside the thread pool.
- The number of worker threads is fixed, regardless of the number of tasks and
relationship between them. This could lead to deadlocks if all running tasks are
blocked by each other.

Depending on the system load, these factors should be considered for further
improvement.
*/

namespace http_simple::detail {
class thread_pool {
  friend class tp_worker;
  std::vector<std::unique_ptr<tp_worker>> workers_;
  std::mutex mutex_;
  std::condition_variable cv_;
  std::deque<tp_work> globalWorkQueue_;
  std::atomic<bool> stopRequested_ = false;
  tp_work get_tp_work();

public:
  explicit thread_pool(size_t size = 0);
  ~thread_pool();
  void submit(tp_work&& work);
};
} // namespace http_simple::detail

#endif // HTTP_SERVER_THREAD_POOL_H
