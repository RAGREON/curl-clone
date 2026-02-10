#pragma once

#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

#include <ws2tcpip.h>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <unordered_map>
#include "request.h"
#include "url.h"

#define CALLER_LINE __LINE__

#define LOG_ERROR(info, line) \
  std::cout << "LINE: " << line << " | "; \
  std::cerr << info << " " << WSAGetLastError() << std::endl;

#define BUFFER_SIZE 1024

class Client {
private:
  WSADATA       wsaData;
  int           iResult;
  SOCKET        clientSocket;
  sockaddr_in   serverAddress;
  bool          connected;

  void inline CHECK_RC(std::string errorMessage, int line = CALLER_LINE) {
    if (iResult == SOCKET_ERROR) {
      LOG_ERROR(errorMessage, line);
      WSACleanup();
      exit(1);
    }
  }

  void parseResponse(std::string response) {
    int r_count = 0, n_count = 0;
    for (const char& c: response) {
      r_count += (c == '\r') ? 1 : 0;
      n_count += (c == '\n') ? 1 : 0;
    }
    std::cout << "r: " << r_count << "\n";
    std::cout << "n: " << n_count << std::endl;
  }

public:
  Client(int af_family, int sock_type, int protocol);

  void setServerAddress(std::string ip, int port);
  void setServerAddress(std::string url); 
  void openConnection();
  void sendRequest(Request::Type type, std::string route = "/"); 
  std::string recvResponse();
  SOCKET getSocket();
  void closeConnection();
 
  ~Client();
};