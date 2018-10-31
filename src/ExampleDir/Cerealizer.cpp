/**
 * @file
 */

#include "Cerealizer.h"

Cerealizer::Cerealizer(uint16_t port) {
  listener.reset(new tcpListener(port));
  cerealize = std::thread(&tcpListener::listen, listener.get());
}

Cerealizer::~Cerealizer() {
  stopListening();
}

void Cerealizer::stopListening() {
  listener->stopListening();
  if (cerealize.joinable()) {
    cerealize.join();
  }
}

std::string Cerealizer::getCereal() {
  return std::move(listener->getPacket());
}

std::string Cerealizer::cerealizeJson(Json::Value& data) {
  return styledWriter.write(data);
}

Json::Value Cerealizer::decerealizeJson(const std::string& data) {
  Json::Value retval;
  reader.parse(data, retval);
  return retval;
}

bool Cerealizer::sendJson(Json::Value& data) {
  return true;
}




