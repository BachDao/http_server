//
// Created by Bach Dao.
//

#ifndef HTTP_SERVER_ROUTER_H
#define HTTP_SERVER_ROUTER_H
#include "http_server/router/route_handler_base.h"
#include <memory>
#include <vector>

namespace http_simple::detail {
class router {
  std::vector<std::unique_ptr<::http_simple::route_handler_base>>
      routeHandlers_;

public:
  template <typename RouteHandler> void register_route(std::string pattern) {
    static_assert(
        std::is_base_of_v<::http_simple::route_handler_base, RouteHandler>,
        "route_handler must be derived from route_handler_base class");
    auto ptr = std::make_unique<RouteHandler>(pattern);
    routeHandlers_.push_back(std::move(ptr));
  }

  route_handler_base* get_route_handler(std::string route);

}; // namespace http_simple::detail
} // namespace http_simple::detail

#endif // HTTP_SERVER_ROUTER_H
