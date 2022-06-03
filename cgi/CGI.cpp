#include "CGI.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <fcntl.h> // open
#include <unistd.h> // fork, dup2, execve

std::vector<const char*>    setEnvp(RequestInfo &request, ServerSetup &server)
{
    std::vector<const char*> envp;

    envp.push_back(strdup((std::string("REQUEST_METHOD") + "=" + request.getRequest_method()).c_str()));
	envp.push_back(strdup((std::string("CONTENT_LENGTH") + "=" + std::to_string(request.getBody().length())).c_str()));
	envp.push_back(strdup((std::string("CONTENT_TYPE") + "=text/html").c_str()));
    envp.push_back(strdup((std::string("QUERY_STRING") + "=" + request.getQueryString()).c_str()));
    envp.push_back(strdup((std::string("REDIRECT_STATUS") + "=200").c_str()));
    envp.push_back(strdup((std::string("PATH_TRANSLATED") + "=" + server.getRoot()
            + request.getRequest_target()).c_str()));
	envp.push_back(strdup((std::string("PATH_INFO") + "=" + server.getRoot()
            + request.getRequest_target()).c_str()));
	envp.push_back(strdup((std::string("GATEWAY_INTERFACE") + "=CGI/1.1").c_str()));

	envp.push_back(strdup((std::string("SERVER_NAME") + "=" + server.getServer_name()[0]).c_str()));
	envp.push_back(strdup((std::string("SERVER_PORT") + "=" + std::to_string(server.getListen().first)).c_str()));
	envp.push_back(strdup((std::string("SERVER_PROTOCOL") + "=HTTP/1.1").c_str()));
	// envp.push_back(strdup((std::string("SERVER_SOFTWARE") + "=" + "SERVER_NAME").c_str()));
	envp.push_back(strdup((std::string("REMOTE_ADDR") + "=127.0.0.1").c_str()));
	envp.push_back(strdup((std::string("REMOTE_HOST") + "=" + "localhost").c_str()));
    envp.push_back(strdup((std::string("SCRIPT_NAME") + "=" + request.getRequest_target()).c_str()));
    envp.push_back(NULL);
    return (envp);
}

const std::string     handle_cgi(std::string path, RequestInfo &request, ServerSetup &server)
{   
    std::cout << "Handle a CGI: " << path << std::endl; // DEBUG

    std::vector<const char*> argv;
    std::vector<const char*> envp;
    std::string out_file = "cgi.html";
    std::fstream in_file("body_req.txt");
    pid_t pid;

    envp = setEnvp(request, server);
    argv.push_back("php-cgi");
    argv.push_back(path.c_str());
    argv.push_back(NULL);

    in_file << request.getBody();
    in_file.close();
    
    int fd_out = open(out_file.c_str(), O_CREAT|O_RDWR, 0777);
    int fd_in = open("body_req.txt", O_CREAT|O_RDWR, 0777);

    if ((pid = fork()) == - 1)
        return ("Error: fork"); // Set error fork
    else if (pid == 0)
    {
        dup2(fd_in, 0);
        dup2(fd_out, 1);
        execve(argv[0], const_cast<char * const *>(argv.data()), const_cast<char * const *>(envp.data()));
        exit (0);
    }
    else
        waitpid(pid, NULL, 0);
    close (fd_in);
    close (fd_out);
    return out_file;
}