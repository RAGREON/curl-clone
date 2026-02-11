#pragma once

#include <iostream>
#include <string>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>

class SSLClient {
private:
  const SSL_METHOD*   method;
  SOCKET              socketFd;
  SSL_CTX*            ctx;
  SSL*                ssl;

public:
  SSLClient(SOCKET fd) {
    method = TLS_client_method();

    ctx = SSL_CTX_new(method);
    if (!ctx) 
      exit(1);
    
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, fd);
  }

  void sslConnect() {
    if (SSL_connect(ssl) <= 0) {
      exit(1);
    }
  }

  void sendRequest(std::string request) {
    if (SSL_write(ssl, request.c_str(), int(request.length())) <= 0) {
      std::cout << "error sending request";
    }
  }

  void recvResponse() {
    char buffer[1024];
    int bytes;

    do {
      bytes = SSL_read(ssl, buffer, sizeof(buffer) - 1);
      if (bytes > 0) {
        buffer[bytes] = '\0';
        std::cout << buffer;
      }
      else if (bytes < 0) {
        std::cout << "error";
      }
    } while (bytes > 0);
  }

  void closeConnection(SSL* ssl) {
    SSL_shutdown(ssl);
    SSL_free(ssl);
  }

  ~SSLClient() {
    SSL_CTX_free(ctx);
    EVP_cleanup();
  }
};