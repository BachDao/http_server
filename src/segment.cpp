//
// Created by Bach Dao.
//

#include "http_server/buffer/segment.h"

namespace http_simple::detail {
size_t segment::size() const { return len_; }

std::byte* segment::data() { return ptr_; }

segment::segment(std::byte* ptr, size_t len) : ptr_(ptr), len_(len) {}

segment::operator bool() const { return ptr_ != nullptr && len_ > 0; }
} // namespace http_simple::detail