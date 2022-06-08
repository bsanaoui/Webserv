#ifndef SERVER_HPP
#define SERVER_HPP

#include "Utils.hpp"
#define LENGTH_RECV_BUFFER 2048
class Server{

  std::vector<struct sockaddr_in> _v_address;
  std::vector<int> _v_server_fd;

  int _server_fd;
  struct sockaddr_in _address;
  int _address_len;
  int opt;

  public:

  Server(std::vector<ServerSetup> servers);
  std::vector<int> GetServerFds();
  //pair<server, possition>
  int     AcceptNewConnection(std::pair<int, size_t> pair);
  static  void handleConnection(ServerSetup server_setup, int new_socket);
  static  std::string   receiveRequest(int new_socket);

};

#endif