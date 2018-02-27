#!/bin/bash

#
# Script to run C++ REST Template (crt)  by hand.  By default crt is started 
# by upstart.  This script is the "normal" way to start the process and place
# it in the background.  
# 
crt -r -l debug >> /var/log/crt.log 2>&1 &
