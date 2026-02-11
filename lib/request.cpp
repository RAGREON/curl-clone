#include <request.h>

#include <sstream>

namespace Request {
  std::string constructRequest(sockaddr_in& address, Type type, std::string route) {
    std::stringstream request, header, host, body;

    header << 
      requestMap[type] << " " << 
      route << " " <<
      "HTTP/1.1\r\n";
    
    host <<
      "Host: " << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port) << "\r\n" <<
      "Connection: Close\r\n" <<
      "\r\n";

    request << header.str() << host.str();

    return request.str();
  }

  int sendRequest(SOCKET& socket, std::string request) {
    return send(socket, request.c_str(), (int)request.length(), 0);
  }
}
