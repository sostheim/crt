//! \file
//! 
//! Router.cpp
//! 
#include "Router.h"

#include "resources/APIError.h"
#include "resources/RootDocument.h"
#include "resources/VersionsEndpoint.h"
#include "resources/Version1Endpoints.h"
#include "resources/GenericResource.h"
#include "resources/MonitorEndpoint.h"

//
// Standard C++ Libary Includes
//
#include <iostream>
#include <stdexcept>

//
// POCO Library Includes
//
#include <Poco/URI.h>
#include <Poco/Logger.h>
#include <Poco/Net/HTTPServerResponse.h>
using namespace Poco;

static Logger * g_logger( 0 );

// Returns true as the pair.first if the desired parameter is present and setin pair.second
std::pair<bool, std::string> getQueryStringArg( URI::QueryParameters & queryParms, std::string parm ) 
{
    for( auto qpIt : queryParms ) 
    {
		if ( parm == qpIt.first )
		{
			return std::pair<bool, std::string>( true, std::string( qpIt.second ));
		}
    }

    return std::pair<bool, std::string>(false, std::string());
}

// Returns pair.first: ture iff the argument can be converted to int which is then present in pair.second
std::pair<bool, std::uint32_t> queryStringArgToInt( std::string arg, bool rethrow = true ) 
{
    if ( not arg.empty() )
    {
		try 
		{
			return std::pair<bool, std::uint32_t>( true, std::stoi( arg ) );
		}
		catch( std::invalid_argument ) 
		{
			if ( rethrow ) throw; 
		}
		catch( std::out_of_range ) 
		{
			if ( rethrow ) throw; 
		}
    }

    return std::pair<bool, std::uint32_t>( false, 0 );
}

Router::Router( CrtSvc & _crtSvc ) 
  : crtSvc( _crtSvc )
{
    if ( !g_logger ) 
    {
        g_logger = &Logger::get( "Crt.Router" );
    }
}

Router::~Router( void )
{
}

HTTPRequestHandler * Router::createRequestHandler( const HTTPServerRequest & request ) 
{
    std::string path( request.getURI() );
    std::string method( request.getMethod() );
    URI uri( path );

    poco_information_f3( *g_logger,
			 			 "createRequestHandler(): request method: %s, URI: %s, Query: %s", 
			 			 method, path, uri.getQuery() );
    
    // The path as a collection of individual segments
    std::vector< std::string > segments;
    uri.getPathSegments( segments );

    // The URI's query parameters collection
    URI::QueryParameters queryParms( uri.getQueryParameters() );

    // The Request's method
    bool get( HTTPServerRequest::HTTP_GET == method );
    bool put( HTTPServerRequest::HTTP_PUT == method );
    bool post( HTTPServerRequest::HTTP_POST == method );
    bool patch( HTTPServerRequest::HTTP_PATCH == method );
    bool del( HTTPServerRequest::HTTP_DELETE == method );
    if ( not ( get || put || post || patch || del ) )
    {
        return new APIError( HTTPResponse::HTTPStatus::HTTP_METHOD_NOT_ALLOWED );
    }

    if ( path == "/" ) 
    {
        // root document endpoint
        if ( not get )
		{
			return new APIError( HTTPResponse::HTTPStatus::HTTP_METHOD_NOT_ALLOWED );
		}
        return new RootDocument(); 
    }
    else if ( ( path == "/versions" ) || ( path == "/versions/" ) )
    {
        // versions endpoint
        if ( not get )
		{
			return new APIError( HTTPResponse::HTTPStatus::HTTP_METHOD_NOT_ALLOWED );
		}
        return new VersionsEndpoint(); 
    }
    else if ( ( path == "/health" ) || ( path == "/health/" ) )
    {
        // versions endpoint
        if ( not get )
		{
			return new APIError( HTTPResponse::HTTPStatus::HTTP_METHOD_NOT_ALLOWED );
		}
        return new MonitorEndpoint( crtSvc ); 
    }
    else if ( ( path == "/v1" ) || ( path == "/v1/" ) )
    {
        // Base versioned URI
        if ( not get )
		{
			return new APIError( HTTPResponse::HTTPStatus::HTTP_METHOD_NOT_ALLOWED );
		}
        return new Version1Endpoints(); 
    }
    else if ( (segments[0] == "v1" ) && ((segments.size() > 1) && (segments[1] == "resources")))
    {
        if ( not get )
		{
			return new APIError( HTTPResponse::HTTPStatus::HTTP_METHOD_NOT_ALLOWED );
		}

		bool descriptors( false );
		std::pair<bool, std::string> descArg( getQueryStringArg( queryParms, "descriptors" ) );
		if ( descArg.first and (not descArg.second.empty() ) )
		{
			descriptors = (descArg.second == "true");
		}
	  
 		std::string resourceIdStr( (segments.size() > 2)? segments[2] : std::string() );
		if ( resourceIdStr.empty() ) 
		{
			return new APIError( HTTPResponse::HTTPStatus::HTTP_BAD_REQUEST, 
								 "No valid resource id specified in URI" ); 
		}

		poco_debug_f1( *g_logger,
					   "createRequestHandler(): resource id request string: %s", 
					   resourceIdStr );

		std::uint8_t id( 0 );
		try 
		{
			id = std::stoi( resourceIdStr );
		}
		catch( std::invalid_argument ) 
		{
			return new APIError( HTTPResponse::HTTPStatus::HTTP_BAD_REQUEST, 
								 "Invalid path element: <ResourceId> is garbage" ); 
		}
		catch( std::out_of_range ) 
		{
			return new APIError( HTTPResponse::HTTPStatus::HTTP_BAD_REQUEST, 
								 "Invalid path element: <ResourceId> is out of range" ); 
		}

		if ( not crtSvc.isValidResourceId( id ) )
		{
			return new APIError( HTTPResponse::HTTPStatus::HTTP_BAD_REQUEST, 
								 "Invalid path element: <ResourceId> is not known to the system" ); 
		}
        return new GenericResource( crtSvc, id, descriptors ); 
    }
    // Some bogus path that we don't recognize
    return new APIError( HTTPResponse::HTTPStatus::HTTP_NOT_FOUND );
}
