#include "Response.hpp"

// --------------------------------------------------------- //
// --------------- Constructors and Operators -------------- //
// --------------------------------------------------------- //

Response::Response(int fd_sock_req, RequestInfo request_info, ServerSetup server_setup) 
    : _server_setup(server_setup), _request_info(request_info), _fd_sock_req(fd_sock_req), _type_req_target(IS_LOCATION)
{
    this->_fd_sock_req = fd_sock_req;
    // PUT Location info in server_info; if not found Error and send it 
    if (request_info.getRequest_target() != "/")
    {
        t_location *location;

        if (request_info.isBadRequest())
        {   
            std::cout << "Create ERROR(Bad Request) Response And Send it to the client" << std::endl; 
            this->_is_error = true;
            return ;
        }
        if (request_info.getRequest_target() != "/"
            && !(location = this->_server_setup.getLocation(request_info.getRequest_target(), &_type_req_target)))
        {
            if (this->_type_req_target == IS_NOT_FOUND)
            {
                std::cout << "Create ERROR(Not Found File/Location) Response And Send it to the client" << std::endl;
                this->_is_error = true;
                return ;
            }
        }
        InitResponseConfig(location);
    }
    this->_response_file.open(RESPONSE_FILE_NAME, std::ios::out); // Open File
    if (!_response_file.is_open())
            std::cout << "Create ERROR(File not opened) Response And Send it to the client" << std::endl;
}


Response::~Response() // Memory Leaks if exist
{
    this->_response_file.close();
    std::remove(RESPONSE_FILE_NAME);
}

// --------------------------------------------------------- //
// --------------------  Member Methods -------------------- //
// --------------------------------------------------------- //

void            Response::InitResponseConfig(t_location *location)
{
    if (location->root.length())
        _server_setup.root += location->root;
    if (!location->index.empty())
        _server_setup.index = location->index;
    if (!location->error_pages.empty())
        _server_setup.error_pages = location->error_pages;
    if (location->client_max_body_size != -1)
        _server_setup.client_max_body_size = location->client_max_body_size;
    if (!location->request_method.empty())
        _server_setup.request_method = location->request_method;
    if (location->autoindex.length())
         _server_setup.autoindex = location->autoindex;
}

std::pair<std::string, std::string>    Response::getErrorPage(int& status_code) // (pair(path, msg))
{
    // Check in the config file;
    std::vector<std::pair<short, std::string> > v = _server_setup.getError_pages();
    for(int i = 0; i < (int)v.size(); i++)
        if (v[i].first == status_code)
            return (std::make_pair(_server_setup.getRoot() + v[i].second, "OK"));
    if (status_code == 404)
        return (std::make_pair(ERROR_PAGE_404, "Page_Not_Found"));
    else if (status_code == 500)
        return (std::make_pair(ERROR_PAGE_500, "KO"));
    else if (status_code == 400)
        return (std::make_pair(ERROR_PAGE_400, "KO"));
    return (std::make_pair(ERROR_PAGE_404, "KO")); // default
}


void               Response::appendStartLine(int& status_code, const std::string& msg)
{
    this->_response_file << "HTTP/1.1 ";
    this->_response_file << status_code;
    this->_response_file << " " + msg;
    this->_response_file << "\r\n";
}

void                Response::appendContentType(const std::string& path)
{
    this->_response_file << "Content-Type: ";
    this->_response_file << "text/html";  (void)path ;//content-type = function(path)
    this->_response_file << "\r\n";
}

void                Response::appendContentLength(const std::string& path)
{
    this->_response_file << "Content-Length: ";
    this->_response_file << "10000";  (void)path; //size_file = function(path) 
    this->_response_file << "\r\n";
}

void                Response::appendBody(const std::string& path)
{
    std::ifstream   in_file(path);
    std::string     tmp_line;

    if (!in_file.is_open())
            return ;
    this->_response_file << "\r\n";

    while (std::getline(in_file, tmp_line))
    {
        this->_response_file << tmp_line;
        if (!in_file.eof())
            this->_response_file << "\n";
    }
    if (in_file.is_open())
        in_file.close();
}
// --------------------------------------------------------- //
// ------------------  Non Member Functions ---------------- //
// --------------------------------------------------------- //



// ----------------------------- TEST !!!!! ---------------------------------------//
// ----------------------------- TEST !!!!! ---------------------------------------//
std::string  Response::test(RequestInfo request_info, ServerSetup server_setup){

    //1 -  verify any method (GET POST DELETE) //     // and exist in config file
    // Verifiez extention of file if exist
    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: ";
    // Verifier root && the file if exist
    // Calculate Lenght of the file;
    if (request_info.getRequest_method() == "GET")
    {   
        std::string path;
        // if root
        if (request_info.getRequest_target() == "/about")
            path = server_setup.getRoot() + server_setup.getLocations()[0].path + "/" + server_setup.getLocations()[0].index[0];
        else // if location
            path = server_setup.getRoot() + "/" + server_setup.getIndex().at(0);
        
        std::ifstream index_file(path);

        if (!index_file.is_open())
            return (0);
    
        std::cout << "\n\n/<< ****************** Index Content : ********************* >>"<< std:: endl;
        std::string response_line;
        std::string body;
        int         size_body = 0;
        while (std::getline(index_file, response_line))
        {
            // std::istringstream iss(response_line);
            // int a, b;
            // if (!(iss >> a >> b)) { break; } // error
            body.append(response_line);
            size_body += response_line.length();
            if (!index_file.eof())
            {
                body.append("\n");
                size_body++;
            }
        }
        response.append(std::to_string(size_body));
        response.append("\r\n\r\n");
        response.append(body);
        index_file.close();
    }
    return (response);
}