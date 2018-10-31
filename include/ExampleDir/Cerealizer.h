/**
 * @file
 */

#ifndef THIS_IS_AN_EXAMPLE_H
#define THIS_IS_AN_EXAMPLE_H

#include <jsoncpp/json/json.h>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "TcpListener.h"

/// Retrieves and stores the last chunk of an IPv4 address associated with an interface name.
class Cerealizer {
private:
  std::unique_ptr<tcpListener> listener;
  Json::Reader reader;
  Json::StyledWriter styledWriter;
  std::string cereal;
  std::mutex cerealMutex;
  std::condition_variable cerealReadiness;
  std::atomic<bool> isCerealReady;
  std::atomic<bool> isListening;
public:
  explicit Cerealizer(uint16_t port);
  NetworkReturnStatus listenToTcp();
  bool sendJson(Json::Value& data);
  std::string cerealizeJson(Json::Value& data);
  Json::Value decerealizeJson(const std::string& data);
  std::string getCereal();
  void setStatus(bool val) {isListening = val;}
};

#endif //THIS_IS_AN_EXAMPLE_H

