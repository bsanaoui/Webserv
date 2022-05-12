#include <iostream>
#include <sys/socket.h>

#define SERVER_PORT 80
#define MAXLINE     4096
#define SA struct sockaddr

int test()
{
    std::cout << "Hello, From Server" << std::endl;

    int server_fd;

    // if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    // {
    //     std::cerr << "cannot create socket";ß
    //     return (1);
    // }

    return (0);
}