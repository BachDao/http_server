//
// Created by Bach Dao.
//

#include "http_server/server.h"
using namespace http_simple;

class test_route : public http_simple::route_handler_base {
public:
  explicit test_route(std::string path)
      : http_simple::route_handler_base(path) {}

  void http_get(http_simple::http_request& req,
                http_simple::http_response& res) override {}

  void http_post(http_simple::http_request& req,
                 http_simple::http_response& res) override {}

  void http_put(http_simple::http_request& req,
                http_simple::http_response& res) override {}

  void http_delete(http_simple::http_request& req,
                   http_simple::http_response& res) override {}
};

using namespace http_simple;

int main() {
  server s{8888};
  s.router_.register_route<test_route>("/hello_world");
  s.start();
}