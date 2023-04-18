//
// Created by Bach Dao.
//
#include "http_server/router/router.h"
#include <gtest/gtest.h>

using namespace http_simple::detail;

bool getFnCalled = false;

class simple_route : public ::http_simple::route_handler_base {
public:
  simple_route(std::string baseUrl) : route_handler_base(baseUrl) {}

  void http_get(http_simple::http_request& req,
                http_simple::http_response& res) override {}

  void http_post(http_simple::http_request& req,
                 http_simple::http_response& res) override {}

  void http_put(http_simple::http_request& req,
                http_simple::http_response& res) override {}

  void http_delete(http_simple::http_request& req,
                   http_simple::http_response& res) override {}
};

template <typename T, typename U> bool is_instance_of(U* ptr) {
  auto r = dynamic_cast<T*>(ptr);
  return r != nullptr;
}

TEST(Router, SimpleRoute) {
  router router;
  router.register_route<simple_route>("/simple_route");

  auto handler = router.get_route_handler("/simple_route");
  EXPECT_TRUE(handler);
  EXPECT_TRUE(is_instance_of<simple_route>(handler));
}