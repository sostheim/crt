//! 
//! \file
//!
//! VersionsEndpoint.h
//! 

//
// POCO Library Includes
//
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
using namespace Poco;
using namespace Poco::Net;

class VersionsEndpoint : public HTTPRequestHandler
{ 
public: 
    VersionsEndpoint( void );
    ~VersionsEndpoint( void );

    void handleRequest( HTTPServerRequest& request, 
			            HTTPServerResponse& response );
};
