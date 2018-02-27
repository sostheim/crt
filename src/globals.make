#
# Global build configuration settings.
#

#
# setup the semantic version 
#
MAJOR=0
MINOR=1
PATCH=1
VERSION=${MAJOR}.${MINOR}.${PATCH}

#
# and the compile and linker env
#
CC          = g++
CPPFLAGS    = -std=c++14
DEBUGFLAGS  = -g -DDEBUG
3RDPARTY    = -DUSE_POCO_LIBRARIES
# POCO_LOG    =
POCO_LOG    = -DPOCO_LOG_DEBUG
# 3RDPARTY  = -DUSE_BOOST_LIBRARIES
DEFFLAGS    = $(CPPFLAGS) $(DEBUGFLAGS) $(3RDPARTY) $(POCO_LOG)
WARNFLAGS   = -Wall -Werror
PTHFLAG     = -pthread
NOFLAG      =
O2FLAG      = -O2
O3FLAG      = -O3
OPTFLAG     = $(NOFLAG)
RELOFLAGS   = -fPIC
INCDIRS     = 

CC_ARGS     = $(DEFFLAGS) $(WARNFLAGS) $(OPTFLAG) $(PTHFLAG) $(INCDIRS)
CC_LIB_ARGS = $(CC_ARGS) $(RELOFLAGS)

LIBTYPE     = -shared
ASNEEDED    = -Wl,--no-as-needed
POCO_LIBS   = -lPocoUtil -lPocoNet -lPocoNetSSL -lPocoFoundation
POCO_D_LIBS = -lPocoUtild -lPocoNetd -lPocoNetSSLd -lPocoFoundationd
LIBS        = -lstdc++ -lpthread -lrt -ldl $(POCO_D_LIBS) 
LIBDIRS     = -L./lib -L/usr/local/lib/ -L/usr/local/lib64/ 

LD_ARGS     = $(LIBDIRS) $(LIBS) 
LD_LIB_ARGS = $(LD_ARGS) $(LIBTYPE) $(ASNEEDED)
