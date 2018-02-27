include ../globals.make

LIB_SRCS = Resource.cpp \
	       CrtSvc.cpp

LOCAL_OBJS = $(LIB_SRCS:.cpp=.o)

.cpp.o:
	$(CC) $(CC_LIB_ARGS) -c $< -o $@

SUBDIRS   = 
SUBDIRS.A = 

all: $(SUBDIRS) $(LOCAL_OBJS)
	$(CC) $(LD_LIB_ARGS) -Wl,-soname,libCrtSvc.so -o libCrtSvc.so $(LOCAL_OBJS)

	mkdir --mode=755 -p ../lib/
	cp ./*.so ../lib

static: $(SUBDIRS.A) $(LOCAL_OBJS)
	$(AR) rcs libCrtSvc.a $(LOCAL_OBJS)

	mkdir --mode=755 -p ../lib/
	cp ./*.a  ../lib

clean:
	rm -f *.o *.a *.so *~ 
