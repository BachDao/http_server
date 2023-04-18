//
// Created by Bach Dao.
//

#include "http_server/connection.h"
#include "http_server/executor.h"
#include "http_server/log.h"
#include "http_server/router/http.h"
#include "http_server/server.h"
#include <cassert>
#include <sys/socket.h>
#include <unistd.h>

namespace http_simple::detail {

connection::connection(int fd, executor* ptrExe, server& s,
                       event_loop* ptrEventLoop)
    : fd_(fd), ptrExe_(ptrExe), server_(s), ptrEventLoop_(ptrEventLoop) {}

bool connection::handle_incoming_data() {
  ptrExe_->submit([this] {
    receive_data();
    if (buffer_.size() == 0) {
      close_connection();
    }

    std::optional<http_request> req = parse_http_request();
    if (!req) {
      std::cout << "buffer_ size: " << buffer_.size() << std::endl;
    }

    if (req) {
      routing_request(req.value());
    }
  });
  return true;
}

void connection::receive_data() {
  // read as much as we can
  while (true) {
    if (buffer_.capacity() == 0) {
      // our received buffer is full, we should try to parse request
      // to know if we should continue with this connection
      break;
    }
    auto seg = buffer_.allocate(128);
    if (!seg) {
      // seem we have request with header exceed max size
      log_e("can't allocate buffer");
      close_connection();
    }
    int bytesReceived = recv(fd_, seg.data(), seg.size(), 0);

    if (bytesReceived == -1)
      if (errno = EAGAIN || errno == EWOULDBLOCK)
        break;
      else {
        close_connection();
      }
    if (bytesReceived == 0) {
      break;
    }
    buffer_.advance(bytesReceived);
  }
}

std::optional<http_request> connection::parse_http_request() {
  return parser_.parse(buffer_.data());
}

void connection::close_connection() { ptrEventLoop_->release_connection(this); }

void connection::routing_request(http_request& req) {
  http_response res;
  server_.routing_request(req, res);
  if (res.data_.size() > 0) {
    send_data(res.data_);
  }
  reset_connection_state();
}

void connection::send_data(const std::string& res) {
  response_ = res;
  auto bytesSent = sync_send();

  // most of times this test will success
  if (bytesSent == response_.size()) {
    response_.clear();
    close_connection();
    return;
  }

  response_ = response_.substr(bytesSent);
  // fall back to an async path
  setup_async_notify();
}

bool connection::handle_outgoing_data() {
  ptrExe_->submit([this] {
    auto bytesSent = sync_send();
    response_ = response_.substr(bytesSent);
    if (response_.size() == 0) {
      ptrEventLoop_->release_connection(this);
    }
  });
  return true;
}

void connection::setup_async_notify() {
  if (!ptrEventLoop_->monitor_send(this))
    close_connection();
}

size_t connection::sync_send() {
  size_t bytesSend = 0;
  while (true) {
    if (bytesSend == response_.size())
      return bytesSend;

    int size = send(fd_, response_.data(), response_.size(), 0);

    if (size < -1) {
      if (errno == EWOULDBLOCK | errno == EAGAIN) {
        return bytesSend;
      }

      // client closed connection
      // we're done here
      close_connection();
      return bytesSend;
    }
    if (size > 0)
      bytesSend += size;
  }
}

void connection::reset_connection_state() { buffer_.reset(); }
} // namespace http_simple::detail
