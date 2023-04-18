//
// Created by Bach Dao.
//

#ifndef HTTP_SERVER_SERVER_H
#define HTTP_SERVER_SERVER_H
#include "forward_declarations.h"
#include "http_server/event_loop.h"
#include "http_server/router/router.h"
#include <cassert>

namespace http_simple {

class server {
public:
  static constexpr int INVALID_FD = -1;
  const size_t THREAD_POOL_SIZE = std::thread::hardware_concurrency();
  friend class detail::connection;
  int port_;
  int serverFd_ = INVALID_FD;
  std::unique_ptr<detail::event_loop> ptrEventLoop_ = nullptr;
  std::unique_ptr<detail::executor> ptrExecutor_ = nullptr;
  detail::router router_;
  bool init_server();
  int create_listening_socket();
  void routing_request(http_request& req, http_response& res);
  std::string curPath_;

public:
  explicit server(int port);
  void start();
  server& with(const std::string& path);

  template <typename RouteHandler> void route_to();
};

template <typename RouteHandler> void server::route_to() {
  assert(curPath_.size() > 0 && "Specify path first");
  router_.register_route<RouteHandler>(curPath_);
}
} // namespace http_simple
#endif // HTTP_SERVER_SERVER_H
