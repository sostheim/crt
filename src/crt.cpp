//! 
//! \file
//!
//! crt.cpp
//! 
//! Copyright 2018 Richard Sostheim.
//! 
//! Licensed under the Apache License, Version 2.0 (the "License");
//! you may not use this file except in compliance with the License.
//! You may obtain a copy of the License at
//! 
//!     http://www.apache.org/licenses/LICENSE-2.0
//! 
//! Unless required by applicable law or agreed to in writing, software
//! distributed under the License is distributed on an "AS IS" BASIS,
//! WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//! See the License for the specific language governing permissions and
//! limitations under the License.
//! 

//
// Standard C++ Libary Includes
//
#include <list>
#include <string>
#include <iostream>
#include <sstream>
#include <system_error>

//
// C++'d standard C libraries
//
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <cerrno>

//
// Standard C Libary Includes
//
#include <time.h>
#include <stdarg.h>
#include <unistd.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>

#if defined(USE_POCO_LIBRARIES)
//
// POCO Library Includes
//
#include <Poco/String.h>
#include <Poco/Logger.h>
#include <Poco/Message.h>
#include <Poco/AutoPtr.h>
#include <Poco/Exception.h>
#include <Poco/FileChannel.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/NetException.h>
#include <Poco/PatternFormatter.h>
#include <Poco/FormattingChannel.h>
using namespace Poco;
using namespace Poco::Net;
#endif // USE_POCO_LIBRARIES

#if defined(USE_BOOST_LIBRARIES)
//
// Boost Library Includes.
//
// Drag in all of ASIO
//
#include <boost/asio.hpp>
#endif // USE_BOOST_LIBRARIES

//
// Local Project Includes
//
#include "service/CrtSvc.h"
#include "Router.h"

//
// Performance measure constants
//
// 1,000,000,000 one nanosecond
static const unsigned int OneNanosecond = 1000000000;
static const unsigned int OneKilobyte = 1024;
static const unsigned int OneMegabyte = 1048576;
static const unsigned int OneGigabyte = 1073741824;

//
// Operational static globals
//
static bool g_rest( false );
static bool g_daemon( false );
static unsigned int g_interval( 10 );
static unsigned short g_port( 8092 );  // An IANA unassigned port to listen on, configurable.
static unsigned short g_running( 1 );
static std::string g_dbg_str( "DEBUG: " );
static std::string g_out_str;
static Logger * g_logger( 0 );

//
// Our main processing function
//
static void controller( CrtSvc & crtSvc ) 
{
    poco_trace( *g_logger, std::string( "::controller() entered." ) );

    while ( g_running )
    {
        if ( !g_rest )
        {
            poco_information( *g_logger, 
                              std::string( "{ \"Application\": \"crt\", \"State\": " +
                              crtSvc.toJSON() +  "}" ) );
        }
        poco_trace( *g_logger, 
                    std::string( "Crt goes to sleep, next poll operation update in: " +
                    std::to_string( g_interval ) +  " seconds." ) );
        sleep( g_interval );
        // crtSvc.doMeaningfulWork();
    }

    poco_information( *g_logger, "::controller(): signaled quit/shutdown." );
    poco_trace( *g_logger, std::string( "::controller() exit." ) );
}


static bool setup_http_server( HTTPServer ** server, CrtSvc & crtSvc )
{
    poco_trace( *g_logger, std::string( "::setup_http_server() entered." ) );
    try 
    {
        Poco::UInt16 port( g_port );
        ServerSocket serverSocket( port ); 

        HTTPServerParams * params( new HTTPServerParams ); 
        params->setMaxQueued( 32 );   // Queued Connections
        params->setMaxThreads( 4 ); 

        HTTPServer * srv( new HTTPServer( new Router( crtSvc ), 
                          serverSocket, 
                          params ) ); 
        //
        // Everything has been successfully initialized, start the HTTPServer 
        //
        srv->start(); 

        //
        // Return our intialized and running server 
        //
        *server = srv;
    }
    catch ( NetException & ne ) 
    {
        poco_fatal( *g_logger, 
		            std::string( "::setup_http_server(): error: caught NetException(): message: " +
				    ne.message() ) );
        return false;
    }
    catch( std::bad_alloc ) 
    {
        poco_fatal( *g_logger, 
		            std::string( "::setup_http_server(): error: caught bad_alloc\n" ) );
        return false;
    }

    poco_trace( *g_logger, std::string( "::setup_http_server() exit: true." ) );
    return true;
}


