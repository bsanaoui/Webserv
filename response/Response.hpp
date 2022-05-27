#ifndef __RESPONSE_HPP__
#define __RESPONSE_HPP__

#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <cstdio> // remove file response

#include "../parser_config/include/ServerSetup.hpp"
#include "../parser_request/include/RequestInfo.hpp"
#include "../include/EnumRequestTarget.hpp"

// Defines String
#define RESPONSE_FILE_NAME "response.tmp"

// Defines Error Pages
#define ERROR_PAGE_404 "error_pages/404_NOT_FOUND.html"
#define ERROR_PAGE_500 "error_pages/500_SERVER_ERROR.html"
#define ERROR_PAGE_400 "error_pages/400_BAD_REQUEST.html"


class Response
{
private:
    //------ private member --------- >>
    ServerSetup         _server_setup;      // Precis config of the request
    RequestInfo         _request_info;      // Request Info
    int                 _fd_sock_req;       // FD of Socket Request
    std::fstream        _response_file;     // temp "response http message file"
    bool                _is_error;          // In response
    TypeRequestTarget   _type_req_target;   // LOCATION, FILE, PHP_FILE or NOT_FOUND

public:
    // --------------------------------------------------------- //
    // --------------- Constructors and Operators -------------- //
    // --------------------------------------------------------- //
    Response(int fd_sock_req, RequestInfo request_info, ServerSetup server_setup);
    ~Response();

    // --------------------------------------------------------- //
    // --------------------  Member Methods -------------------- //
    // --------------------------------------------------------- //
    void                                    InitResponseConfig(t_location *location);
    std::pair<std::string, std::string>     getErrorPage(int& status_code);             //get (path,msg)
    void                                    appendStartLine(int& status_code, const std::string& msg);
    void                                    appendContentType(const std::string& path);
    void                                    appendContentLength(const std::string& path);
    void                                    appendBody(const std::string& path);
    // --------------------------------------------------------- //
    // ------------------  Non Member Functions ---------------- //
    // --------------------------------------------------------- //

    // --------------------  Test Methods ---------------------- //
    std::string     test(RequestInfo request_info, ServerSetup server_setup);

};

#endif // !RESPONSE_HPP
