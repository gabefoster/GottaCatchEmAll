/**
 * @file
 */

#ifndef THIS_IS_AN_EXAMPLE_H
#define THIS_IS_AN_EXAMPLE_H

#include <mutex>

/*
 * @class
 * @brief Retrieves and stores the last chunk of an IPv4 address associated with an interface name.
 */
class NetworkProperties {
  private:
    static std::once_flag once;
    static std::string ipAddrChunk;
  public:
    static constexpr char interfaceName[] = "wlp2s0";
    /**
     * @brief Retrieves the last chunk of the IPv4 address associated with the interfaceName above.
     * @return Last chunk of IP address with interface name above.
     */
    static const std::string& getLastIpChunk();
};

#endif //THIS_IS_AN_EXAMPLE_H

