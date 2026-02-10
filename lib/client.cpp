#include <client.h>

Client::Client(int af_family, int sock_type, int protocol) {
  /*
    Configuring socket type:
      IPv4 TCP  
  */
  clientSocket = socket(af_family, sock_type, protocol);

  CHECK_RC("Socket creation failed.");

  serverAddress.sin_family = AF_INET;
}

void Client::setServerAddress(std::string ip, int port) {
  serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());
  serverAddress.sin_port = htons(port);
}

void Client::setServerAddress(std::string url) {
  addrinfo hints {};
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  addrinfo* result = nullptr;

  auto _meta = Url::parseUrl(url);

  iResult = getaddrinfo(_meta.host.c_str(), std::to_string(_meta.port).c_str(), &hints, &result);

  for (addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
    if (connect(clientSocket, ptr->ai_addr, (int)ptr->ai_addrlen) == 0) break;
  }

  serverAddress = *reinterpret_cast<sockaddr_in*>(result->ai_addr);
  
  std::cout << "ip:" << inet_ntoa(serverAddress.sin_addr) << "\n";
  std::cout << "port:" << ntohs(serverAddress.sin_port) << std::endl;

  CHECK_RC("Failed to resolve url.");
}

void Client::openConnection() {
  iResult = connect(clientSocket, (SOCKADDR*) &serverAddress, sizeof(serverAddress));

  CHECK_RC("Failed to form connection to server.");

  connected = true;
  std::cout << "Successfully connected to server." << std::endl;
} 

void Client::sendRequest(Request::Type type, std::string route) {
  std::string _request = Request::constructRequest(serverAddress, type, route);

  iResult = Request::sendRequest(clientSocket, _request);

  CHECK_RC("Failed to send request.");
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

SOCKET Client::getSocket() {
  return clientSocket;
}

void Client::closeConnection() {
  if (!connected) return;

  iResult = closesocket(clientSocket);

  CHECK_RC("Failed to close socket.");
}

Client::~Client() {
  WSACleanup();
  closeConnection();
}