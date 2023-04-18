//
// Created by Bach Dao.
//

#ifndef HTTP_SERVER_EVENT_LOOP_H
#define HTTP_SERVER_EVENT_LOOP_H

#include "http_server/connection.h"
#include "http_server/executor.h"

namespace http_simple::detail {

class event_loop {
  static constexpr int MaxEvents = 128;
  static constexpr int INVALID_FD = -1;
  friend class connection;
  // server listening socket
  int serverFd_ = INVALID_FD;
  int receiveEpollFd_ = INVALID_FD;
  int sendEpollFd_ = INVALID_FD;
  executor* ptrExecutor_ = nullptr;
  server& server_;
  int init_receive_epoll();
  void monitor_receive_epoll();
  void accept_connection(int epollFd);
  void exit();
  int set_fd_flag(int fd, int flag);
  connection* make_connection(int fd);
  bool release_connection(connection* ptr);

public:
  event_loop(int serverSocketFd, executor* ptr, server& s);
  void run();
  bool monitor_send(connection* pConnection);
  int init_send_epoll();
  void monitor_send_epoll();
};
} // namespace http_simple::detail

#endif // HTTP_SERVER_EVENT_LOOP_H
