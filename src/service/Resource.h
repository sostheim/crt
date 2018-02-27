//! 
//! \file
//!
//! Resource.h
//!
#pragma once

//
// Standard C++ Libary Includes
//
#include <sstream>
#include <iostream>

///
/// Resource - Generic Resource Implementation
///
class Resource
{
public:
    // Class constants(s)

public:
    // Class enumeration(s)

public: 
    // 
    // Parameteric ctors and virtual dtor
    // 
    explicit Resource( std::uint32_t resourceId ); 

    // Standard copy ctor
    Resource( const Resource & );

    Resource( std::uint32_t resourceId, std::uint32_t value );

    virtual ~Resource( void );

    // Assigment operator - primarily for updates
    virtual Resource & operator=( const Resource & rhs );

    //! Emit a JSON formatted object representation.
    virtual std::string toJSON( bool desc = false ) const;

    //! Emit a string object representation.
    virtual std::string toString( bool desc = false ) const;

    //!
    //! Accessor's only. 
    //!
    inline std::uint32_t getId( void ) const { return id; }
    inline std::uint32_t getValue( void ) const { return value; }

private:
    std::uint32_t id;
    std::uint32_t value;
};
