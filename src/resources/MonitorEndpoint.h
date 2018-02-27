//! 
//! \file
//!
//! MonitorEndpoint.h
//! 

#include "../service/CrtSvc.h"

//
// POCO Library Includes
//
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
using namespace Poco;
using namespace Poco::Net;

class MonitorEndpoint : public HTTPRequestHandler
{ 
    //! MonitorType
    enum class MonitorType : std::uint8_t
    {
        Undefined = 0,
	    Liveness,
        Readiness,
	    Max
    };
    
    static std::string MonitorTypeToString( MonitorType val )
    {
        switch ( val ) {
        case MonitorType::Undefined: return "Undefined";
        case MonitorType::Liveness:  return "Liveness";
        case MonitorType::Readiness: return "Readiness";
        case MonitorType::Max:       return "Max";
        default:
    	  ;
	    }
	    return "Error";
    }

public: 
    //
    // Construct a Monitor for the generic service.  The monitor can
    // be for either Liveness (default), or Readiness 
    //
    MonitorEndpoint( CrtSvc & CrtSvc, MonitorType = MonitorType::Liveness );
    ~MonitorEndpoint( void );

    void handleRequest( HTTPServerRequest& request, 
            			HTTPServerResponse& response );
protected:
    CrtSvc & crtSvc;
    MonitorType monType;
};
