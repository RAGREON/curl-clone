#pragma once

#include <winsock.h>

#include <openssl/ssl.h>
#include <iostream>

class Manager {
private:
  void initializeWSA() {
    /*
      WSA Initialization
    */

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
      std::cerr << "WSAStartup failed" << std::endl;
      exit(1);
    }
  }  

  void initializeSSL() {
    /*
      OpenSSL Initialization 
    */

    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
  }

public:
  Manager() {} 

  void initialize() {
    initializeWSA();
    initializeSSL();
  }
};