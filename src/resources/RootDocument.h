//! 
//! \file
//!
//! RootDocument.h
//! 

//
// POCO Library Includes
//
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
using namespace Poco;
using namespace Poco::Net;

class RootDocument : public HTTPRequestHandler
{ 
public: 
    RootDocument( void );
    ~RootDocument( void );

    void handleRequest( HTTPServerRequest& request, 
			            HTTPServerResponse& response );
};
