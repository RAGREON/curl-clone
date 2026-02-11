#include "ssl.h"
#include <client.h>

#include <iostream>
#include <sstream>

#define LOG_ERROR(info, line) \
  std::cout << "LINE: " << line << " | "; \
  std::cerr << info << " " << WSAGetLastError() << std::endl;

/*
  PRIVATE 
*/

void inline Client::CHECK_RC(std::string errorMessage, int line) {
  if (iResult == SOCKET_ERROR) {
    LOG_ERROR(errorMessage, line);
    WSACleanup();
    exit(1);
  }
}

/*
  PUBLIC
*/

Client::Client(int af_family, int sock_type, int protocol) {
  /*
    Configuring socket type:
      IPv4 TCP  
  */
  clientSocket = socket(af_family, sock_type, protocol);

  CHECK_RC("Socket creation failed.");

  serverAddress.sin_family = AF_INET;

  addressList = nullptr;
}

void Client::setHostAddress(std::string url) {
  addrinfo hints {};
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  auto _meta = Url::parseUrl(url);

  iResult = getaddrinfo(_meta.host.c_str(), std::to_string(_meta.port).c_str(), &hints, &addressList);

  if (addressList == nullptr) {
    std::cerr << "Invalid url" << std::endl;
    exit(1);
  }

  CHECK_RC("Failed to resolve url.");
}

void Client::openConnection() {
  for (addrinfo* ptr = addressList; ptr != nullptr; ptr = ptr->ai_next) {
    if ((iResult = connect(clientSocket, ptr->ai_addr, (int)ptr->ai_addrlen)) == 0) break;
  }

  serverAddress = *reinterpret_cast<sockaddr_in*>(addressList->ai_addr);

  if (serverAddress.sin_port == htons(443)) {
    std::cout << "https port\n";
  }

  CHECK_RC("Failed to form connection to server.");

  connected = true;
  std::cout << "Successfully connected to server." << std::endl;
} 

void Client::sendRequest(Request::Type type, std::string route) {
  std::string _request = Request::constructRequest(serverAddress, type, route);

  if (serverAddress.sin_port != htons(443)) {
    iResult = Request::sendRequest(clientSocket, _request);
    CHECK_RC("Failed to send request.");
    return;
  }

  SSLClient* _client = new SSLClient(clientSocket);

  _client->sslConnect();
  _client->sendRequest(_request);
  _client->recvResponse();
}

std::string Client::recvResponse() {
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

void Client::closeConnection() {
  if (!connected) return;

  iResult = closesocket(clientSocket);

  CHECK_RC("Failed to close socket.");
}

Client::~Client() {
  closeConnection();
}