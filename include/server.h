//
// Created by Bach Dao.
//

#ifndef HTTP_SERVER_SERVER_H
#define HTTP_SERVER_SERVER_H

class server {
  int port_;
  int serverFd_;

  void init_server();

public:
  explicit server(int port);
};

#endif // HTTP_SERVER_SERVER_H
