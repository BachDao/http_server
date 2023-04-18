//
// Created by Bach Dao.
//

#ifndef HTTP_SERVER_LOG_H
#define HTTP_SERVER_LOG_H

#include <iostream>

namespace http_simple {
enum class log_level { info, debug, error };

static std::string to_string(log_level level) {
  switch (level) {
  case log_level::info:
    return "info: ";
  case log_level::debug:
    return "debug: ";
  case log_level::error:
    return "error: ";
  default:
    throw std::logic_error("invalid log level");
  }
}

static void log(log_level level, std::string msg) {
  std::cout << to_string(level) << msg << '\n';
}

static void log_d(std::string msg) {
#ifdef NDEBUG
  log(log_level::debug, msg);
#endif
}

static void log_e(std::string msg) { log(log_level::error, msg); }

static void log_i(std::string msg) { log(log_level::info, msg); }
} // namespace http_simple
#endif // HTTP_SERVER_LOG_H
