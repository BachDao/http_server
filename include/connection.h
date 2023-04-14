//
// Created by Bach Dao.
//

#ifndef HTTP_SERVER_CONNECTION_H
#define HTTP_SERVER_CONNECTION_H

#include "buffer.h"
#include "executor.h"
class connection {
  int fd_;
  executor *ptrExecutor_ = nullptr;
  buffer *ptrBuffer_ = nullptr;
  void handle_incoming_data();

public:
  connection(int fd, executor *ptrExe);
};

#endif // HTTP_SERVER_CONNECTION_H
