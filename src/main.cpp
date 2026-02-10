#include <client.h>
#include <manager.h>
#include <ssl.h>
#include <winsock.h>

int main(int argc, char** argv) {
  Manager manager;
  manager.initialize();

  Client* socket_client = new Client(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  SSLClient* ssl_client = new SSLClient(); 

  socket_client->setServerAddress("142.250.76.68", 443);
  socket_client->openConnection();
  
  SOCKET fd = socket_client->getSocket();

  ssl_client->createContext();
  ssl_client->createSSL(fd);
  ssl_client->sslConnect();

  std::string request = "GET / HTTP/1.1\r\nHost: 142.250.76.68:443\r\nConnection: close\r\n\r\n";
  ssl_client->sendRequest(request);
  ssl_client->recvResponse();

  delete ssl_client;

  return 0;
}