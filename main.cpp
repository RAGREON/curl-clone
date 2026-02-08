#include "client.h"

int main(int argc, char** argv) {
  std::string ip = argv[1];
  int port = std::stoi(argv[2]);

  Client* _client = new Client();

  _client->setServerAddress(ip, port);
  
  _client->openConnection();

  std::string request =
    "GET / HTTP/1.1\r\n"
    "Host: 127.0.0.1:8070\r\n"
    "Connection: close\r\n\r\n";
  _client->sendRequest(request);

  _client->recvResponse();

  return 0;
}