//
// Created by Bach Dao.
//

#ifndef HTTP_SERVER_HTTP_H
#define HTTP_SERVER_HTTP_H
#include "http_server/buffer/segment.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace http_simple {

enum class http_method { GET, POST };

struct http_request {
  std::string_view method_;
  std::string_view url_;
  std::string_view body_;
  std::unordered_map<std::string_view, std::string_view> headers_;

  http_request() : headers_() { headers_.reserve(50); }
};

struct http_response {
  std::string data_;

  void send_hello() {
    data_ = "HTTP/1.1 200 OK\r\n"
            "Content-Length: 12\r\n"
            "Content-Type: text/plain; charset=utf-8\r\n"
            "\r\n"
            "Hello World!";
  }

  void send_404() {
    data_ = "HTTP/1.1 404 \r\n"
            "Content-Length: 0\r\n"
            "\r\n";
  }
};
} // namespace http_simple
#endif // HTTP_SERVER_HTTP_H
