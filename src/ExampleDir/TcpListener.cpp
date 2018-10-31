/**
 * @file
 */

#include <cstring>
#include <iostream>
#include <sys/time.h>
#include <sys/socket.h>
#include <unistd.h>
#include "TcpListener.h"
#include "NetworkServices.h"

tcpListener::tcpListener(uint16_t port) {
  cmd_sock = initSocket(port);
  if (cmd_sock < 0) {
    exit(1);
  }
  std::cerr << "Awaiting a connection on port " << port << "." << std::endl;
  connfd = accept(cmd_sock, (struct sockaddr*)NULL, NULL);
  std::cerr << "Accepted" << std::endl;
  if (connfd == -1) {
    exit(1);
  }
};

tcpListener::~tcpListener() {
  close(port);
}

NetworkReturnStatus tcpListener::listen(std::string& incomingBody) {
  PacketHeader incomingHeader;
  NetworkReturnStatus listenerStatus = recv_header(connfd, &incomingHeader);
  std::cerr << incomingHeader.bodySize << std::endl;
  if (listenerStatus == NetworkReturnStatus::BUFFER_FULL) {
    std::cerr << "Buffer is full and no Header was found" << std::endl;
    return NetworkReturnStatus::BUFFER_FULL;
  }
  if (listenerStatus == NetworkReturnStatus::COULD_NOT_RECEIVE_PACKET) {
    std::cerr << "Could not receive data packet" << std::endl;
    return NetworkReturnStatus::COULD_NOT_RECEIVE_PACKET;
  }
  if (listenerStatus == NetworkReturnStatus::CONNECTION_CLOSED) {
    std::cerr << "Connection was closed. Reopening..." << std::endl;
    // Close Socket
    close(connfd);
    // Block on receiving a socket connection
    connfd = accept(cmd_sock, (struct sockaddr*)NULL, NULL);
  }
  else {
    char cereal[incomingHeader.bodySize];
    receiveBody(cereal, incomingHeader.bodySize);
    incomingBody = cereal;
  }
  return NetworkReturnStatus::NO_ERROR;
}

bool tcpListener::receiveBody(void* const incomingBody, size_t incomingBodySize) {
  if (incomingBody == nullptr && incomingBodySize > 0) {
    throw std::bad_alloc();
  }
  if (incomingBodySize > 0) {
    size_t bodyBytesReceived = 0;
    while (bodyBytesReceived < incomingBodySize) {
      ssize_t bytesReceived = recv(connfd, reinterpret_cast<char*>(incomingBody) + bodyBytesReceived, incomingBodySize - bodyBytesReceived, MSG_NOSIGNAL);
      if (bytesReceived == 0) {
        std::cerr << "Connection was closed by the client before the body was received. Packet will be corrupted" << std::endl;
        return false;
      }
      else if (bytesReceived == -1) {
        std::cerr << "Network error in reception of the body. Body might be corrupted. Error code " << errno << "." << std::endl;
        return false;
      }

      bodyBytesReceived += bytesReceived;
    }
  }

  return true;
}

int tcpListener::tcpSendAll(void* packet, size_t packetSize) {
  size_t packetBytesSent = 0;
  ssize_t sentBytes = 0;
  while (packetBytesSent < packetSize) {
    sentBytes = send(connfd, reinterpret_cast<char*>(packet) + packetBytesSent, packetSize - packetBytesSent, MSG_NOSIGNAL);
    if (sentBytes == -1) {
      std::cerr << "Failed to send packet body with errno " << errno << "." << std::endl;
      return errno;
    }
    else {
      packetBytesSent += sentBytes;
    }
  }

  return 0;
}