static bool setup_crt( void ) 
{
    poco_trace( *g_logger, std::string( "setup_crt() entered." ) );
    poco_trace( *g_logger, std::string( "setup_crt() exit: true." ) );
    return true;
}

//
// A standard function with the signal hander prototype: 
// - void (*sa_handler)(int) 
//
void shut_down( int signo )
{
    // shut down signal received
    g_running = 0;

    return;
}

static bool setup_signals( void )
{
    struct sigaction sa;

    //
    // Handle signals of interest.
    //
    sa.sa_handler = shut_down;
    sigemptyset( &sa.sa_mask );
    sigaddset( &sa.sa_mask, SIGTERM );
    sa.sa_flags = 0;
    if ( sigaction( SIGTERM, &sa, NULL ) < 0 ) 
    {
        std::cerr << "setup_signals(): error: can't register SIGTERM handler: errno: " 
		          << errno << std::endl;
	    return false;
    }

    sa.sa_handler = shut_down;
    sigemptyset( &sa.sa_mask );
    sigaddset( &sa.sa_mask, SIGQUIT );
    sa.sa_flags = 0;
    if ( sigaction( SIGQUIT, &sa, NULL ) < 0 ) 
    {
        std::cerr << "setup_signals(): error: can't register SIGQUIT handler: errno: " 
		          << errno << std::endl;
	    return false;
    }
    return true;
}

//
// Parse command line arguments
//
static bool opt_args( int argc, char **argv, Message::Priority & log_level )
{
    int idx = 0;
    while( ( idx = getopt( argc, argv, "dhrl:p:i:" ) ) > 0 )
    {
        switch( idx )
        {
        case 'd' : // run as a daemon
	        g_daemon = true;
            break;

        case 'r' : // enable the http api
	        g_rest = true;
            break;

        case 'p' : // port number parse
	    {
            g_port = atoi( optarg );
            if( g_port <= 0 || g_port > 65535 )
            {
                std::cerr << "error: invalid port specified: " 
                          << g_port 
			              << ", request ignored, using default port: 8092" 
			              << std::endl;
                g_port = 8092;
            }
    	}
	    break;

        case 'l' : // requested log level
	    {
            std::string log_string( optarg );
            if ( "fatal" == log_string ) {
                log_level = Message::Priority::PRIO_FATAL;
                break;
	        }
	        else if ( "critical" == log_string ) {
	            log_level = Message::Priority::PRIO_CRITICAL;
    		    break;
    	    }
	        else if ( "error" == log_string  ) {
	            log_level = Message::Priority::PRIO_ERROR;
		        break;
	        }
	        else if ( "warning" == log_string  ) {
	            log_level = Message::Priority::PRIO_WARNING;
		    break;
	        }
	        else if ( "notice" == log_string  ) {
	            log_level = Message::Priority::PRIO_NOTICE;
		        break;
	        }
	        else if ( "information" == log_string ) {
  	            log_level = Message::Priority::PRIO_INFORMATION;
		        break;
            }
            else if ( "debug" == log_string  ) {
                log_level = Message::Priority::PRIO_DEBUG;
                break;
            }
            else if ( "trace" == log_string )
            {
                log_level = Message::Priority::PRIO_TRACE;
                break;
            }
            // this case preceeds default and 'h' so we can fall through
        }
	    break;

        case 'i' : // interval value parse
	    {
            int arg_val = atoi( optarg );
            if( arg_val <= 0 || arg_val > 86400 )
            {
                std::cerr << "error: invalid interval specified: " 
                          << arg_val
		            	  << "s, request ignored, using default: 100" 
			              << std::endl;
                arg_val = 100;
            }
	        else if( arg_val <= 10 )
            {
                std::cerr << "waring: using exceptionally short interval specified: " 
                          << arg_val
			              << "s, startup continuing but Crt may potentially use excessive system resources." 
			             << std::endl;
            }
    	    else if( arg_val >= 3600  || arg_val <= 86400 )
            {
                std::cerr << "waring: using exceptionally long interval specified: " 
                          << arg_val
	            		  << "s, network interface changes are likely to be missed at this interval." 
			              << std::endl;
            }
	        g_interval = arg_val;
    	}
	    break;
        
        case 'h' : // help switch
        default: 
            std::cout 
              << "NAME\n"
              << "\tC++ REST Template (crt) Application \n\n"
              << "SYNOPSIS:\n"
              << "\tcrt [OPTIONS]\n\n"
              << "OPTIONS:\n"
              << "\t-d\tenable the controller to run as a daemon process, default: disabled\n"
              << "\t-r\tenable the HTTP REST interface, default: disabled\n"
              << "\t-p <port number>\n"
		      << "\t\tspecify the port for the HTTP REST API server to listen on, default: " << g_port <<"\n"
              << "\t-l <logging level>\n"
              << "\t\tset the desired level of log file output, default: notice\n"
              << "\t\tthe value specified for logging level may only be one of the following values:\n"
              << "\t\t  fatal       - errors that cause the system to exit\n"
              << "\t\t  critical    - errors that cause unexpected/unpredictable system behavior\n"
              << "\t\t  error       - errors that are handled and allow the system to continue operating\n"
              << "\t\t  warning     - indication of a condition that merit ongoing monitoring\n"
              << "\t\t  notice      - general system condition notification, e.g. timer expiry\n"
              << "\t\t  information - general operational output\n"
              << "\t\t  debug       - verbose output for system troubleshooting\n"
              << "\t\t  trace       - extremely verbose output for full system diagnostics\n"
              << std::endl;

            if ( idx == 'h' )
            {
                exit( EXIT_SUCCESS );
            }
            return false;
        }
    }

    return true;
}


