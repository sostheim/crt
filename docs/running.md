# Running the CRT Application
To run the application, it must be built and installed on a target system.  For our purposes, the current execution environment is a Docker container.  Specifically, the Docker container used as the reproducible build context in the [Building the CRT Application](building.md) guide.

* Coming Soon: running the application as a Kubernetes Deployment

## Help
The application has a help option (`-h`) to get you started. Most of the basics are covered in this context.
```
root@eb878eedb396:/usr/src/src# /usr/local/crt/bin/crt -h
NAME
	C++ REST Template (crt) Application 

SYNOPSIS:
	crt [OPTIONS]

OPTIONS:
	-d	enable the controller to run as a daemon process, default: disabled
	-r	enable the HTTP REST interface, default: disabled
	-p <port number>
		specify the port for the HTTP REST API server to listen on, default: 8092
	-l <logging level>
		set the desired level of log file output, default: notice
		the value specified for logging level may only be one of the following values:
		  fatal       - errors that cause the system to exit
		  critical    - errors that cause unexpected/unpredictable system behavior
		  error       - errors that are handled and allow the system to continue operating
		  warning     - indication of a condition that merit ongoing monitoring
		  notice      - general system condition notification, e.g. timer expiry
		  information - general operational output
		  debug       - verbose output for system troubleshooting
		  trace       - extremely verbose output for full system diagnostics
```

## Basic Execution
To simply run the CRT Application execute the command:
```
root@eb878eedb396:/usr/src/src# /usr/local/crt/bin/crt -d -r         
```
That's it.  What's actually taken place here is that the `crt` application started up as a Linux daemon (`-d`) process, and is running the REST API Service (`-r`).  

We can see this as follows:
```
root@eb878eedb396:/usr/src/src# ps -eaf
UID        PID  PPID  C STIME TTY          TIME CMD
root         1     0  0 19:35 pts/0    00:00:00 bash
root       182     1  0 20:01 ?        00:00:00 /usr/local/crt/bin/crt -d -r
```

## Logging
In the previous example the application was run with the default logging level: `notice`.  As such, there weren't any events to see in the CRT log file.  

Restart the application (after terminating the old one to avoid a port collision), with the logging level set to `debug`.  
```
root@eb878eedb396:/usr/src/src# /usr/local/crt/bin/crt -d -r -l debug        
```  
Now inspect the logfile to view the debugging information output.
```
root@eb878eedb396:/usr/src/src# cat /var/log/crt/crt.log 
# Log file created/rotated Tuesday, 27 Feb 18 20:12:44 GMT
02-27-2018 20:12:44.238 | 199:0 | Information(6) | crt.cpp:575 | main(): REST API Initialized. |
02-27-2018 20:12:44.238 | 199:0 | Information(6) | crt.cpp:578 | main(): successful initialization, entering main event loop. |
```

## Query the REST API
Now that the service is running in the background as a daemon process, and listening on the default port, we can begin querying the service.  The debug logging remains enabled to show additional output.  As noted in the [Help](#Help) section above, the default port is 8092.

Query the root endpoint (`/`) on the default port.
```
root@eb878eedb396:/usr/src/src# curl http://127.0.0.1:8092/                      
{
	"endpoints": [{
		"urn": "/v1",
		"supported_methods": ["GET"]
	}, {
		"urn": "/versions",
		"supported_methods": ["GET"]
	}, {
		"urn": "/health",
		"supported_methods": ["GET"]
	}]
}
```
Note: the returned JSON body has been externally formatted for readability (steps not shown).

Query the unversioned health check endpoint (`/health`).
```
root@eb878eedb396:/usr/src/src# curl http://127.0.0.1:8092/health
{ "type": "liveness" }
```

Query a versioned endpoint... etc... more to come when they're working...

## Shutting Down 
The daemon process gracefully handles shutdown requests from the Linux [kill](http://man7.org/linux/man-pages/man1/kill.1.html) command.
```
root@eb878eedb396:/usr/src/src# ps aux | grep crt
root       199  0.0  0.4 276212  9136 ?        Sl   20:12   0:01 /usr/local/crt/bin/crt -d -r -l debug

root@eb878eedb396:/usr/src/src# kill 199
```

### Logged Output
From the series of queries shown above, the following logging output is captured.
```
# cat /var/log/crt/crt.log 
# Log file created/rotated Tuesday, 27 Feb 18 20:12:44 GMT
02-27-2018 20:12:44.238 | 199:0 | Information(6) | crt.cpp:575 | main(): REST API Initialized. |
02-27-2018 20:12:44.238 | 199:0 | Information(6) | crt.cpp:578 | main(): successful initialization, entering main event loop. |
02-27-2018 20:21:15.040 | 199:2 | Information(6) | Router.cpp:87 | createRequestHandler(): request method: GET, URI: /, Query:  |
02-27-2018 20:25:39.251 | 199:2 | Information(6) | Router.cpp:87 | createRequestHandler(): request method: GET, URI: /health, Query:  |
02-27-2018 20:25:39.251 | 199:2 | Debug(7) | MonitorEndpoint.cpp:46 | MonitorEndpoint::handleRequest()) |
02-27-2018 20:34:36.943 | 199:0 | Information(6) | crt.cpp:125 | ::controller(): signaled quit/shutdown. |
02-27-2018 20:34:37.183 | 199:0 | Information(6) | crt.cpp:590 | main(): exiting. |
```
