#include "client.h"

int main(int argc, char** argv) {
  std::string url = argv[1];

  Client* _client = new Client();

  _client->setServerAddress(url);

  // _client->setServerAddress(ip, port);
  
  // _client->openConnection();

  _client->sendRequest(GET, "/");

  std::string response = _client->recvResponse();
  
  std::cout << "\n-----Response-----\n";
  std::cout << response;

  delete _client;

  return 0;
}