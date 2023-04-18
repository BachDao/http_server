//
// Created by Bach Dao.
//

#include <arpa/inet.h>
#include <cassert>
#include <cstring>
#include <fcntl.h>
#include <memory>
#include <sys/epoll.h>
#include <unistd.h>

#include "http_server/connection.h"
#include "http_server/log.h"
#include "http_server/server.h"

// TODO : add fileEvent to stop waiting on epoll

namespace http_simple {

server::server(int port) : port_(port) {
  assert(port > 0 && "invalid port value");
  init_server();
}

bool server::init_server() {
  serverFd_ = create_listening_socket();
  if (serverFd_ == INVALID_FD) {
    return false;
  }
  return true;
}

int server::create_listening_socket() {
  int fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  if (fd == INVALID_FD) {
    log_e("Unable to create server socket");
    return INVALID_FD;
  }

  int reuse = 1;
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
    log_e("Unable to set socket options");
    return INVALID_FD;
  }

  sockaddr_in server_addr{};
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port_);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
    log_e("Unable to bind socket");
    return INVALID_FD;
  }

  if (listen(fd, SOMAXCONN) == -1) {
    log_e("Unable to listen for connections");
    return INVALID_FD;
  }
  return fd;
}

void server::start() {
  ptrExecutor_ = std::make_unique<detail::executor>(THREAD_POOL_SIZE);
  ptrEventLoop_ = std::make_unique<detail::event_loop>(
      serverFd_, ptrExecutor_.get(), *this);
  ptrEventLoop_->run();
}

void server::routing_request(http_request& req, http_response& res) {
  auto handler = router_.get_route_handler(std::string(req.url_));
  if (handler) {
    handler->handle_request(req, res);
    res.send_hello();
  } else
    res.send_404();
}

server& server::with(const std::string& path) {
  curPath_ = path;
  return *this;
}

} // namespace http_simple