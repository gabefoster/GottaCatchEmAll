/**
 * @file
 */

#include <cstring>
#include <iostream>
#include "Cerealizer.h"

Cerealizer::Cerealizer() {
  listener.reset(new tcpListener());
  cerealize = std::thread(&tcpListener::listen, listener.get());
}

Cerealizer::Cerealizer(uint16_t port) {
  listener.reset(new tcpListener(port));
  cerealize = std::thread(&tcpListener::listen, listener.get());
}

Cerealizer::Cerealizer(const std::string& addr) {
  listener.reset(new tcpListener(addr, DEFAULT_TCP_PORT));
  cerealize = std::thread(&tcpListener::speak, listener.get());
}

Cerealizer::Cerealizer(const std::string& addr, uint16_t port) {
  listener.reset(new tcpListener(addr, port));
  cerealize = std::thread(&tcpListener::speak, listener.get());
}

Cerealizer::~Cerealizer() {
  stopListening();
}

void Cerealizer::stopListening() {
  listener->stopCommunicating();
  if (cerealize.joinable()) {
    cerealize.join();
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




