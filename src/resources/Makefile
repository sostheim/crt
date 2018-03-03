include ../globals.make

LIB_SRCS   = $(wildcard *.cpp)
LOCAL_OBJS = $(LIB_SRCS:.cpp=.o)

all: $(LOCAL_OBJS)
	$(CC) $(LD_LIB_ARGS) -Wl,-soname,libCrtRestInterface.so -o libCrtRestInterface.so $(LOCAL_OBJS)

	mkdir --mode=755 -p ../lib/
	cp ./*.so ../lib

static: $(LOCAL_OBJS)
	$(AR) rcs libCrtRestInterface.a $(LOCAL_OBJS)

	mkdir --mode=755 -p ../lib/
	cp ./*.a  ../lib

.cpp.o:
	$(CC) $(CC_LIB_ARGS) -c $< -o $@

clean:
	rm -f *.o *.a *.so *~ 
