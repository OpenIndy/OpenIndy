# OpenIndy on ubuntu

## ubuntu 16.04 
   
    wget http://releases.ubuntu.com/16.04/ubuntu-16.04.6-desktop-i386.iso
    # wget http://releases.ubuntu.com/16.04/ubuntu-16.04.6-desktop-amd64.iso

## ubunut 16.04 prepare

    sudo apt install wget 
    sudo apt install git

    sudo apt install libgl1-mesa-dev
    sudo apt install libglu1-mesa-dev

    sudo apt install libblas-dev
    sudo apt install liblapack-dev
    sudo apt install libarmadillo-dev
  
## Qt 5.5.1

    wget http://download.qt.io/archive/qt/5.5/5.5.1/qt-opensource-linux-x86-5.5.1.run 
    chmod +x qt-opensource-linux-x86-5.5.1.run 
    ./qt-opensource-linux-x86-5.5.1.run


    # git clone --recurse-submodule https://github.com/OpenIndy/OpenIndy.git
    git clone --single-branch --branch feature/OI-482 --recurse-submodule https://github.com/OpenIndy/OpenIndy.git
    cd OpenIndy
    git submodule foreach --recursive git checkout feature/OI-482 

    #git clone --recurse-submodule https://github.com/OpenIndy/OpenIndy-DefaultPlugin.git

## run

Copy content of OpenIndy/res-rt to bin/debug and start openIndy from QT Creator. 
