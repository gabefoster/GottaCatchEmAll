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
  while (true) {
    cereal = std::move(cerealizer.getCereal());
    root = cerealizer.decerealizeJson(cereal);
    std::cerr << "Test is: " << root.get("test", "woof").asString() << std::endl;
  }
  cerealizer.stopListening();
  return 0;
}

