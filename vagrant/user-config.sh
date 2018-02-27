#!/bin/bash

set -x

################################################################################
# Note: these commands are run inside the VM as the vagrant user 
################################################################################

################################################################################
# Dot files
################################################################################
for f in ~/synced_folder/global_config/.*; do [ -f $f ] && ln -s $f ~/$(basename $f); done

################################################################################
# Persisted directories (including ~/code)
################################################################################
for f in ~/synced_folder/local_config/*; do [ -d $f ] && ln -s $f ~/$(basename $f); done

################################################################################
# Auto completion
################################################################################
git_completion=https://raw.githubusercontent.com/git/git/master/contrib/completion/
wget -O ~/.git-completion.bash ${git_completion}git-completion.bash
wget -O ~/.git-prompt.sh ${git_completion}git-prompt.sh
hub_completion=https://raw.githubusercontent.com/github/hub/master/etc/
wget -O ~/.hub.bash_completion.sh ${hub_completion}hub.bash_completion.sh

################################################################################
# rbenv
################################################################################
git clone https://github.com/sstephenson/rbenv.git ~/.rbenv
mkdir ~/.rbenv/plugins
git clone git://github.com/sstephenson/ruby-build.git ~/.rbenv/plugins/ruby-build
export PATH="$HOME/.rbenv/bin:$PATH"
eval "$(rbenv init -)"

################################################################################
# Install common rubies
################################################################################
# for ruby in 1.9.3-p547 2.1.2
for ruby in 1.9.3-p547
do
  rbenv install ${ruby}
  rbenv rehash
  rbenv global ${ruby}
  # Bundler
  gem install bundler
  gem install debugger-ruby_core_source
  rbenv rehash
done

################################################################################
# Pull in project dependency POCO
################################################################################

# If it doesn't exist, clone POCO from source (as opposed to the version in the apt universe)
if [ ! -d /home/vagrant/code/github/poco ]; then
    git clone git://github.com/pocoproject/poco.git /home/vagrant/code/github/poco
    cd /home/vagrant/code/github/poco
    ./configure --prefix=/usr --omit=Data/ODBC
    cd
    sudo chown -R vagrant:vagrant /home/vagrant/code/github/poco
fi
# Note: project will not successfully compile if it's path contains symbolic links
cp -Rp /home/vagrant/code/github/poco /home/vagrant/poco
cd /home/vagrant/poco
make -s
if [ $? -eq 0 ]; then
    sudo make -s install
    if [ $? -eq 0 ]; then
        # if the build was successful, and the install completed, then go ahead and remove the source dir
        # in the vagrant user home, but leave the source repository
        cd
        sudo rm -rf /home/vagrant/poco
        echo "POCO install successfu, directory removed"
    else
        cd
        echo "POCO make install failed, directory not removed so the build failure can be checked"
    fi
else
    cd
    echo "POCO make failed, directory not removed so the build failure can be checked"
fi

################################################################################
# Cleanup output
################################################################################
set +x

################################################################################
# Next Steps
################################################################################
echo "################################################################################"
echo "# Finished!"
echo "# Next steps:"
echo "# $ vagrant ssh"
echo "# $ cd code/"
echo "# $ git clone git@github.com:you/your_awesome_project.git"
echo "# start developing"
echo "#"
echo "# Good luck!"
echo "################################################################################"
