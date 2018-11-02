/** @file
 * @brief Main control loop for listening for commands to run the system
 */

#ifndef TCP_LISTENER_H_
#define TCP_LISTENER_H_

#include <mutex>
#include <condition_variable>
#include <atomic>
#include "NetworkServices.h"

class tcpListener {
#ifdef BUILD_TESTING
  friend class tcpListenerTest;
#endif
public:
  tcpListener();
  explicit tcpListener(uint16_t port);
  tcpListener(const std::string& addr);
  tcpListener(const std::string& addr, uint16_t port);
  ~tcpListener();
  NetworkReturnStatus listen();
  void stopCommunicating();
  std::string passPacketUp();
  void passPacketDown(std::string packet);
  bool speak();
  /**
   * @brief Send packet header and body over TCP.
   * @param packetHeader AEye Packet Header
   * @param packetBody AEye Packet Body
   * @param bodySize Size of AEye Packet Body
   */
  int tcpSendAll(void* packet, size_t packetSize);
private:
  NetworkReturnStatus hear(std::string& incomingBody);
  std::string packet;
  std::mutex packetMutex;
  std::condition_variable packetReadiness;
  std::atomic<bool> isPacketReady;
  std::atomic<bool> isCommunicating;
  std::atomic<bool> isConnected;
  uint16_t port;
  int tcpFileDescriptor; //!< File descriptor of TCP connection
  int commandSocket; //!< TCP command socket

  bool receiveBody(void* const incomingBody, size_t incomingBodySize);
};

#endif /* TCP_LISTENER_H_ */
