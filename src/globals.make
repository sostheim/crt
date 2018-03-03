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

# git commit sha
ifneq ("$(wildcard ../.git)","")
COMMIT = $(shell git rev-parse HEAD)
endif 

#
# and the compile and linker env
#
CC          = g++
CPPFLAGS    = -std=c++14
DEBUGFLAGS  = -g -DDEBUG
3RDPARTY    = -DUSE_POCO_LIBRARIES -DPOCO_LOG_DEBUG
# 3RDPARTY  = -DUSE_BOOST_LIBRARIES
DEFFLAGS    = $(CPPFLAGS) $(DEBUGFLAGS) $(3RDPARTY)
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
STATYPE     = -static
ASNEEDED    = -Wl,--no-as-needed
LIBDIRS     = -L./lib -L/usr/local/lib/ -L/usr/local/lib64/ 

POCO_LIBS   = -lPocoUtil -lPocoNet -lPocoNetSSL -lPocoFoundation
POCO_D_LIBS = -lPocoUtild -lPocoNetd -lPocoNetSSLd -lPocoFoundationd
LIBS        = -lstdc++ -lpthread -lrt -ldl $(POCO_D_LIBS) 

AR_P_LIBS   = -l:libPocoUtil.a -l:libPocoNet.a -l:libPocoNetSSL.a -l:libPocoFoundation.a
AR_P_D_LIBS = -l:libPocoUtild.a -l:libPocoNetd.a -l:libPocoNetSSLd.a -l:libPocoFoundationd.a
AR_LIBS     = -lstdc++ -lpthread -lrt -ldl $(AR_P_D_LIBS)

AR_ARGS     = $(LIBDIRS) $(AR_LIBS) 
LD_ARGS     = $(LIBDIRS) $(LIBS) 
LD_LIB_ARGS = $(LD_ARGS) $(LIBTYPE) $(ASNEEDED)
