### Simple HTTP Server

<br/>
Simple implementation of http server that supports HTTP/1.1, with very limited features:

 + The maximum size for header is 8KB, a client will be rejected if a bigger request is sent.
 + Only GET method is supported, any other method leads to "404" response 
 + Response body is fixed (plaintext/utf8). Request handler is called for each request, but it results simply discard
 + The Connection is closed after the request is complete.

#### High-level implementation

- Sockets set with non-blocking mode.
- Single event-loop that waits for incoming tcp connection, accept and put client socket to an epoll monitoring list.
- There are two epoll instances, each for read and write side.
- Event-loop monitoring epoll for any socket that is ready for reading and writing, then queue the work to thread-pool
  to handle data.
- Each connection has a separate preallocate fixed-size buffer, data coming from the socket is combined there for later
  parsing.

#### Dependencies

- [GoogleTest](https://github.com/google/googletest) : For unit testing.
- [llhttp](https://github.com/nodejs/llhttp) for http parser.

#### Build

- Prerequisite: (*Dockerfile is provided for easy setup step.*)
    + GCC 12.2.1 (but any compiler that supports C++17 should be fine)
    + CMake 3.11
    + Ninja, pkgconfig...

### Benchmark

The Following benchmark is tested on Alpine container, with 4 v-CPU and 4Gb Ram. [Wrk](https://github.com/wg/wrk) is
used as a benchmarking tool

- The Current implementation is capable of handling 10k concurrent connections,
  with around 70k req/s

  ![alt text](https://github.com/BachDao/http_server/blob/main/resources/cpp_test.png)

- Benchmarking of nodejs-http module (with the same environment) is also provided for reference.

  Node-http module can handle 10k connection at speed around 37k req/s. Of-course Node is production server with many
  features supported, the number should be taken with grant of salt.

- ![alt text] (https://github.com/BachDao/http_server/blob/main/resources/nodejs_test.png)

- ![alt text](https://github.com/BachDao/http_server/blob/main/resources/nodejs_ref_code.png)
