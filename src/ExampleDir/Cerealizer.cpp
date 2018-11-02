/**
 * @file
 */

#include <cstring>
#include <iostream>
#include "Cerealizer.h"

Cerealizer::Cerealizer() {
  listener.reset(new tcpListener());
  receiveThread = std::thread(&tcpListener::listen, listener.get());
  sendThread    = std::thread(&tcpListener::speak,  listener.get());
}

Cerealizer::Cerealizer(uint16_t port) {
  listener.reset(new tcpListener(port));
  receiveThread = std::thread(&tcpListener::listen, listener.get());
  sendThread    = std::thread(&tcpListener::speak,  listener.get());
}

Cerealizer::Cerealizer(const std::string& addr) {
  listener.reset(new tcpListener(addr, DEFAULT_TCP_PORT));
  receiveThread = std::thread(&tcpListener::listen, listener.get());
  sendThread    = std::thread(&tcpListener::speak,  listener.get());
}

Cerealizer::Cerealizer(const std::string& addr, uint16_t port) {
  listener.reset(new tcpListener(addr, port));
  receiveThread = std::thread(&tcpListener::listen, listener.get());
  sendThread    = std::thread(&tcpListener::speak,  listener.get());
}

Cerealizer::~Cerealizer() {
  stopListening();
}

void Cerealizer::stopListening() {
  listener->stopCommunicating();
  if (sendThread.joinable()) {
    sendThread.join();
  }
  if (receiveThread.joinable()) {
    receiveThread.join();
  }
}

std::string Cerealizer::getCereal() {
  return std::move(listener->passPacketUp());
}

std::string Cerealizer::cerealizeJson(Json::Value& data) {
  return styledWriter.write(data);
}

Json::Value Cerealizer::decerealizeJson(const std::string& data) {
  Json::Value retval;
  reader.parse(data, retval);
  return retval;
}

void Cerealizer::sendJson(Json::Value& data) {
  listener->passPacketDown(std::move(cerealizeJson(data)));
}




