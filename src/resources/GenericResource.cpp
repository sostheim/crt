//! \file
//! 
//! GenericResource.cpp
//! 

#include "GenericResource.h"

//
// Standard C++ Libary Includes
//
#include <sstream>

GenericResource::GenericResource( CrtSvc & _crtSvc, std::uint32_t _resourceId, bool _descriptor ) 
  : crtSvc( _crtSvc ),
    resourceId( _resourceId ),
    descriptor( _descriptor )
{
}

GenericResource::~GenericResource( void )
{
}

void GenericResource::handleRequest( HTTPServerRequest & request, 
				                     HTTPServerResponse & response ) 
{ 
    response.setChunkedTransferEncoding( true ); 
    response.setContentType( "application/json" );
    std::ostream & ostr = response.send();
    ostr << crtSvc.resourceJSON( resourceId, descriptor );
} 
