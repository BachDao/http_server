//
// Created by Bach Dao.
//

#include "http_server/router/router.h"

namespace http_simple::detail {

route_handler_base* router::get_route_handler(std::string route) {
  for (auto& h : routeHandlers_) {
    if (h->match(route)) {
      return h.get();
    }
  }
  return nullptr;
}
} // namespace http_simple::detail
