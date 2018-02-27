# Builds
The project uses standard [GNU Make](https://www.gnu.org/software/make/manual/html_node/index.html) as the basis for the project [Makefile](../src/Makefile)

It is not recommended, but you can create a Linux server with a compilation environment capable of generating the reproducible build.  The steps are outlined in the Vagrant configuration.

The recommended method for building the project is to run the `make` command inside a Docker container using the image [GCC Development Environment](https://hub.docker.com/r/sostheim/gcc-dev-env/).  This image contains the latest version of the GCC compiler and several library dependencies.

* Coming Soon: the next step for this project is to package the deployment artifact as a Docker image...

## Make Targets
All of the following examples assume that you are running the Docker container described above, in the project’s `src/` directory with the following command.
```
$ docker run -v "$(pwd)":/usr/src/`basename $(pwd)` -it sostheim/gcc-dev-env
root@eb878eedb396:/usr/src# cd src
root@eb878eedb396:/usr/src/src# 
```
### `make`  
The default target for simply running the `make` command is `all`.  To build the project, accepting all the defaults.
```
root@eb878eedb396:/usr/src/src# make
[ . . . Verbose Output Omitted . . . ]
g++ -std=c++14 -g -DDEBUG -DUSE_POCO_LIBRARIES -DPOCO_LOG_DEBUG -Wall -Werror  -pthread  -c Router.cpp -o Router.o
g++ -std=c++14 -g -DDEBUG -DUSE_POCO_LIBRARIES -DPOCO_LOG_DEBUG -Wall -Werror  -pthread  -c crt.cpp -o crt.o
g++ -std=c++14 -g -DDEBUG -DUSE_POCO_LIBRARIES -DPOCO_LOG_DEBUG -Wall -Werror  -pthread  -o crt Router.o crt.o  -L./lib -L/usr/local/lib/ -L/usr/local/lib64/  -lstdc++ -lpthread -lrt -ldl -lPocoUtild -lPocoNetd -lPocoNetSSLd -lPocoFoundationd   -lCrtSvc -lCrtRestInterface 
```
### `make install`  
To install the newly built application and shared objects on to the system so that it can be run.
```
root@eb878eedb396:/usr/src/src# make install
mkdir --mode=755 -p /usr/local/crt/bin
mkdir --mode=755 -p /usr/local/crt/lib
cp ./crt /usr/local/crt/bin
cp ./lib/*.so /usr/local/crt/lib
cp ./etc/ld.so.conf.d/crt.conf /etc/ld.so.conf.d/
mkdir --mode=755 -p /var/log/crt/
chmod 777 /var/log/crt/
ldconfig
```
### `make clean`  
To clean up all of the build artifacts.
```
root@eb878eedb396:/usr/src/src# make clean
cd service && make clean
make[1]: Entering directory '/usr/src/src/service'
rm -f *.o *.a *.so *~ 
make[1]: Leaving directory '/usr/src/src/service'
cd resources && make clean
make[1]: Entering directory '/usr/src/src/resources'
rm -f *.o *.a *.so *~ 
make[1]: Leaving directory '/usr/src/src/resources'
rm -rf pkgdir
rm -f lib/* xfer/* *.o *~ *.deb crt
```
*Note:* running clean does not uninstall the application from the system.
