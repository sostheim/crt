//! \file
//! 
//! Version1Endpoints.cpp
//! 

#include "Version1Endpoints.h"

//
// Standard C++ Libary Includes
//
#include <sstream>

Version1Endpoints::Version1Endpoints( void )
{
}

Version1Endpoints::~Version1Endpoints( void )
{
}

void Version1Endpoints::handleRequest( HTTPServerRequest & request, 
				                       HTTPServerResponse & response ) 
{ 
    response.setChunkedTransferEncoding( true ); 
    response.setContentType( "application/json" );
    std::ostream & ostr = response.send();
    ostr << "{ \"endpoints\": ["
	     << " { \"urn\": \"/v1/resources[?descriptors=true]\", \"supported_methods\" : [ \"GET\" ] },"
	     << " { \"urn\": \"/v1/resources/<resource_id>[?descriptors=true]\", \"supported_methods\" : [ \"GET\", \"PUT\", \"POST\", \"DELETE\" ] }"
	     << "] }" << std::endl;
} 
