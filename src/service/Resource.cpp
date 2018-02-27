//!
//! \file
//! 
//! Resource.cpp
//! 

#include "Resource.h"

//
// Standard C++ Libary Includes
//
#include <sstream>

Resource::Resource( std::uint32_t resourceId ) 
  : id(resourceId), value( 0 )
{
}
 
Resource::Resource( const Resource & rhs )
  : id(rhs.id), value( rhs.value )
{
}

Resource::Resource( std::uint32_t resourceId, 
					std::uint32_t _value )
  : id(resourceId), value( _value  )
{
}

Resource::~Resource( void )
{
}

// Assigment operator - primarily for updates
Resource & Resource::operator=( const Resource & rhs )
{
  if ( this != &rhs )
  {
      id = rhs.id;
      value = rhs.value;
  }
  return *this;
}

//! Emit a JSON formatted object representation.
std::string Resource::toJSON(bool desc) const 
{
    std::stringstream ss;
    ss << "{ \"Resource\": {" 
       << ", \"id\": " << id;
    if ( desc ) {
       ss << ", \"value\": " << value;
    }
    ss << "} }";
    return ss.str();
}

//! Emit a string object representation.
std::string Resource::toString(bool desc) const
{
    std::stringstream ss;
    ss << "Resource: [ " 
       << "id: " << id;
    if ( desc ) {
       ss << "value: " << value;
    }
    ss << " ]";
    return ss.str();
}
