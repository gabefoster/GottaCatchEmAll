/**
 * @file
 */

#include "Example.h"
#include <iostream>
#include <chrono>

int main() {
  auto beginning = std::chrono::system_clock::now();
  std::cout << NetworkProperties::getLastIpChunk() << std::endl;
  auto middle = std::chrono::system_clock::now();
  std::cout << NetworkProperties::getLastIpChunk() << std::endl;
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> firstCallTime  = beginning - middle;
  std::chrono::duration<double> secondCallTime = middle - end;
  std::cout << "Elapsed time for first call: "  << firstCallTime.count()  << std::endl;
  std::cout << "Elapsed time for second call: " << secondCallTime.count() << std::endl;
  return 0;
}
