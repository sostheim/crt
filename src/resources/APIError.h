//! 
//! \file
//!
//! APIError.h
//! 

//
// C++ Standard Libraries
//
#include <string>

//
// POCO Library Includes
//
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
using namespace Poco;
using namespace Poco::Net;

class APIError : public HTTPRequestHandler
{ 
public: 
    explicit APIError( HTTPResponse::HTTPStatus status,
		       const std::string & reason = "" );
    ~APIError( void );

    void handleRequest( HTTPServerRequest& request, 
			HTTPServerResponse& response );

protected:
    HTTPResponse::HTTPStatus status;
    std::string reason;

private:
    explicit APIError( const APIError & );
};
