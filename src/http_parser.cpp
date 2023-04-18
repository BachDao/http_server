//
// Created by Bach Dao.
//

#include "http_server/parser/http_parser.h"
#include <cstdlib>

namespace http_simple::detail {
void http_parser::init_parser() {
  llhttp_settings_init(&settings);

  settings.on_message_begin = on_message_begin;
  settings.on_message_complete = on_message_complete;

  settings.on_method = on_method;
  settings.on_url = on_url;

  settings.on_header_field = on_header_field;
  settings.on_header_value = on_header_value;

  settings.on_body = on_body;

  llhttp_init(&parser_, HTTP_BOTH, &settings);
  parser_.data = &state_;
}

std::optional<http_request> http_parser::parse(segment s) {
  auto err = llhttp_execute(&parser_, reinterpret_cast<const char*>(s.data()),
                            s.size());
  if (err == HPE_OK) {
    return state_.request_;
  }
  return std::nullopt;
}

void http_parser::reset() { llhttp_reset(&parser_); }

http_parser::http_parser() { init_parser(); }

int on_method(llhttp_t* parser, const char* at, size_t len) {
  parser_state& parserState = *static_cast<parser_state*>(parser->data);
  parserState.request_.method_ = {at, len};
  return 0;
}

int on_url(llhttp_t* parser, const char* at, size_t len) {
  parser_state& parserState = *static_cast<parser_state*>(parser->data);
  parserState.request_.url_ = {at, len};
  return 0;
}

int on_header_field(llhttp_t* parser, const char* at, size_t len) {
  parser_state& parserState = *static_cast<parser_state*>(parser->data);
  parserState.prevHeader_ = {at, len};
  return 0;
}

int on_header_value(llhttp_t* parser, const char* at, size_t len) {
  parser_state& parserState = *static_cast<parser_state*>(parser->data);
  parserState.request_.headers_[parserState.prevHeader_] = {at, len};
  return 0;
}

int on_body(llhttp_t* parser, const char* at, size_t len) {
  parser_state& parserState = *static_cast<parser_state*>(parser->data);
  parserState.request_.body_ = {at, len};
  return 0;
}

int on_message_complete(llhttp_t* parser) { return 0; }

int on_message_begin(llhttp_t* parser) { return 0; }
} // namespace http_simple::detail
