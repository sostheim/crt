//! \file
//! 
//! APIError.cpp
//! 

#include "APIError.h"

//
// Standard C++ Libary Includes
//
#include <sstream>

APIError::APIError( HTTPResponse::HTTPStatus _status,
		            const std::string & _reason )
  : status( _status ),
    reason( _reason )
{
}

APIError::~APIError( void )
{
}

void APIError::handleRequest( HTTPServerRequest & request, 
			                  HTTPServerResponse & response ) 
{ 
    response.setChunkedTransferEncoding( true ); 
    response.setContentType( "text/plain" );
    if ( reason.empty() )
    {
	response.setStatusAndReason( status );
    }
    else
    {
        response.setStatusAndReason( status, reason );
    }
    response.send();
} 
