//
// Created by Bach Dao.
//

#ifndef HTTP_SERVER_BUFFER_H
#define HTTP_SERVER_BUFFER_H
#include "http_server/buffer/segment.h"

namespace http_simple::detail {
template <size_t Size> class buffer {
  std::byte storage_[Size];
  size_t readOffset_;
  size_t writeOffset_;

public:
  segment allocate(size_t size);
  void advance(size_t size);

  segment data();
  void consume(size_t size);

  void reset();
  size_t size() const;
  size_t capacity() const;
};

template <size_t Size> size_t buffer<Size>::capacity() const {
  return Size - writeOffset_;
}

template <size_t Size> size_t buffer<Size>::size() const {
  return writeOffset_ - readOffset_;
}

template <size_t Size> void buffer<Size>::reset() {
  writeOffset_ = readOffset_ = 0;
}

template <size_t Size> void buffer<Size>::consume(size_t size) {
  // call-site should be sure about the validity of "size"
  readOffset_ += size;
}

template <size_t Size> segment buffer<Size>::data() {
  return {storage_ + readOffset_, writeOffset_ - readOffset_};
}

template <size_t Size> void buffer<Size>::advance(size_t size) {
  // call-site should be sure about the validity of "size"
  writeOffset_ += size;
}

template <size_t Size> segment buffer<Size>::allocate(size_t size) {
  if (size + writeOffset_ <= Size)
    return {storage_ + writeOffset_, size};
  return {};
}

} // namespace http_simple::detail

#endif // HTTP_SERVER_BUFFER_H
