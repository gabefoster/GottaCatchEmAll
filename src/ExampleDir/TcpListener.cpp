/**
 * @file
 */

#include "TcpListener.h"
#include "NetworkServices.h"
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

tcpListener::tcpListener() {
  isConnected = false;
  commandSocket = acceptFromTcp(DEFAULT_TCP_PORT);
  tcpFileDescriptor = accept(commandSocket, (struct sockaddr*)NULL, NULL);
  isConnected = true;
  if (tcpFileDescriptor == -1) {
    throw(std::runtime_error("Failed to accept TCP socket."));
  }
  std::cerr << "Awaiting a connection on port " << port << "." << std::endl;
}

tcpListener::tcpListener(uint16_t port) {
  isConnected = false;
  commandSocket = acceptFromTcp(port);
  std::cerr << "Awaiting a connection on port " << port << "." << std::endl;
  tcpFileDescriptor = accept(commandSocket, (struct sockaddr*)NULL, NULL);
  isConnected = true;
  if (tcpFileDescriptor == -1) {
    throw(std::runtime_error("Failed to accept TCP socket."));
  }
}

tcpListener::tcpListener(const std::string& addr) {
  isConnected = false;
  commandSocket = connectToTcp(addr, DEFAULT_TCP_PORT);
  isConnected = true;
  if (commandSocket < 0) {
    throw(std::runtime_error("Failed to initialize TCP socket."));
  }
}

tcpListener::tcpListener(const std::string& addr, uint16_t port) {
  isConnected = false;
  tcpFileDescriptor = connectToTcp(addr, port);
  isConnected = true;
  if (tcpFileDescriptor < 0) {
    throw(std::runtime_error("Failed to initialize TCP socket."));
  }
};

tcpListener::~tcpListener() {
  if (isConnected) {
    close(port);
  }
}

NetworkReturnStatus tcpListener::hear(std::string& incomingBody) {
  PacketHeader incomingHeader;
  NetworkReturnStatus listenerStatus = recv_header(tcpFileDescriptor, &incomingHeader);
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
    close(tcpFileDescriptor);
    // Block on receiving a socket connection
    tcpFileDescriptor = accept(commandSocket, (struct sockaddr*)NULL, NULL);
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
      ssize_t bytesReceived = recv(tcpFileDescriptor, reinterpret_cast<char*>(incomingBody) + bodyBytesReceived, incomingBodySize - bodyBytesReceived, MSG_NOSIGNAL);
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
    sentBytes = send(tcpFileDescriptor, reinterpret_cast<char*>(packet) + packetBytesSent, packetSize - packetBytesSent, MSG_NOSIGNAL);
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

NetworkReturnStatus tcpListener::listen() {
  NetworkReturnStatus status = NetworkReturnStatus::NO_ERROR;
  isCommunicating = true;
  while (isCommunicating) {
    {
      status = hear(packet);
      if (status != NetworkReturnStatus::NO_ERROR) { break; }
      std::lock_guard<std::mutex> lk(packetMutex);
      isPacketReady = true;
    }
    packetReadiness.notify_all();
  }
  return status;
}

void tcpListener::passPacketDown(std::string outgoingPacket) {
  {
    packet = std::move(outgoingPacket);
    std::lock_guard<std::mutex> lk(packetMutex);
    isPacketReady = true;
  }
  packetReadiness.notify_all();
}

std::string tcpListener::passPacketUp() {
  std::string retval;
  std::unique_lock<std::mutex> lk(packetMutex);
  packetReadiness.wait(lk, [&]{return isPacketReady.load();});
  retval = std::move(packet);
  lk.unlock();
  packetReadiness.notify_all();
  isPacketReady = false;
  return retval;
}

bool tcpListener::speak() {
  bool status = true;
  size_t packetSize = 0;
  isCommunicating = true;
  while (isCommunicating) {
    PacketHeader packetHeader;
    std::unique_lock<std::mutex> lk(packetMutex);
    packetReadiness.wait(lk, [&]{return isPacketReady.load();});
    packetSize = packet.size() + 1;
    packetHeader.bodySize = packetSize;
    char packetC[packetSize];
    strncpy(packetC, packet.c_str(), packetSize);
    packet.clear();
    lk.unlock();
    packetReadiness.notify_all();
    isPacketReady = false;
    status = tcpSendAll(&packetHeader, sizeof(PacketHeader)) == 0;
    if (!status) { break; }
    status = tcpSendAll(packetC, packetSize) == 0;
    //status = tcpSendAll(packetC, packetSize) == 0;
    if (!status) { break; }
  }

  return status;
}

void tcpListener::stopCommunicating() {
  isCommunicating = false;
  isCommunicating = false;
}