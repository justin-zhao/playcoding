#!/bin/bash


grep "NAME=" /etc/os-release |grep "CentOS"
if [ $? == 0 ]; then
#    sudo yum update
	sudo yum install gtk3-devel.x86_64 ncurses-devel.x86_64 -y
else
#    sudo apt-get update
    sudo apt-get install gtk+-3.0 libncurses5-dev libncursesw5-dev -y
fi
