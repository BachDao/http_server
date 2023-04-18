//
// Created by Bach Dao.
//

#ifndef HTTP_SERVER_CONNECTION_H
#define HTTP_SERVER_CONNECTION_H

#include "executor.h"
#include "forward_declarations.h"
#include "http_server/buffer/buffer.h"
#include "http_server/parser/http_parser.h"
#include "http_server/router/http.h"
#include <optional>

namespace http_simple::detail {
enum class error_code {

};

/* Each client is denoted by a connection object that utilizes an 8Kb buffer,
 * which is pre-allocated and fixed for the header, a client that sends bigger
 * headers size will be rejected. This buffer size is generally sufficient for
 * most use cases.
 *
 * By default, the body also uses an 8Kb buffer, but in cases where the client
 * sends a larger body (such as in Multipart cases), it is the responsibility of
 * the middleware function to manage the memory (writing the data to a
 * file or providing some type of "stream" to the HTTP handler or do other
 * meaningful).
 */
class connection {
  static constexpr size_t BufferSize = 16 * 1024;
  friend class event_loop;
  executor* ptrExe_ = nullptr;
  event_loop* ptrEventLoop_ = nullptr;
  buffer<BufferSize> buffer_;
  server& server_;
  std::string response_;

  int fd_ = -1;
  http_parser parser_;
  void receive_data();
  void close_connection();
  std::optional<http_request> parse_http_request();

public:
  explicit connection(int fd, executor* ptrExe, server& s,
                      event_loop* ptrEventLoop);
  void on_data_receive(segment);
  bool handle_incoming_data();
  bool handle_outgoing_data();
  void routing_request(http_request& req);
  void send_data(const std::string& res);
  void setup_async_notify();
  size_t sync_send();
  void reset_connection_state();
};
} // namespace http_simple::detail
#endif // HTTP_SERVER_CONNECTION_H
