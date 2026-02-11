#include <client.h>
#include <manager.h>

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "url not provided\n";
    return 1;
  }
  
  std::string url = argv[1];

  Manager manager;
  manager.initialize();

  Client socket_client = Client(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  socket_client.setServerAddress(url);
  socket_client.openConnection();
  
  SOCKET fd = socket_client.getSocket();
  sockaddr_in address = socket_client.getAddress();

  // SSLClient ssl_client = SSLClient(fd); 
  // ssl_client.sslConnect();

  // std::string _request = Request::constructRequest(address, Request::GET, "/");

  // ssl_client.sendRequest(_request);
  // ssl_client.recvResponse();


  return 0;
}