//
// An Implementaiton of the classic APUE Daemonize Function
//
void daemonize( void )
{
    pid_t pid;
    struct rlimit rl;
    struct sigaction sa;

    //
    // clear file creation mask
    //
    umask( 0 );
    
    //
    // get maximum number of file description
    //
    if( getrlimit( RLIMIT_NOFILE, &rl ) < 0 ) 
    {
        std::cerr << g_out_str << "daemonize(): can't get file limit\n";
	exit( EXIT_FAILURE );
    }

    //
    // become a session leader to lose controlling TTYs
    //
    if( ( pid = fork() ) < 0 ) 
    {
        std::cerr << g_out_str << "daemonize(): can't fork (initial)\n";
	exit( EXIT_FAILURE );
    }
    else if( pid != 0 ) 
    { 
        // parent exits succesfully
	exit( EXIT_SUCCESS );
    }

    //
    // continuing in child process 
    //
    setsid(); 

    //
    // ensure future opens won't allocate controlling TTYs 
    //
    sa.sa_handler = SIG_IGN;
    sigemptyset( &sa.sa_mask );
    sa.sa_flags = 0;
    if( sigaction( SIGHUP, &sa, NULL ) < 0) 
    {
        std::cerr << g_out_str << "daemonize(): can't ignore SIG_HUP\n";
	    exit( EXIT_FAILURE );
    }

    if( ( pid = fork() ) < 0) 
    {
        std::cerr << g_out_str << "daemonize(): can't fork (daemon)\n";
	    exit( EXIT_FAILURE );
    }
    else if( pid != 0 ) 
    { 
        // parent exits succesfully
        exit( EXIT_SUCCESS );
    }

    //
    // change the current working directory 
    //
    if( chdir("/") < 0 ) 
    {
        std::cerr << g_out_str << "daemonize(): can't chdir()\n";
	    exit( EXIT_FAILURE );
    }

    //
    // close all open file descriptors 
    //
    if( rl.rlim_max == RLIM_INFINITY ) 
    {
        rl.rlim_max = 1024;
    }
    for( unsigned int i = 0; i < rl.rlim_max; i++ ) 
    {
        close( i );
    }

    // Change the working directory to the root directory
    chdir( "/" );

    // Close all open file descriptors
    for ( int x = sysconf(_SC_OPEN_MAX); x > 0 ; x--)
    {
        close ( x );
    }

    // that's it... were a damon.
}


