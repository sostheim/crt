include globals.make

LOCAL_SRCS = Router.cpp \
	     crt.cpp

LOCAL_OBJS = $(LOCAL_SRCS:.cpp=.o) 

LD_ARGS += -lCrtSvc -lCrtRestInterface 

SUBDIRS   = svc api
SUBDIRS.A = svc.a api.a

all: $(SUBDIRS) $(LOCAL_OBJS)
	$(CC) $(CC_ARGS) -o crt $(LOCAL_OBJS) $(LD_ARGS)

static: $(SUBDIRS.A) $(LOCAL_OBJS)
	$(CC) $(CC_ARGS) -o crt $(LOCAL_OBJS) $(LD_ARGS)

svc:
	cd service && $(MAKE)

svc.a:
	cd service && $(MAKE) static

api:
	cd resources && $(MAKE)

api.a:
	cd resources && $(MAKE) static

.cpp.o:
	$(CC) $(CC_ARGS) -c $< -o $@

clean:
	cd service && $(MAKE) clean
	cd resources && $(MAKE) clean
	rm -rf pkgdir
	rm -f lib/* xfer/* *.o *~ *.deb crt

transfer:
	mkdir --mode=755 -p ./xfer
	rm -f ./xfer/*
	cp crt ./xfer/
	cp ./lib/*.so ./xfer/
	cd xfer && tar czvf dx_xfer.tgz *

install:
	mkdir --mode=755 -p /usr/local/crt/bin
	mkdir --mode=755 -p /usr/local/crt/lib
	cp ./crt /usr/local/crt/bin
	cp ./lib/*.so /usr/local/crt/lib
	cp ./etc/ld.so.conf.d/crt.conf /etc/ld.so.conf.d/
	mkdir --mode=755 -p /var/log/crt/
	chmod 777 /var/log/crt/
	ldconfig

package:
        #
        # Always start clean just in case there are residual directories laying 
        # around from a previous, or failed, build.  
        #
	rm -rf pkgdir

        #
        # Instead of making a separate package for POCO, for right now we just bundle it
        # we remove the libData* to obviate dependencies on mysqlclient and odbc.
        #
	mkdir --mode=755 -p ./pkgdir/usr/lib
	cp -d /usr/local/lib/libPoco* ./pkgdir/usr/lib
	cd ./pkgdir/usr/lib; \
	rm -f libPocoData*

	chmod 644 ./pkgdir/usr/lib/*.so

        #
        # The rest is analogous to install, but with pkgdir as the root and release versioning
        #
	mkdir --mode=755 -p ./pkgdir/etc/ld.so.conf.d
	mkdir --mode=755 -p ./pkgdir/usr/bin
	mkdir --mode=755 -p ./pkgdir/usr/local/crt/bin
	mkdir --mode=755 -p ./pkgdir/usr/local/crt/lib
	mkdir --mode=755 -p ./pkgdir/var/log/crt/
	mkdir --mode=755 -p ./pkgdir/var/log/crt/
	chmod 755 ./pkgdir/var/log/crt/

        # Conf files
	cp -r ./etc/ ./pkgdir/

        # crt pplication binary
	cp ./crt ./pkgdir/usr/local/crt/bin/crt.$(VERSION)

	cd ./pkgdir/usr/bin; \
	ln -f -s ../local/crt/bin/crt.$(VERSION) crt

        # crt application libraries
	cp ./lib/libCrtSvc.so ./pkgdir/usr/local/crt/lib/libCrtSvc.so.$(VERSION)
	cp ./lib/libCrtRestInterface.so ./pkgdir/usr/local/crt/lib/libCrtRestInterface.so.$(VERSION)

	cd ./pkgdir/usr/local/crt/lib \
	ln -f -s libCrtSvc.so.$(VERSION) libCrtSvc.so \
	ln -f -s libCrtRestInterface.so.$(VERSION) libCrtRestInterface.so \

        #
        # Create the Debian package
        #
	rm -f crt-x86_64.$(VERSION).deb

	fpm -s dir -t deb \
	    -C ./pkgdir \
	    --name crt -v $(VERSION) \
	    --package crt-x86_64.$(VERSION).deb \
	    --maintainer Richard Sostheim \
	    --description "The C++ Rest Template (crt) Service" \
	    --provides "crt" \
	    --depends 'openssl > 1.0' \
	usr var etc

	chown root:root crt-x86_64.$(VERSION).deb

	rm -rf pkgdir
