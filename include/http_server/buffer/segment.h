//
// Created by Bach Dao.
//

#ifndef HTTP_SERVER_SEGMENT_H
#define HTTP_SERVER_SEGMENT_H

#include <cstddef>

namespace http_simple::detail {
class segment {
  std::byte* ptr_ = nullptr;
  size_t len_ = 0;

public:
  segment() = default;
  segment(std::byte* ptr, size_t len);
  size_t size() const;
  std::byte* data();
  explicit operator bool() const;
};
} // namespace http_simple::detail

#endif // HTTP_SERVER_SEGMENT_H
