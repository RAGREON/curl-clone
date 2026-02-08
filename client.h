#pragma once

#pragma comment(lib, "Ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <stdlib.h>

#define LOG_ERROR(info) \
  std::cout << "LINE: " << __LINE__ << " | "; \
  std::cerr << info << " " << WSAGetLastError() << std::endl;

#define BUFFER_SIZE 1024

class Client {
private:
  WSADATA       wsaData;
  int           iResult;
  SOCKET        clientSocket;
  sockaddr_in   serverAddress;

  void inline CHECK_RC(std::string errorMessage) {
    if (iResult == SOCKET_ERROR) {
      LOG_ERROR(errorMessage);
      WSACleanup();
      exit(1);
    }
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

    std::cout << "Successfully connected to server." << std::endl;
  } 

  void sendRequest(std::string request) {
    iResult = send(clientSocket, request.c_str(), (int)request.length(), 0);

    CHECK_RC("Failed to send request.");

    std::cout << "Request sent to server." << std::endl;
  }

  void recvResponse() {
    char buffer[BUFFER_SIZE] = { };

    std::cout << "\n-----Response-----\n";

    do {
      iResult = recv(clientSocket, buffer, BUFFER_SIZE, 0);

      if (iResult > 0) 
        std::cout << buffer;
      else if (iResult == 0)
        std::cout << "Server close connection." << std::endl;
      else 
        CHECK_RC("Failed to receive buffer from server.");
    } while (iResult > 0);
  }

  void closeConnection() {
    iResult = closesocket(clientSocket);

    CHECK_RC("Failed to close socket.");
  }

  ~Client() {
    WSACleanup();
    closeConnection();
  }
};