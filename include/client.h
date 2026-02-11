#pragma once

#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

#include <ws2tcpip.h>
#include <string>
#include <iostream>
#include <sstream>
#include <url.h>
#include <request.h>

#define CALLER_LINE __LINE__

#define BUFFER_SIZE 1024

class Client {
private:
  WSADATA       wsaData;
  int           iResult;
  SOCKET        clientSocket;
  addrinfo*     addressList;
  sockaddr_in   serverAddress;
  bool          connected;

  void inline CHECK_RC(std::string errorMessage, int line = CALLER_LINE);

public:
  Client(int af_family, int sock_type, int protocol);

  void setServerAddress(std::string url); 
  void openConnection();
  void sendRequest(Request::Type type, std::string route = "/"); 
  std::string recvResponse();
  SOCKET getSocket();
  sockaddr_in getAddress() const { return serverAddress; }
  void closeConnection();
 
  ~Client();
};