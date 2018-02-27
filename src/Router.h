//! 
//! \file
//!
//! Router.h
//! 

#include "service/CrtSvc.h"

//
// POCO Library Includes
//
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
using namespace Poco;
using namespace Poco::Net;

class Router : public HTTPRequestHandlerFactory 
{ 
public: 
    Router( CrtSvc & CrtSvc );
    ~Router( void );

    HTTPRequestHandler * createRequestHandler( const HTTPServerRequest & request );

protected:
    CrtSvc & crtSvc;
};
