# OpenIndy

OpenIndy is a metrology software solution that can be extended by [plugins](https://github.com/OpenIndy/OiPluginTemplate). The project started in 2013 as a student project in the Department of Geoinformatics and Surveying ([HS Mainz](https://www.hs-mainz.de/)).

## IDE

OpenIndy is developed with the Qt framework (Qt libs + Qt Creator IDE). You can download the framework [here](http://qt-project.org/downloads).

## Dependencies

- [OpenIndy-Core](https://github.com/OpenIndy/OpenIndy-Core)
- [Qt](http://qt-project.org)
- [sqlite](https://sqlite.org)

## build

### Ubuntu

Tested with Ubuntu 18.04 (Qt 5.9.5), Ubuntu 20.04 (Qt 5.12.8)

#### install dependencies

    sudo apt install git

    sudo apt install build-essential

    sudo apt install libgl1-mesa-dev
    sudo apt install libglu1-mesa-dev

    sudo apt install libblas-dev
    sudo apt install liblapack-dev
    sudo apt install libarmadillo-dev

#### install Qt shipped with Ubuntu

    sudo apt install qt5-default
    sudo apt install libqt5svg5-dev libqt5websockets5-dev
    sudo apt install libqt5serialport5-dev
    sudo apt install libqt5multimedia5 libqt5multimedia5-plugins qtmultimedia5-dev

    sudo apt install qtcreator

#### clone and build OpenIndy

    git clone --recurse-submodule https://github.com/OpenIndy/OpenIndy.git


    cd OpenIndy
    qmake openIndy.pro -r -spec linux-g++ CONFIG+=debug
    make

#### clone and build OpenIndy-DefaultPlugin

    git clone --recurse-submodule https://github.com/OpenIndy/OpenIndy-DefaultPlugin.git


    cd OpenIndy-DefaultPlugin
    qmake OpenIndy-DefaultPlugin.pro -r -spec linux-g++ CONFIG+=debug
    make

#### run

Copy nessesary files to bin/debug and start openIndy from Qt Creator.

    cp -R res-rt/* bin/debug/
    cp db/oisystemdb.sqlite bin/debug/

### commandline: install & run

    cd OpenIndy
    mkdir -p dist/lib
    cp bin/debug/openIndy dist/
    cp -R res-rt/* dist/
    cp db/oisystemdb.sqlite dist/
    find lib -name "libopenIndy*.so.*" -exec cp -v {} dist/lib/ \;

    # run
    chmod +x ./dist/openIndy.sh
    ./dist/openIndy.sh

## Windows & MinGW

Tested with Qt 5.5.1 / MinGW 4.9.2

### install Qt

install required  Qt / Qt Creator

### clone and build OpenIndy

    git clone  --recurse-submodule https://github.com/OpenIndy/OpenIndy.git


    cd OpenIndy
    qmake.exe OpenIndy.pro -r -spec win32-g++ "CONFIG+=debug"
    mingw32-make.exe

### clone and build OpenIndy-DefaultPlugin

    git clone --recurse-submodule https://github.com/OpenIndy/OpenIndy-DefaultPlugin.git


    cd OpenIndy-DefaultPlugin
    qmake.exe OpenIndy-DefaultPlugin.pro -r -spec win32-g++ "CONFIG+=debug"
    mingw32-make.exe

### Qt Creator: build and run OpenIndy

Add `./lib/OpenIndy-Core/lib/OpenIndy-Math/lib/armadillo-3.910.0/examples/lib_win32` to Qt Creator PATH.
Copy content of OpenIndy/res-rt and db\oisystemdb.sqlite to bin/debug and start openIndy from Qt Creator.

### commandline: install & run

    cd OpenIndy
    mkdir dist
    copy bin\debug\openIndy.exe dist\

    xcopy res-rt\*.* dist\ /s /e
    copy db\oisystemdb.sqlite dist\

    copy lib\OpenIndy-Core\bin\debug\openIndyCore1.dll dist\
    copy lib\OpenIndy-Core\lib\OpenIndy-Math\bin\debug\openIndyMath1.dll dist\
    copy lib\OpenIndy-Core\lib\OpenIndy-Math\lib\armadillo-3.910.0\examples\lib_win32\blas_win32_MT.dll dist\
    copy lib\OpenIndy-Core\lib\OpenIndy-Math\lib\armadillo-3.910.0\examples\lib_win32\lapack_win32_MT.dll dist\

    rem run

    cd dist

    rem set `QT_DIR` to your Qt install path
    openIndy.bat
