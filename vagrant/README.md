# Vagrant Environment

The CRT environment is defined in the following Vagrantfile and correspodning bootstrap.sh.  These files should be easily adaptable to any vagrant environment, but they heavily favor users of https://github.com/strux/codemachine.

The bootstap takes care of building the system dependencies, most of which are fairly apt-get installable and therefore very straight forward.  There are a couple of dependineces to watch out for however...

## POCO C++ Libraries Project

The first one to watch out for is POCO Project (https://github.com/pocoproject/poco) which must be downloaded and built.  If the build is successful the appropriate header files and libraries are installed on the devbox and can be complied / linked against.  If the build fails, then the vagrant user's home directory will contain a ~/poco directory in a partially built state.  
