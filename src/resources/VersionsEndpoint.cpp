//! \file
//! 
//! VersionsEndpoint.cpp
//! 

#include "VersionsEndpoint.h"

//
// Standard C++ Libary Includes
//
#include <sstream>

VersionsEndpoint::VersionsEndpoint( void )
{
}

VersionsEndpoint::~VersionsEndpoint( void )
{
}

void VersionsEndpoint::handleRequest( HTTPServerRequest & request, 
			     HTTPServerResponse & response ) 
{ 
    response.setChunkedTransferEncoding( true ); 
    response.setContentType( "application/json" );
    std::ostream & ostr = response.send();
    ostr << "{ \"versions\": ["
	     << " { \"version_specifier\": \"v1\" } "
	     << "] }" << std::endl;
} 
