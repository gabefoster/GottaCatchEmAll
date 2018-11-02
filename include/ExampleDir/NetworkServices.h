/** @file
 * @brief Network Services is responsible for managing the network connection and settings of the sensor.
 */

#ifndef NETWORK_SERVICES_H_
#define NETWORK_SERVICES_H_

#include <cstdint>
#include <string>
#include <sys/socket.h>

#define ATTRIBUTE_PACKED __attribute__((__packed__))

// Port that the command server will listen on
#define DEFAULT_TCP_PORT 7000
#define HEADER_BUFFER_SIZE 1024

typedef struct {
  uint8_t   marker[4] = {'A', 'E', 'Y', 'E'}; ///< Magic Number
  uint32_t  bodySize; ///< This is body size
} ATTRIBUTE_PACKED PacketHeader;

enum class NetworkReturnStatus : int16_t {  // uses in commands responses as returning value.
  FATAL_NETWORK_ERROR       = -5,
  MEMORY_ALLOC_ERROR        = -4,
  PORT_OUT_OF_RANGE         = -3,
  COULD_NOT_RECEIVE_PACKET  = -2,
  BUFFER_FULL               = -1,
  NO_ERROR                  =  0,
  CONNECTION_CLOSED         =  1,
};

  /**
   * @brief Initialize the TCP command socket
   * @return Success or failure mode of method
   */
  int connectToTcp(const std::string& addr, uint16_t port);
  int acceptFromTcp(uint16_t port);

  /**
   * @brief Initialize the UDP data sockets
   * @param remoteAddr Socket address
   * @param localPort Port on which to initialize socket
   * @param outPort Outbound port on which to initialize socket
   * @param remoteIp IP address of client machine
   * @return
   */
  int initUdpSocket(struct sockaddr_in* remoteAddr, long localPort, long outPort, const char* remoteIp);

  /**
   * @brief Take in a network packet header
   * @param fd File descriptor of TCP socket on which to receive headers
   * @param header Header which has been received
   * @return Success or failure mode of method
   */
  NetworkReturnStatus recv_header(int fd, PacketHeader* header);

  /**
   * @brief Tag a network packet with an {AEYE} header marker.
   * @param header Header to be tagged
   */
  void setHeaderMarker(PacketHeader &header);

#endif /* NETWORK_SERVICES_H_ */

