//! 
//! \file
//!
//! Version1Endpoints.h
//! 

//
// POCO Library Includes
//
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
using namespace Poco;
using namespace Poco::Net;

class Version1Endpoints : public HTTPRequestHandler
{ 
public: 
    Version1Endpoints( void );
    ~Version1Endpoints( void );

    void handleRequest( HTTPServerRequest& request, 
			            HTTPServerResponse& response );
};
