//
// Created by Bach Dao.
//

#ifndef HTTP_SERVER_BUFFER_H
#define HTTP_SERVER_BUFFER_H
#include <cstddef>
struct block {
  void *ptr;
  size_t size;
};

class buffer {

public:
  block alloc(size_t size);
  void dealloc(block blk);
};

#endif // HTTP_SERVER_BUFFER_H
