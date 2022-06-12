#include "include/Request.hpp"
#include "include/Utils.hpp"


Request::Request()
{
    _content_length = 0;
    _readed_body = 0;
    _buffer = "";
    _isHeaderReaded = false;
    _is_chanked = false;
}

int         Request::getContentLength()
{
    return (this->_content_length);
}


int         Request::getReadBody()
{
    return (this->_readed_body);
}


std::string Request::getBuffer()
{
    return (this->_buffer);
}

void        Request::setHeaderReaded(bool isHeaderReaded)
{
    this->_isHeaderReaded = isHeaderReaded;
}

// Member functions
void        Request::appandBuffer(char * buffer, int length)
{
    this->_buffer.append(std::string(buffer, length));
    if (this->_isHeaderReaded == false)
    {
        this->_readed_body += calculateReadedBody(buffer, length);
        this->_isHeaderReaded = true;
        return ;
    }
    this->_readed_body += length;
}

bool        Request::isHeaderReaded()
{
    return (this->_isHeaderReaded);
}

bool        Request::isChanked()
{
    return (this->_is_chanked);
}

int         Request::calculateReadedBody(char *buffer, int length)
{
	for (int i = 0; i < (length - 3); i++)
	{
		if (buffer[i] == '\r' && buffer[i + 1] == '\n' && buffer[i + 2] == '\r' && buffer[i + 3] == '\n')
			return (length - i - 4);
	}
	return (0);
}

// set Content-length and Transfer-Encoding
void         Request::setHeaders(char *buffer)
{
    LexerRe lexer(buffer);
	ParserRe parser(lexer);
	RequestInfo request_info = parser.parse();

	if (request_info.getHeaders().find("Content-Length") != request_info.getHeaders().end())
		this->_content_length =  stringToInt(request_info.getHeaders()["Content-Length"]);
    else
	    this->_content_length = 0;
    if (request_info.getHeaders().find("Transfer-Encoding") != request_info.getHeaders().end())
        if (request_info.getHeaders()["Transfer-Encoding"] == "chunked")
            this->_is_chanked = true;
}

int      isHexa(std::string str) // add to header
{
    size_t i;
    for (i = 0; i < str.length(); i++)
    {
        if (!((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'f')))
            return (0);
    }
    return (i);
}

void        Request::deleteDelimeter(bool begin) // true if begining,
{ 
    size_t pos = 0;
   // _readed_body; // minuse deleted delimeter !!!
   if (begin == true && (pos = _buffer.find("\r\n\r\n", pos)) != std::string::npos)
    {
        std::string hexa = _buffer.substr(pos + 4, _buffer.find("\r\n", pos + 4) - pos - 4);

        std::cout << "=========== Hexa ============" << std::endl;
        std::cout << hexa << std::endl;
        std::cout << "=========== Hexa ============" << std::endl;

        if (isHexa(hexa) != 0)
        {
            _buffer.erase(pos + 4, hexa.length() + 2);
            _readed_body -= (hexa.length() + 2);
        }
    }
    // while ((pos = this->_buffer.find("\r\n", pos)) != std::string::npos)
    // {
    //     this->_buffer.erase(this->_buffer.find("\r\n\r\n"), 4);
    // }
}