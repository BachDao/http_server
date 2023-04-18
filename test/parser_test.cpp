//
// Created by Bach Dao.
//
#include "http_server/parser/http_parser.h"
#include <gtest/gtest.h>
using namespace http_simple::detail;

TEST(ParserFixture, NormalRequest) {
  const char* request =
      "POST /cgi-bin/process.cgi HTTP/1.1\r\n"
      "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
      "Host: www.tutorialspoint.com\r\n"
      "Content-Type: application/x-www-form-urlencoded\r\n"
      "Content-Length: 50\r\n"
      "Accept-Language: en-us\r\n"
      "Accept-Encoding: gzip, deflate\r\n"
      "Connection: Keep-Alive\r\n"
      "\r\n"
      "licenseID=string&content=string&/paramsXML=string\r\n";
  http_parser parser;
  auto response =
      parser.parse({reinterpret_cast<std::byte*>(const_cast<char*>(request)),
                    strlen(request)});
  EXPECT_TRUE(response.has_value());
}
