/**
 * @file
 */

#include "NetworkServices.h"
#include "arpa/inet.h"
#include <algorithm>
#include <stdexcept>
#include <cstring>

int connectToTcp(const std::string& addr, uint16_t port) {
  int ret;  // for checking return values
  int listenfd;

  // Get a socket file descriptor for the command socket (via TCP)
  listenfd = socket(AF_INET, SOCK_STREAM, 0);

  // Check for an error in creating the socket
  if (listenfd == -1) {
    throw std::runtime_error("Could not create TCP socket.");
  }

  struct sockaddr_in serv_addr{};

  // Set up the socket options
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(addr.c_str());
  serv_addr.sin_port = htons(port);

  // Bind to the open socket
  ret = ::connect(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  if( ret < 0 ) { // bind failed
    throw std::runtime_error("Could not connect to port.");
  }

  // Listen on the TCP socket (accept 10 clients)
  //listen(listenfd, 10);
  return listenfd;
}

int acceptFromTcp(uint16_t port) {
  int ret;  // for checking return values
  int listenfd;

  // Get a socket file descriptor for the command socket (via TCP)
  listenfd = socket(AF_INET, SOCK_STREAM, 0);

  // Check for an error in creating the socket
  if (listenfd == -1) {
    throw std::runtime_error("Could not create TCP socket.");
  }

  struct sockaddr_in serv_addr{};

  // Set up the socket options
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port);

  // Bind to the open socket
  ret = bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  if( ret < 0 ) { // bind failed
    throw std::runtime_error("Could not connect to port.");
  }

  // Listen on the TCP socket (accept 10 clients)
  listen(listenfd, 10);
  return listenfd;
}

NetworkReturnStatus recv_header(int fd, PacketHeader* header) {
    char buffer[HEADER_BUFFER_SIZE];
    char* it_header_begin = buffer;
    unsigned int bytes_received = 0;
    const char marker[4] = {'A', 'E', 'Y', 'E'};

    do {

        int n = recv(fd, buffer + bytes_received,
                     std::min((unsigned int)(sizeof(PacketHeader)), HEADER_BUFFER_SIZE - bytes_received), MSG_NOSIGNAL);
        if( n == -1){
            return NetworkReturnStatus::COULD_NOT_RECEIVE_PACKET;
        }
        if ( n == 0){
            return NetworkReturnStatus::CONNECTION_CLOSED;
        }
        bytes_received += n;
        if ( bytes_received >= HEADER_BUFFER_SIZE){
            return NetworkReturnStatus::BUFFER_FULL;
        }
        it_header_begin = std::search(it_header_begin, buffer + bytes_received, marker, marker + 4);

    } while (
        it_header_begin == buffer + bytes_received
    );

    while ( bytes_received - (it_header_begin - buffer)< (int)sizeof(PacketHeader) ) {

        int n = recv(fd, buffer + bytes_received,
                     std::min((unsigned int)(sizeof(PacketHeader)), HEADER_BUFFER_SIZE - bytes_received), MSG_NOSIGNAL);
        if( n == -1){
            return NetworkReturnStatus::COULD_NOT_RECEIVE_PACKET;
        }
        if ( n == 0){
            return NetworkReturnStatus::CONNECTION_CLOSED;
        }
        bytes_received += n;
        if ( bytes_received >= 1024){
            return NetworkReturnStatus::BUFFER_FULL;
        }

    } // TODO: Fix comparison between signed and unsigned ints in do-while loop.

    memcpy(header, it_header_begin, sizeof(PacketHeader));
    return NetworkReturnStatus::NO_ERROR;
}
