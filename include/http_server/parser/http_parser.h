//
// Created by Bach Dao.
//

#ifndef HTTP_SERVER_HTTP_PARSER_H
#define HTTP_SERVER_HTTP_PARSER_H
#include "http_server/router/http.h"
#include <llhttp.h>
#include <optional>

namespace http_simple::detail {
struct parser_state {
  http_request request_;
  std::string_view prevHeader_;
};

int on_message_begin(llhttp_t* parser);

int on_message_complete(llhttp_t* parser);

int on_header_value_complete(llhttp_t* parser);

int on_version_complete(llhttp_t* parser);

int on_header_field(llhttp_t* parser, const char* at, size_t len);

int on_header_value(llhttp_t* parser, const char* at, size_t len);

int on_url(llhttp_t* parser, const char* at, size_t len);

int on_method(llhttp_t* parser, const char* at, size_t len);

int on_body(llhttp_t* parser, const char* at, size_t len);

struct http_parser {
  llhttp_t parser_;
  llhttp_settings_t settings;
  parser_state state_;
  http_parser();
  void reset();
  std::optional<http_request> parse(segment s);

private:
  void init_parser();
};
} // namespace http_simple::detail

#endif // HTTP_SERVER_HTTP_PARSER_H
