/**
 * @file
 */

#include "Cerealizer.h"
#include <iostream>
#include <future>
#include <unistd.h>

int main() {
  /* Client */
  Cerealizer cerealizer("127.0.0.1", 7001);
  Json::Value root;
  std::string foo;
  while(true) {
    std::cin >> foo;
    root["test"] = foo;
    cerealizer.sendJson(root);
    //std::cout << cerealizer.getCereal() << std::endl;
  }
  cerealizer.stopListening();
  /**/

  /* Server
  Cerealizer meow(7001);
  Json::Value root;
  std::string foo;
  while(true) {
    root["test"] = foo;
    std::cout << meow.getCereal() << std::endl;
    //std::cin >> foo;
    //meow.sendJson(root);
  }
  meow.stopListening();
  */
  //close(7001);
  return 0;
}

