/**
 * @file
 */

#include "Cerealizer.h"

Cerealizer::Cerealizer(uint16_t port) {
  listener.reset(new tcpListener(port));
  isListening = false;
  isCerealReady = false;
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

NetworkReturnStatus Cerealizer::listenToTcp() {
  NetworkReturnStatus status = NetworkReturnStatus::NO_ERROR;
  isListening = true;
  while (isListening) {
    {
      status = listener->listen(cereal);
      if (status != NetworkReturnStatus::NO_ERROR) { break; }
      std::lock_guard<std::mutex> lk(cerealMutex);
      isCerealReady = true;
    }
    cerealReadiness.notify_all();
  }
  return status;
}

std::string Cerealizer::getCereal() {
  std::string retval;
  std::unique_lock<std::mutex> lk(cerealMutex);
  cerealReadiness.wait(lk, [&]{return isCerealReady.load();});
  retval = std::move(cereal);
  lk.unlock();
  cerealReadiness.notify_all();
  isCerealReady = false;
  return retval;
}
