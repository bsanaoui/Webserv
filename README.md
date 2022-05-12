# Webserv
This project is about writing my ow HTTP server.

# Socket 
  - Create an endpoint for communication
  - **Endpoint** any device that connects to a computer network.
  
## 1- Create Socket :

* int server_fd = socket(int domain, type, protocol);

* Domain, or adresse family :
    - Communication domain in which the socket should be created
    - AF_INET : IPv4 type

* Type : type of socket (STREAM, DATAGRAM ...)
    - We will use **SOCK_STREAM** that use TCP protocol
    - 
* Protocol :
    - indicate a specific protocol to use in supporting the sockets operation (if more than one , we can specified)

* Return is a fd;

## 2- Identify (name) a socket: 
* Assigning a transport address to the socket (a port number in IP networking)
* In sockets, this operation is called **binding** an address and the bind system call is used for this.
* Bind actually creates a **file** in the file system.
* int bind(int socketfd, const struct sockaddr *addr, socklen_t addrlen):
    - assigns the address specified by **addr** to the **socket** referred to by the **file descriptor sockfd**
    - address : 