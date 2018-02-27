#!/usr/bin/env bash
set -x 

################################################################################
# Note: these commands are run inside the VM as root
################################################################################

#
# This is going to take some time -> 'vagrant up' is going to be slow the first time
#
apt-get update

apt-get install -y python2.7-dev python-pip python-software-properties
sudo pip install --upgrade pip 
sudo pip install --upgrade virtualenv

apt-get install -y bridge-utils uml-utilities iftop ntp
apt-get install -y emacs24-nox 
apt-get install -y git git-core build-essential gdb 
apt-get install -y make cmake autoconf libtool pkg-config flex bison 
apt-get install -y pydb ddd valgrind valkyrie electric-fence libdmalloc5
apt-get install -y splint jlint pylint jshon silversearcher-ag nodejs
apt-get install -y wget curl zip unzip ssl-cert 
apt-get install -y libssl-dev libcurl4-openssl-dev libgcrypt-dev libxslt-dev 
apt-get install -y libyajl-dev libsensors4-dev libopenipmi-dev libmemcached-dev 
apt-get install -y libreadline-dev libssl-dev zlib1g-dev libxml2-dev libyaml-dev
apt-get install -y libpcap-dev librrd-dev libvirt-dev librabbitmq-dev libevent-dev
apt-get install -y sparsehash libcppunit-1.13-0 libboost-all-dev 
# POCO build dependencies
apt-get install -y libiodbc2 libiodbc2-dev libmysqlclient-dev
# iproute2 build dependencies
apt-get install -y libdb5.3-dev
# fpm
apt-get install ruby-dev
gem install fpm

#
# The following are all optional based on what the devbox is going to be used for and what services are needed
# - Note: several of these packages have competing dependencies (e.g. ports) and functionality (e.g. webservers, jdk's), 
# - so know what your installing and why.
#
# apt-get install -y openjdk-7-jdk

#
# For Oracle Java 8 JDK installer PPA for Ubuntu Trusty (14.04)
#
echo "deb http://ppa.launchpad.net/webupd8team/java/ubuntu trusty main" | tee /etc/apt/sources.list.d/webupd8team-java.list
echo "deb-src http://ppa.launchpad.net/webupd8team/java/ubuntu trusty main" | tee -a /etc/apt/sources.list.d/webupd8team-java.list
apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys EEA14886
apt-get update
echo oracle-java8-installer shared/accepted-oracle-license-v1-1 select true | sudo /usr/bin/debconf-set-selections
apt-get install -y oracle-java8-installer oracle-java8-set-default

#
# For Oracle Java 7 JDK installer PPA
#
# add-apt-repository -y ppa:webupd8team/java
#
# These Debian configuration variable settings are specific to the webupd8team/java PPA and are
# subject to change.  If the oracle-java7-installer fails (or hangs) this is likely the issue.
#
# echo debconf shared/accepted-oracle-license-v1-1 select true | sudo debconf-set-selections
# echo debconf shared/accepted-oracle-license-v1-1 seen true | sudo debconf-set-selections
# apt-get install -y oracle-java7-installer oracle-java7-set-default

apt-get install -y maven jlint

# apt-get install -y memcached 
# apt-get install -y redis-server 
# apt-get install -y rrdtool
# apt-get install -y collectd
# apt-get install -y lighttpd
# apt-get install -y apache2 apache2-dev 
# apt-get install -y php5-cgi php5
# apt-get install -y mongodb-10gen=2.4.11
# apt-get install -y libsqlite3-dev 
# apt-get install -y libpq-dev postgresql postgresql-contrib
# apt-get install -y perl libperl-dev 

# This ruby install is only needed if user-config.sh is changed to no longer install ruby.
# apt-get install -y ruby ruby-dev rdoc ri irb 

# Hub
wget https://github.com/github/hub/releases/download/v2.2.1/hub-linux-386-2.2.1.tar.gz
tar -xf hub-linux-386-2.2.1.tar.gz
cp hub-linux-386-2.2.1/hub /usr/local/bin/
chmod +x /usr/local/bin/hub
rm -rf hub*

# Uncommenting the next set of commands will install RabbitMQ Server
# wget https://www.rabbitmq.com/rabbitmq-signing-key-public.asc
# sudo apt-key add rabbitmq-signing-key-public.asc
# echo 'deb http://www.rabbitmq.com/debian/ testing main' | sudo tee /etc/apt/sources.list.d/rabbitmq.list
# apt-get update
# apt-get install rabbitmq-server

#
# Setup the synced_folder links and the user environment before continuing with the remainder of the setup.
#
su -c "source /vagrant/synced_folder/scripts/user-config.sh" vagrant
