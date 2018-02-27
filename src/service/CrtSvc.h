//! 
//! \file
//!
//! CrtSvc.h
//! 
#pragma once

//
// Project Includes
//
#include "Resource.h"

//
// Standard C++ Libary Includes
//
#include <list>
#include <string>
#include <cstdint>

///
/// The primary component responsible interacting with some meaningful backend.
///
class CrtSvc
{ 
public: 
    CrtSvc( void );
    ~CrtSvc( void );

    void survey( void );
    void print( bool log = true );

    bool isValidResourceId( std::uint32_t resourceId ) const;

    //! Accessors
    const Resource * getResource( std::uint32_t resourceId ) const;
    inline const std::list< Resource * > getResources( void ) const { return resList; }
      
    //
    // JSON-ifiers
    //
    std::string toJSON( void ) const;
    std::string resourceJSON( std::uint32_t resourceId, bool desc = false ) const;
    std::string resourcesJSON( bool desc = false ) const;

private:     
    std::list< Resource * > resList;
};
