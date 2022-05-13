#include <iostream>
#include <sys/socket.h> // socket
#include <netinet/in.h> // sockaddr_in
#include <unistd.h> //read, write

#define SERVER_PORT 8080


int test()
{
    int                 server_fd, new_socket;
    long                valread;
    struct sockaddr_in  address;
    int                 addrlen = sizeof(address);

    char                *msg = (char * )std::string("Hello, from server!").c_str();

    // Creating socket file desctiptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cerr << "In socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;           // AF_INET for TCP prototcol
    address.sin_addr.s_addr = INADDR_ANY;   // INADDR_ANY : binds the socket to all available interfaces.
    address.sin_port = htons(SERVER_PORT);  // htons : translates a short integer from host byte order to network byte order
    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    if(bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0 )
    {
        std::cerr << "In bind" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0)
    {
        std::cerr << "In listen" << std::endl;
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        std::cout << "\n+++++++ Waiting for new connection ++++++++\n" << std::endl;
        if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t*) &addrlen)) < 0)
        {
            std::cerr << "In accept" << std::endl;
            exit(EXIT_FAILURE);
        }


        char buffer[30000] = {0};
        valread = read (new_socket, buffer, 30000);
        if(valread < 0)
            std::cout << "No bytes are there to read" << std::endl;

        std::cout << buffer;
        write(new_socket, msg, strlen(msg));
        std::cout << "\n+++++++ Hello message sent ++++++++\n" << std::endl;
        close(new_socket);
    }

    return (0);
}