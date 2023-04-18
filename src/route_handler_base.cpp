//
// Created by Bach Dao.
//

#include "http_server/router/route_handler_base.h"

namespace http_simple {

route_handler_base::route_handler_base(std::string baseUrl)
    : pattern_(baseUrl), regex_(baseUrl, std::regex_constants::ECMAScript |
                                             std::regex_constants::icase) {}

bool route_handler_base::match(std::string route) {
  return std::regex_match(route, regex_);
}

void route_handler_base::handle_request(http_request& req, http_response& res) {
  std::string method(req.method_);
  if (method == GetMethod) {
    return http_get(req, res);
  }
}

route_handler_base::~route_handler_base() {}
} // namespace http_simple