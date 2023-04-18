//
// Created by Bach Dao.
//

#ifndef HTTP_SERVER_ROUTE_HANDLER_BASE_H
#define HTTP_SERVER_ROUTE_HANDLER_BASE_H
#include "http_server/router/http.h"
#include <regex>
#include <string>

namespace http_simple {
class route_handler_base {
  std::string pattern_;
  std::regex regex_;
  std::string GetMethod = "GET";

public:
  explicit route_handler_base(std::string baseUrl);
  virtual ~route_handler_base();
  bool match(std::string url);
  void handle_request(http_request& req, http_response& res);
  virtual void http_get(http_request& req, http_response& res) = 0;
  virtual void http_post(http_request& req, http_response& res) = 0;
  virtual void http_put(http_request& req, http_response& res) = 0;
  virtual void http_delete(http_request& req, http_response& res) = 0;
};
} // namespace http_simple

#endif // HTTP_SERVER_ROUTE_HANDLER_BASE_H
