/** @file
 * @brief Main control loop for listening for commands to run the system
 */

#ifndef TCP_LISTENER_H_
#define TCP_LISTENER_H_

#include <memory>
#include "NetworkServices.h"

class tcpListener {
#ifdef BUILD_TESTING
  friend class tcpListenerTest;
#endif
public:
  explicit tcpListener(uint16_t port);
  ~tcpListener();
  NetworkReturnStatus listen(std::string& incomingBody);
private:
  uint16_t port;
  int connfd; //!< File descriptor of TCP connection
  int cmd_sock; //!< TCP command socket
  static constexpr size_t MAX_IN_BODY_SIZE = 358400; //!< Maximum incoming packet size (350 KB)

  /**
   * @brief Send packet header and body over TCP.
   * @param packetHeader AEye Packet Header
   * @param packetBody AEye Packet Body
   * @param bodySize Size of AEye Packet Body
   */
  int tcpSendAll(void* packet, size_t packetSize);

  bool receiveBody(void* const incomingBody, size_t incomingBodySize);
};

#endif /* TCP_LISTENER_H_ */
