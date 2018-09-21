/**
 * @file
 */

#ifndef THIS_IS_AN_EXAMPLE_H
#define THIS_IS_AN_EXAMPLE_H

#include <mutex>

/// Retrieves and stores the last chunk of an IPv4 address associated with an interface name.
class NetworkProperties {
  private:
    static std::once_flag once;
    static std::string ipAddrChunk; //!< Last chunk of IP address with interface name below.
  public:
    static constexpr char interfaceName[] = "wlp2s0"; //!< Interface name whose IP is to be determined.
    /**
     * @brief Retrieves the last chunk of the IPv4 address associated with the interfaceName above.
     * @return NetworkProperties::ipAddrChunk.
     */
    static const std::string& getLastIpChunk();
};

#endif //THIS_IS_AN_EXAMPLE_H

