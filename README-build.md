# OpenIndy on Windows with MinGW

## Ubuntu 18.04

### install dependencies

    sudo apt install git
    
    sudo apt install build-essential

    sudo apt install libgl1-mesa-dev
    sudo apt install libglu1-mesa-dev

    sudo apt install libblas-dev
    sudo apt install liblapack-dev
    sudo apt install libarmadillo-dev
  
### install Qt 5.9.5 shipped with ubuntu 18.04

    sudo apt install qt5-default
    sudo apt install libqt5svg5-dev libqt5websockets5-dev 
    sudo apt install libqt5serialport5-dev
    sudo apt install libqt5multimedia5 libqt5multimedia5-plugins qtmultimedia5-dev
    sudo apt install qtcreator   

### clone and build OpenIndy

    git clone --recurse-submodule https://github.com/OpenIndy/OpenIndy.git


    cd OpenIndy    
    qmake openIndy.pro -r -spec linux-g++ CONFIG+=debug
    make

### clone and build OpenIndy-DefaultPlugin

    git clone --recurse-submodule https://github.com/OpenIndy/OpenIndy-DefaultPlugin.git


    cd OpenIndy-DefaultPlugin
    qmake OpenIndy-DefaultPlugin.pro -r -spec linux-g++ CONFIG+=debug
    make

### run

Copy content of OpenIndy/res-rt to bin/debug and start openIndy from Qt Creator. 

### install

    cd OpenIndy
    mkdir -p dist/lib
    cp bin/debug/openIndy dist/
    cp -R res-rt/* dist/
    find lib -name "libopenIndy*.so.*" -exec cp -v {} dist/lib/ \;

    # run
    ./dist/openIndy.sh

## Windows & MinGW

### install Qt

install required  Qt / Qt Creator

### clone OpenIndy

    git clone  --recurse-submodule https://github.com/OpenIndy/OpenIndy.git  

### clone OpenIndy-DefaultPlugin

    git clone --recurse-submodule https://github.com/OpenIndy/OpenIndy-DefaultPlugin.git
    
### Qt Creator: build and run OpenIndy

Add `./lib/OpenIndy-Core/lib/OpenIndy-Math/lib/armadillo-3.910.0/examples/lib_win32` to Qt Creator PATH.
Copy content of OpenIndy/res-rt to bin/debug and start openIndy from Qt Creator. 

### install

    cd OpenIndy
    mkdir dist
    copy bin\debug\openIndy.exe dist\
    
    xcopy res-rt\*.* dist\ /s /e
    
    copy lib\OpenIndy-Core\bin\debug\openIndyCore1.dll dist\
    copy lib\OpenIndy-Core\lib\OpenIndy-Math\bin\debug\openIndyMath1.dll dist\
    copy lib\OpenIndy-Core\lib\OpenIndy-Math\lib\armadillo-3.910.0\examples\lib_win32\blas_win32_MT.dll dist\
    copy lib\OpenIndy-Core\lib\OpenIndy-Math\lib\armadillo-3.910.0\examples\lib_win32\lapack_win32_MT.dll dist\

    rem run    
    
    cd dist

    rem set `QT_DIR` to your Qt install path
    openIndy.bat
