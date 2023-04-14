//
// Created by Bach Dao.
//

#include "server.h"
#include <cassert>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>

server::server(int port) : port_(port) {
  assert(port > 0 && "invalid port value");
  init_server();
}
void server::init_server() {

}
