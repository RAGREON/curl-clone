#pragma comment(lib, "Ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <stdlib.h>

#define LOG_ERROR(info) \
  std::cout << "LINE: " << __LINE__ << " | "; \
  std::cerr << info << " " << WSAGetLastError() << std::endl;

#define BUFFER_SIZE 1024

int main(int argc, char** argv) {

  std::cout << "Hello World!!" << std::endl;

  /*
    WSA Startup 
  */
  WSADATA wsaData = { };
  int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    LOG_ERROR("WSA Startup failed");
    return 1;
  }

  /*
    Socket that will connect to the servers
    Client Socket

    Address Family = INET / IPv4
    Type = TCP connection
  */
  SOCKET connectSocket;
  connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (connectSocket == INVALID_SOCKET) {
    LOG_ERROR("Connect Socket creation failed");
  }
  
  /*
    Server socket address 
  */
  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
  serverAddress.sin_port = htons(std::stoi(argv[2]));

  iResult = connect(connectSocket, (SOCKADDR*) &serverAddress, sizeof(serverAddress));
  if (iResult == SOCKET_ERROR) {
    LOG_ERROR("Connect failed");
    iResult = closesocket(connectSocket);

    if (iResult == SOCKET_ERROR)
      LOG_ERROR("Failed to close socket");

    WSACleanup();
    return 1;
  }
  
  std::cout << "successfully connected to server!!" << std::endl;

  if (iResult == SOCKET_ERROR) {
    LOG_ERROR("Failed to close socket");
    WSACleanup();
    return 1;
  }

  std::string request = "GET / HTTP/1.1\r\nHost: 127.0.0.1:5500\r\n\r\n"; 

  iResult = send(connectSocket, request.c_str(), (int)request.length(), 0);
  if (iResult == SOCKET_ERROR) {
    LOG_ERROR("Failed to send request");
    WSACleanup();
    return 1;
  }

  char recvBuf[BUFFER_SIZE];

  do {
    iResult = recv(connectSocket, recvBuf, BUFFER_SIZE, 0);
    if (iResult > 0) {
      std::cout << recvBuf;
    }
    else if (iResult == 0) {
      std::cout << "Server closed connection\n";
    }
    else {
      LOG_ERROR("Receive failed");
    }
  } while (iResult > 0);

  closesocket(connectSocket);

  WSACleanup();
  return 0;
}