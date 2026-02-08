#pragma once

#pragma comment(lib, "Ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <unordered_map>

#define LOG_ERROR(info) \
  std::cout << "LINE: " << __LINE__ << " | "; \
  std::cerr << info << " " << WSAGetLastError() << std::endl;

#define BUFFER_SIZE 1024

enum RequestType {
  GET, POST, DEL, PATCH, PUT
};

std::unordered_map<RequestType, std::string> requestMap = {
  { GET,    "GET" },
  { POST,   "POST" },
  { DEL,    "DELETE" },
  { PATCH,  "PATCH"},
  { PUT,    "PUT" }
};

struct UrlMeta {
  int             port;
  std::string     scheme;
  std::string     host;
  std::string     path;

  UrlMeta() : port(80), scheme("http") { }
};

class Client {
private:
  WSADATA       wsaData;
  int           iResult;
  SOCKET        clientSocket;
  sockaddr_in   serverAddress;
  bool          connected;

  void inline CHECK_RC(std::string errorMessage) {
    if (iResult == SOCKET_ERROR) {
      LOG_ERROR(errorMessage);
      WSACleanup();
      exit(1);
    }
  }

  std::string constructRequest(RequestType type, std::string route) {
    std::stringstream request, header, host, body;

    header << 
      requestMap[type] << " " << 
      route << " " <<
      "HTTP/1.1\r\n";
   
    host <<
      "Host: " << inet_ntoa(serverAddress.sin_addr) << ":" << ntohs(serverAddress.sin_port) << "\r\n" <<
      "Connection: Close\r\n" <<
      "\r\n";

    request << header.str() << host.str();

    return request.str();
  }

public:
  Client() {
    /*
      WSA Startup 
    */
    wsaData = { };
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    CHECK_RC("WSA startup failed.");

    /*
      Configuring socket type:
        IPv4 TCP  
    */
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    CHECK_RC("Socket creation failed.");

    serverAddress.sin_family = AF_INET;
  }

  void setServerAddress(std::string ip, int port) {
    serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());
    serverAddress.sin_port = htons(port);
  }

  void openConnection() {
    iResult = connect(clientSocket, (SOCKADDR*) &serverAddress, sizeof(serverAddress));

    CHECK_RC("Failed to form connection to server.");

    connected = true;
    std::cout << "Successfully connected to server." << std::endl;
  } 

  void sendRequest(RequestType type, std::string route = "/") {
    std::string request = constructRequest(type, route);

    std::cout << "\n-----Request-----\n";
    std::cout << request << "\n";

    iResult = send(clientSocket, request.c_str(), (int)request.length(), 0);

    CHECK_RC("Failed to send request.");

    std::cout << "Request sent to server." << std::endl;
  }

  std::string recvResponse() {
    std::stringstream response;

    char buffer[BUFFER_SIZE] = { };

    do {
      iResult = recv(clientSocket, buffer, BUFFER_SIZE, 0);

      if (iResult > 0) 
        response << buffer;
      else if (iResult == 0)
        connected = false;
      else 
        CHECK_RC("Failed to receive buffer from server.");
    } while (iResult > 0);

    return response.str();
  }

  void closeConnection() {
    if (!connected) return;

    iResult = closesocket(clientSocket);

    CHECK_RC("Failed to close socket.");
  }

  ~Client() {
    WSACleanup();
    closeConnection();
  }
};