#pragma once

#include <unordered_map>
#include <string>
#include <sstream>
#include <winsock2.h>

namespace Request {
  enum Type {
    GET, POST, DEL, PATCH, PUT
  };

  static std::unordered_map<Type, std::string> requestMap = {
    { GET,    "GET" },
    { POST,   "POST" },
    { DEL,    "DELETE" },
    { PATCH,  "PATCH"},
    { PUT,    "PUT" }
  };

  std::string constructRequest(sockaddr_in& address, Type type, std::string route);
  int sendRequest(SOCKET& socket, std::string request);
};
