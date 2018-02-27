//! 
//! \file
//!
//! GenericResource.h
//! 

#include "../service/CrtSvc.h"

//
// POCO Library Includes
//
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
using namespace Poco;
using namespace Poco::Net;

class GenericResource : public HTTPRequestHandler
{ 
public: 
    GenericResource( CrtSvc & CrtSvc, std::uint32_t resourceId, bool descriptors = false );
    ~GenericResource( void );

    void handleRequest( HTTPServerRequest& request, 
			            HTTPServerResponse& response );
protected:
    CrtSvc & crtSvc;
    std::uint32_t resourceId;
    bool descriptor;
};
