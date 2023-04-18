//
// Created by Bach Dao.
//

#ifndef HTTP_SERVER_THREAD_HELPER_H
#define HTTP_SERVER_THREAD_HELPER_H
#include "http_server/forward_declarations.h"
#include <atomic>
#include <iostream>
#include <thread>
#include <type_traits>

namespace http_simple::detail {
struct worker_state {
  bool valid() { return ptrCurrentWorker != nullptr; }

  tp_worker* ptrCurrentWorker = nullptr;
};

extern thread_local worker_state workerState;
} // namespace http_simple::detail
#endif // HTTP_SERVER_THREAD_HELPER_H
