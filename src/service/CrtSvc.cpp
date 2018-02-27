//! \file
//! 
//! CrtSvc.cpp
//! 

#include "CrtSvc.h"

//
// Standard C++ Libary Includes
//
#include <sstream>
#include <iostream>
#include <algorithm>
#include <functional>
#include <system_error>

//
// C++'d standard C libraries
//
#include <cstdlib>

//
// POCO Library Includes
//
#include <Poco/Logger.h>
using namespace Poco;
static Logger * g_logger( 0 );

CrtSvc::CrtSvc( void )
  : resList()
{
    if ( !g_logger ) 
    {
	    g_logger = &Logger::get( "Crt.CrtSvc" ); 
    }
}

CrtSvc::~CrtSvc( void )
{
}

bool CrtSvc::isValidResourceId( std::uint32_t resourceId ) const
{ 
    for ( auto res : resList ) {
        if ( resourceId == res->getId() ) {
            return true;
        }
    }
    return false;
}

const Resource * CrtSvc::getResource( std::uint32_t resourceId ) const
{ 
    for ( auto res : resList ) {
        if ( resourceId == res->getId() ) {
            return res;
        }
    }
    return nullptr;
}

std::string CrtSvc::toJSON( void ) const
{ 
    std::stringstream ss;
    ss << "{ \"Type\": \"CrtSvc\","
       << " \"Data\": "
       << resourcesJSON()
       << "}\n";

    return ss.str();
}


std::string CrtSvc::resourcesJSON( bool desc ) const
{ 
    bool first( true );
    std::stringstream ss;
    //
    // Encode the resources content optionally with the full descriptor
    //
    ss << "{ \"resources\":[ ";
    for ( auto res : resList ) {
        if ( not first ) ss << ", ";
            ss << res->toJSON( desc );
        first = false;
    }
    ss << "] }\n";
    return ss.str();
}

std::string CrtSvc::resourceJSON( std::uint32_t resourceId, bool desc ) const
{ 
    std::stringstream ss;

    //
    // Encode the resource content as a full descriptor
    //
    ss << "{ \"resource\": "; 
    for ( auto res : resList ) {
        if ( resourceId == res->getId() ) {
                ss << res->toJSON( desc );
            break;
        }
    }
    ss << " }";
    return ss.str();
}
