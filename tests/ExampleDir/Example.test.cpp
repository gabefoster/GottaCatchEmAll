//
// Created by gfoster on 9/21/18.
//

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Cerealizer.h"
#include <chrono>

TEST_CASE("This is an example of a test case.", "[unit]") {
  auto beginning = std::chrono::system_clock::now();
  REQUIRE(!Cerealizer::getLastIpChunk().empty());
  auto middle = std::chrono::system_clock::now();
  REQUIRE(!Cerealizer::getLastIpChunk().empty());
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> firstCallTime  = middle - beginning;
  std::chrono::duration<double> secondCallTime = end - middle;
  REQUIRE(firstCallTime.count() >= secondCallTime.count());
};