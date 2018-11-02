/**
 * @file
 */

#include "Cerealizer.h"
#include <iostream>
#include <future>
#include <unistd.h>

void runClient(const std::string& address, uint16_t port) {
  Cerealizer cerealizer("127.0.0.1", 7001);
  Json::Value root;
  std::string foo;
  while (true) {
    std::cin >> foo;
    root["test"] = foo;
    cerealizer.sendJson(root);
    std::cerr << cerealizer.getCereal() << std::endl;
  }

  cerealizer.stopListening();
}

void runServer(uint16_t port) {
  Cerealizer cerealizer(7001);
  Json::Value root;
  std::string foo;
  std::cerr << cerealizer.getCereal() << std::endl;
  while(true) {
    std::cin >> foo;
    root["test"] = foo;
    cerealizer.sendJson(root);
    std::cerr << cerealizer.getCereal() << std::endl;
  }
  cerealizer.stopListening();
}

int main() {
  //runServer(7001);
  //runClient("127.0.0.1", 7001);
  //close(7001);
  return 0;
}

