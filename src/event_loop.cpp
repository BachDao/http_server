//
// Created by Bach Dao.
//

#include "http_server/event_loop.h"
#include "http_server/log.h"
#include <cassert>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <unistd.h>

namespace http_simple::detail {
event_loop::event_loop(int serverSocketFd, executor* ptr, server& s)
    : serverFd_(serverSocketFd), ptrExecutor_(ptr), server_(s) {
  receiveEpollFd_ = init_receive_epoll();
  sendEpollFd_ = init_send_epoll();
  if (receiveEpollFd_ == INVALID_FD || sendEpollFd_ == INVALID_FD)
    exit();
}

int event_loop::init_receive_epoll() {
  // Create epoll instance
  int receiveEpoll_ = epoll_create(1);
  if (receiveEpoll_ == INVALID_FD) {
    log_e("Unable to create receive epoll instance");
    return receiveEpoll_;
  }

  epoll_event event;
  event.data.ptr = make_connection(serverFd_);
  event.events = EPOLLIN | EPOLLET;

  // Add server accept socket to a monitoring list
  if (epoll_ctl(receiveEpoll_, EPOLL_CTL_ADD, serverFd_, &event) == -1) {
    log_e("Unable to add receive socket to epoll instance");
    return INVALID_FD;
  }
  return receiveEpoll_;
}

void event_loop::monitor_receive_epoll() {
  epoll_event events[MaxEvents];
  while (true) {
    // block indefinitely until events occur
    int nReady = epoll_wait(receiveEpollFd_, events, MaxEvents, -1);

    if (nReady == -1) {
      log_e("epoll_wait error: ");
      log_e(std::to_string(errno));
      return;
    }

    for (int i = 0; i < nReady; ++i) {
      assert(events[i].data.ptr && "invalid event state");
      auto* ptrConn = static_cast<connection*>(events[i].data.ptr);
      auto fd = ptrConn->fd_;

      if (fd == serverFd_) {
        accept_connection(receiveEpollFd_);
      } else {
        // handle incoming data
        bool success = ptrConn->handle_incoming_data();

        if (!success) {
          // client is close, or it sends something illegal, then we should
          // close connection
          release_connection(ptrConn);
        }
      }
    }
  }
}

void event_loop::monitor_send_epoll() {
  epoll_event events[MaxEvents];
  while (true) {
    // block indefinitely until events occur
    int nReady = epoll_wait(sendEpollFd_, events, MaxEvents, -1);

    if (nReady == -1) {
      log_e("epoll_wait error: ");
      log_e(std::to_string(errno));
      return;
    }

    for (int i = 0; i < nReady; ++i) {
      assert(events[i].data.ptr && "invalid event state");
      auto* ptrConn = static_cast<connection*>(events[i].data.ptr);

      // handle outgoing data
      bool success = ptrConn->handle_outgoing_data();

      if (!success) {
        // close connection
        release_connection(ptrConn);
      }
    }
  }
}

void event_loop::accept_connection(int epollFd) {
  sockaddr_in clientAddr;
  socklen_t clientAddrLen = sizeof(clientAddr);

  // setup epoll event use for all clients;
  epoll_event event;
  event.events = EPOLLIN | EPOLLET;

  // accept all waiting clients
  while (true) {
    auto clientFd =
        accept(serverFd_, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (clientFd == -1) {
      // no more connection
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        break;
      }
      // smt bad happen :P
      log_e("Unable to accept new connection");
      exit();
    }
    // put socket to non-blocking mode
    set_fd_flag(clientFd, O_NONBLOCK);

    // new connection object for this client
    event.data.ptr = make_connection(clientFd);

    // add new connection to epoll monitoring list
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientFd, &event) == -1) {
      log_e("Unable to add client socket to epoll instance");
      exit();
    }
  }
}

int event_loop::set_fd_flag(int fd, int flag) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1) {
    return -1;
  }
  flags |= flag;
  if (fcntl(fd, F_SETFL, flags) == -1) {
    return -1;
  }
  return 0;
}

void event_loop::run() { monitor_receive_epoll(); }

void event_loop::exit() { log_i("event loop exit"); }

connection* event_loop::make_connection(int fd) {
  return new connection(fd, ptrExecutor_, server_, this);
}

// TODO: cache connection for later client
bool event_loop::release_connection(connection* ptr) {
  auto ret = (ptr == nullptr);
  if (ret) {
    close(ptr->fd_);
    delete ptr;
  }
  return ret;
}

bool event_loop::monitor_send(connection* pConnection) {
  epoll_event event;
  event.data.ptr = pConnection;
  event.events = EPOLLOUT | EPOLLET;

  // Add server accept socket to a monitoring list
  if (epoll_ctl(sendEpollFd_, EPOLL_CTL_ADD, pConnection->fd_, &event) == -1) {
    log_e("Unable to add send socket to epoll instance");
    return false;
  }
  return true;
}

int event_loop::init_send_epoll() {
  int fd = epoll_create(1);
  if (fd == INVALID_FD) {
    log_e("Unable to create receive epoll instance");
  }
  return fd;
}
} // namespace http_simple::detail
