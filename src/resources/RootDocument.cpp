//! \file
//! 
//! RootDocument.cpp
//! 
#include "RootDocument.h"

//
// Standard C++ Libary Includes
//
#include <sstream>

RootDocument::RootDocument( void )
{
}

RootDocument::~RootDocument( void )
{
}

void RootDocument::handleRequest( HTTPServerRequest & request,
                                  HTTPServerResponse & response ) 
{ 
    response.setChunkedTransferEncoding( true ); 
    response.setContentType( "application/json" );
    std::ostream & ostr = response.send();
    ostr << "{ \"endpoints\": [ "
         << "{ \"urn\": \"/v1\", \"supported_methods\" : [ \"GET\" ] },"
         << "{ \"urn\": \"/versions\", \"supported_methods\" : [ \"GET\" ] },"
         << "{ \"urn\": \"/health\", \"supported_methods\" : [ \"GET\" ] },"
         << "] }" << std::endl;
} 
