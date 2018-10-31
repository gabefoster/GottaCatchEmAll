/**
 * @file
 */

#include "Cerealizer.h"
#include <iostream>
#include <future>

int main() {
  Cerealizer cerealizer(7000);
  std::string cereal;
  Json::Value root;
  auto cerealize = std::thread(&Cerealizer::listenToTcp, &cerealizer);
  while (true) {
    cereal = std::move(cerealizer.getCereal());
    root = cerealizer.decerealizeJson(cereal);
    std::cerr << "Test is: " << root["test"] << std::endl;
  }
  cerealizer.setStatus(false);
  cerealize.join();
  return 0;
}

