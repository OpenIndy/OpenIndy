# OpenIndy on ubuntu

## ubuntu 18.04 
   
    https://ubuntu.com/download/desktop - Ubuntu 18.04.3 LTS

## ubuntu 18.04 prepare

    sudo apt install git

    sudo apt install libgl1-mesa-dev
    sudo apt install libglu1-mesa-dev

    sudo apt install libblas-dev
    sudo apt install liblapack-dev
    sudo apt install libarmadillo-dev
  
## Qt 5.9.5 shipped with ubuntu 18.04

    sudo apt install build-essential
    sudo apt install qt5-default
    sudo apt install libqt5svg5-dev libqt5websockets5-dev 
    sudo apt install qt5serialport5-dev
    sudo apt install libqt5multimedia5 libqt5multimedia5-plugins qtmultimedia5-dev
    sudo apt install qtcreator   

## clone OpenIndy

    git clone --single-branch --branch feature/OI-482 --recurse-submodule https://github.com/OpenIndy/OpenIndy.git
    cd OpenIndy
    git submodule foreach --recursive git checkout feature/OI-482


    qmake openIndy.pro -r -spec linux-g++ CONFIG+=debug
    make

## clone OpenIndy-DefaultPlugin

    git clone --single-branch --branch feature/OI-482 --recurse-submodule https://github.com/OpenIndy/OpenIndy-DefaultPlugin.git
    cd OpenIndy-DefaultPlugin
    git submodule foreach --recursive git checkout feature/OI-482


    qmake OpenIndy-DefaultPlugin.pro -r -spec linux-g++ CONFIG+=debug
    make

## run

Copy content of OpenIndy/res-rt to bin/debug and start openIndy from QT Creator. 

## install

    cd OpenIndy
    mkdir -p dist/lib
    cp bin/debug/openIndy dist/
    cp -R res-rt/* dist/
    find lib -name "libopenIndy*.so.*" -exec cp -v {} dist/lib/ \;

    # run
    ./dist/openIndy.sh

