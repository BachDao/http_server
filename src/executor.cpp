//
// Created by Bach Dao.
//

#include "http_server/executor.h"
#include <cassert>

namespace http_simple::detail {

executor::executor(size_t workers) : threadPool_(workers) {}

void executor::submit_impl(tp_work&& w) { threadPool_.submit(std::move(w)); }
} // namespace http_simple::detail