static void setup_logger( Message::Priority log_level )
{
    try
    {
        FileChannel * channel( new FileChannel );
        channel->setProperty( "path", "/var/log/crt/crt.log");
        channel->setProperty( "rotation", "12 hours");
        channel->setProperty( "archive", "timestamp");
        channel->setProperty( "purgeAge", "14 days");

        FormattingChannel * fmtChannel = new FormattingChannel( new PatternFormatter( "%m-%d-%Y %H:%M:%S.%i | %P:%I | %p(%l) | %U:%u | %t |" ) );
        fmtChannel->setChannel( channel );
        fmtChannel->open();

        Logger::root().setChannel( fmtChannel );
        Logger::root().setLevel( log_level );
        Logger & logger = Logger::get( "crt" ); 
        g_logger = &logger;
    }
    catch ( Exception & ne ) 
    {
        std::cerr << g_out_str << "setup_logger(): fatal error: caught Exception(): message: "
		          << ne.message()
                  << ".  Unable to establish logging channel/logger\n";
    	exit( EXIT_FAILURE );
    }
    catch ( std::bad_alloc )
    {
        std::cerr << g_out_str << "setup_logger(): fatal error: caught memory allocation "
	        	  << "exception, unable to establish logging channel/logger\n";
	    exit( EXIT_FAILURE );
    }
    catch ( ... )
    {
        std::cerr << g_out_str << "setup_logger(): fatal error: caught general exception, "
		          << "unable to establish logging channel/logger\n";
    	exit( EXIT_FAILURE );
    }

    poco_trace( *g_logger, std::string( "Crt::setup_logger() exited." ) );
}


static void setup_log_strings( void )
{
    std::stringstream str;
    str << "crt[" << getpid() << "]: ";
    g_out_str = str.str();
    g_dbg_str += str.str();
}


int main( int argc, char ** argv )
{
    setup_log_strings();

    //
    // Parse our command line arguments first _or_die_
    //
    Message::Priority log_level( Message::Priority::PRIO_NOTICE );
    if ( false == opt_args( argc, argv, log_level ) )
    {
        std::cerr << g_out_str << "main(): fatal error: invalid or missing "
                  << "parameters, can't start:\n";
        return EXIT_FAILURE;
    }

    //
    // Run as daemon?
    //
    if ( g_daemon ) 
    {
        daemonize();
    }

    //
    // Setup a destination for log output _or_die_
    //
    setup_logger( log_level );

    //
    // Install our signal handlers _or_die_
    //
    if ( false == setup_signals() )
    {
        poco_fatal( *g_logger, "main(): error: unable to configure signal handlers." );
        return EXIT_FAILURE;
    }

    //
    // Initialize our interface to some meaningful backend service  _or_die_
    //
    if ( false == setup_crt() )
    {
        poco_fatal( *g_logger, "main(): error: unable to configure service(s)." );
        return EXIT_FAILURE;
    }

    //
    // As everything has successfully intialized, create the Crt Service.
    //
    CrtSvc crtSvc;

    //
    // Optionally Startup our HTTP Server _or_die_
    //
    HTTPServer * server( 0 );
    if ( g_rest )
    {
        if ( false == setup_http_server( &server, crtSvc ) )
	    {
	        poco_fatal( *g_logger, "main(): error: unable to configure HTTP Server" );
	        return EXIT_FAILURE;
	    }
    	poco_information( *g_logger, "main(): REST API Initialized." );
    }

    poco_information( *g_logger, "main(): successful initialization, entering main event loop." );

    //
    // Do do what we came here to do...
    //
    controller( crtSvc );

    if ( g_rest )
    {
        server->stop();
    }
    
    poco_information( *g_logger, "main(): exiting." );

    return EXIT_SUCCESS;
}
