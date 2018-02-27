//! \file
//! 
//! MonitorEndpoint.cpp
//! 

#include "MonitorEndpoint.h"

//
// Standard C++ Libary Includes
//
#include <sstream>

//
// POCO Library Includes
//
#include <Poco/Logger.h>
using namespace Poco;
static Logger * g_logger( 0 );

MonitorEndpoint::MonitorEndpoint( CrtSvc & _crtSvc, MonitorType _type ) 
  : crtSvc( _crtSvc ),
    monType( _type )
{
    if ( !g_logger ) 
    {
		g_logger = &Logger::get( "Crt.MonitorEndpoint" ); 
    }
}

MonitorEndpoint::~MonitorEndpoint( void )
{
}

void MonitorEndpoint::handleRequest( HTTPServerRequest & request, 
				     				 HTTPServerResponse & response ) 
{ 
    poco_trace( *g_logger, "MonitorEndpoint::handleRequest(): Enter" );

    std::string method( request.getMethod() );

    response.setChunkedTransferEncoding( true ); 
    response.setContentType( "application/json" );

    bool get( HTTPServerRequest::HTTP_GET == method );
    
    poco_debug( *g_logger, "MonitorEndpoint::handleRequest())");

    if ( get ) 
    {
		poco_trace( *g_logger, "MonitorEndpoint::handleRequest(): GET monitor: Exit" );
		std::ostream & ostr = response.send();
		ostr << "{ \"type\": \"liveness\" }" << std::endl;
	}
    return;
} 
