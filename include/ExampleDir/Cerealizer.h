/**
 * @file
 */

#ifndef THIS_IS_AN_EXAMPLE_H
#define THIS_IS_AN_EXAMPLE_H

#include <jsoncpp/json/json.h>
#include <thread>
#include "TcpListener.h"

class Cerealizer {
private:
  std::unique_ptr<tcpListener> listener;
  Json::Reader reader;
  Json::StyledWriter styledWriter;
  std::thread cerealize;
public:
  Cerealizer();
  explicit Cerealizer(uint16_t port);
  explicit Cerealizer(const std::string& addr);
  Cerealizer(const std::string& addr,  uint16_t port);
  ~Cerealizer();
  void stopListening();
  void sendJson(Json::Value& data);
  std::string cerealizeJson(Json::Value& data);
  Json::Value decerealizeJson(const std::string& data);
  std::string getCereal();
};

#endif //THIS_IS_AN_EXAMPLE_H

