/**
 * @file
 */

#include "Example.h"
#include <arpa/inet.h>
#include <vector>
#include <sys/types.h>
#include <ifaddrs.h>
#include <cstring>
#include <string>

std::once_flag NetworkProperties::once;
std::string NetworkProperties::ipAddrChunk;
constexpr char NetworkProperties::interfaceName[];

const std::string& NetworkProperties::getLastIpChunk() {
  auto& tmp = interfaceName;
  std::call_once(once, [](){
    struct ifaddrs *ifaddr_p, *ifaddr;
    struct sockaddr_in* sock;
    char*  addr;
    bool   isCorrectInterface{false};
    size_t pos{};
    std::vector<std::string> tokenizedString;
    std::string delimiter(".");
    getifaddrs(&ifaddr_p);
    // Iterate through the interfaces
    for (ifaddr = ifaddr_p; ifaddr; ifaddr = ifaddr->ifa_next) {
      tokenizedString.clear();
      // Check for AF_INET Family Addresses
      if (ifaddr->ifa_addr && ifaddr->ifa_addr->sa_family == AF_INET) {
        sock = reinterpret_cast<struct sockaddr_in*>(ifaddr->ifa_addr);
        addr = inet_ntoa(sock->sin_addr);
        ipAddrChunk = std::string(addr);
        // Chunk the IP addr
        while ((pos = ipAddrChunk.find(delimiter)) != std::string::npos) {
          tokenizedString.emplace_back(ipAddrChunk.substr(0, pos));
          ipAddrChunk.erase(0, pos + delimiter.length());
        }
        tokenizedString.emplace_back(ipAddrChunk);
        // We're looking for this specific interface name
        if(ifaddr->ifa_name && (strcmp(interfaceName, ifaddr->ifa_name) == 0)) {
          isCorrectInterface = true;
        }
        if (tokenizedString.size() == 4 && isCorrectInterface) {
          break;
        }
      }
    }
    freeifaddrs(ifaddr_p);
    if (!isCorrectInterface) {
      throw(std::runtime_error("No interface named eth0 could be found!"));
    }
  });
  return ipAddrChunk;
}
