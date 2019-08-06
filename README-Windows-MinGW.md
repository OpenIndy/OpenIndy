# OpenIndy on Windows with MinGW

## Qt 5.5.1

Download and install:

    http://download.qt.io/archive/qt/5.5/5.5.1/qt-opensource-windows-x86-mingw492-5.5.1.exe
 
## clone OpenIndy

    git clone --single-branch --branch feature/OI-482 --recurse-submodule https://github.com/OpenIndy/OpenIndy.git
    cd OpenIndy
    git submodule foreach --recursive git checkout feature/OI-482 

## clone OpenIndy-DefaultPlugin

    git clone --single-branch --branch feature/OI-482 --recurse-submodule https://github.com/OpenIndy/OpenIndy-DefaultPlugin.git
    cd OpenIndy-DefaultPlugin
    git submodule foreach --recursive git checkout feature/OI-482

## run

Copy content of OpenIndy/res-rt to bin/debug and start openIndy from QT Creator. 
