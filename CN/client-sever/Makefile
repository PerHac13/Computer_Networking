# Makefile for TCP Chat Application

CXX = g++
CXXFLAGS = -std=c++17 -Wall -pthread

all: server client

server: server.cpp
	$(CXX) $(CXXFLAGS) -o server server.cpp

client: client.cpp
	$(CXX) $(CXXFLAGS) -o client client.cpp

clean:
	rm -f server client