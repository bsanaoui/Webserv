#include "include/Server.hpp"
#include "include/Utils.hpp"

#include "parser_request/include/ParserRe.hpp"
#include "parser_request/include/RequestInfo.hpp"
#include "response/Response.hpp"

Server::Server(std::vector<ServerSetup> servers) : _address_len(sizeof(_address)), opt(1)
{

	std::vector<ServerSetup>::iterator it(servers.begin());
	size_t size = servers.size();

	for (size_t i = 0; i < size; i++)
	{
		if ((_server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			std::cerr << "cannot creat socket" << std::endl;
			exit(EXIT_FAILURE);
		}
		_v_server_fd.push_back(_server_fd);

		memset(&_address, 0, _address_len);
		_address.sin_family = AF_INET; // use IPv4
		_address.sin_port = htons((*it).getListen().first);
		_address.sin_addr.s_addr = (*it).getListen().second;
		it++;
		_v_address.push_back(_address);

		// Control socket descriptors
		if (fcntl(_server_fd, F_SETFL, O_NONBLOCK) < 0)
		{
			std::cerr << "non_blocking error" << std::endl;
			exit(EXIT_FAILURE);
		}

		if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		{
			std::cerr << "setsockpt error" << std::endl;
			exit(EXIT_FAILURE);
		}

		if (!samePort(_v_address))
		{
			if (bind(_server_fd, (struct sockaddr *)&_address, _address_len) < 0)
			{
				std::cerr << "bind does not working" << std::endl;
				exit(EXIT_FAILURE);
			}
		}

		// limit backlog is 128
		if (listen(_server_fd, 128) < 0)
		{
			std::cerr << "In listen" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}

std::vector<int> Server::GetServerFds()
{
	return _v_server_fd;
}

// pair<server, possition>
int Server::AcceptNewConnection(std::pair<int, size_t> pair)
{
	int new_socket;
	if ((new_socket = accept(pair.first, (struct sockaddr *)&_v_address[pair.second], (socklen_t *)&_address_len)) < 0)
	{
		std::cerr << "In accept" << std::endl;
		exit(EXIT_FAILURE);
	}
	return new_socket;
}

void Server::handleConnection(ServerSetup server_setup, int new_socket)
{
	// std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 37\r\n\r\n<html><body><h2>default ok</h2>";
	// ---------------------- Reading Request --------------------------- //
	std::string request = receiveRequest(new_socket);

	// --------------------- Parsing The Request ------------------------- //
	LexerRe lexer(request);
	ParserRe parser(lexer);
	RequestInfo request_info = parser.parse();

	// ----------------------- Print Request ------------------------------ //

	std::cout << "<< ================== Start Request =================== >>" << std::endl;
	std::cout << request << "\n" << std::endl;
	std::cout << "<< =================== End Request ==================== >>" << std::endl;
	std::cout << "<< ================== Lenght all Request ============== >>" << std::endl;
	std::cout << "Lenght Body :" << request_info.getBody().length() << std::endl;
	std::cout << "<< ==================================================== >>" << std::endl;

	// ----------------------- Handle Response ----------------------------- //
	Response resp(new_socket, request_info, server_setup);
	resp.handleResponse();

	// ----------------------- Send Response To client --------------------- //
	if (resp.IsSended())
		resp.sendResponse();
	std::cout << "\n++++++++++ message sent ++++++++++++\n"
			  << std::endl;
}


// return pair(string, bool)
std::string Server::receiveRequest(int new_socket)
{
	char buffer[LENGTH_RECV_BUFFER] = {0};
	long valread = 0;
	std::string request_str;

	// valread = recv(new_socket, buffer, LENGTH_RECV_BUFFER, 0);
	// request_str = std::string(buffer, valread);
	// --------------------- Parsing The Request ------------------------- //
	// LexerRe lexer(buffer);
	// ParserRe parser(lexer);
	// RequestInfo request_info =  parser.parse();

	// int body_lenght = 0;
	// for(int i = 0; i < (valread - 3) ; i++)
	// {
	//   if (buffer[i] == '\r' && buffer[i + 1] == '\n'
	//         && buffer[i + 2] == '\r' && buffer[i + 3] == '\n')
	//         {
	//             body_lenght = valread - i - 4;
	//             break;
	//         }
	// }
	// valread = valread - body_lenght;
	// if (request_info.getHeaders().find("Content-Length") != request_info.getHeaders().end())
	// {
	//   int content_length = stringToInt(request_info.getHeaders()["Content-Length"]);
	//   std::cout << "content_length : " << content_length << std::endl;
	//   // valread = body_lenght;
	//   while (valread < content_length)
	//   {
	//     memset(buffer, 0, LENGTH_RECV_BUFFER);
	//     int valread_tmp = recv(new_socket, buffer, LENGTH_RECV_BUFFER, 0);
	//     valread += valread_tmp;
	//     if (valread_tmp > 0)
	//       request_str += std::string(buffer, valread_tmp); // to convert char* to string event /0 not stop in
	//   }
	// }
	// if (request_info.getHeaders().find("Content-Length") != request_info.getHeaders().end())
	// {
	  int valread_tmp;
	  while ((valread_tmp = recv(new_socket, buffer, LENGTH_RECV_BUFFER, 0)) > 0)
	  {
	    valread += valread_tmp;
	    request_str += std::string(buffer, valread_tmp);
		memset(buffer, 0, LENGTH_RECV_BUFFER);
	  }
	  // if 1(content lenght exist) && 2(check body lenght = content_length) 
	  				//=> return (request_str, true)
	  // else return ("", false);
	// }
	return (request_str);
}
