#!/bin/bash

g++ -std=c++20 -I"./include" lib/*.cpp src/main.cpp -o client.exe -lssl -lcrypto -lws2_32