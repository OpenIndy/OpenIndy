#-------------------------------------------------
#
# Project created by QtCreator 2020-01-28T14:06:30
#
#-------------------------------------------------
CONFIG   += c++11

QT       += testlib xml

QT       -= gui

TARGET = featuresortertest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += featuresortertest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"


# test dependencies
INCLUDEPATH += \
    $$PWD/../../controller \
    $$PWD/../../controller/config \
    $$PWD/../../lib/OpenIndy-Core/include/util \
    $$PWD/../../lib/OpenIndy-Core/include \
    $$PWD/../../lib/OpenIndy-Core/lib/OpenIndy-Math/include \
    $$PWD/../../lib/OpenIndy-Core/include/geometry \
    $$PWD/../../lib/OpenIndy-Core/include/plugin/ \
    $$PWD/../../lib/OpenIndy-Core/include/plugin/function \
    $$PWD/../../lib/OpenIndy-Core/include/plugin/simulation \
    $$PWD/../../lib/OpenIndy-Core/include/plugin/sensor

CONFIG(debug, debug|release) {
    BUILD_DIR=debug
} else {
    BUILD_DIR=release
}

#INCLUDEPATH += \
#    $$PWD/../..

# test dependencies as libraries
linux-g++ {

LIBS += \
    -L$$PWD/../../lib/OpenIndy-Core/bin/$$BUILD_DIR -lopenIndyCore \
    -L$$PWD/../../lib/OpenIndy-Core/lib/OpenIndy-Math/bin/$$BUILD_DIR -lopenIndyMath

} else : _win32-g++ {

LIBS += \
    -L$$PWD/../../lib/OpenIndy-Core/bin/$$BUILD_DIR -lopenIndyCore1 \
    -L$$PWD/../../lib/OpenIndy-Core/lib/OpenIndy-Math/bin/$$BUILD_DIR -lopenIndyMath1

} else : win32 {

LIBS += \
    -L$$PWD/../../lib/OpenIndy-Core/bin/$$BUILD_DIR -lopenIndyCore1 \
    -L$$PWD/../../lib/OpenIndy-Core/lib/OpenIndy-Math/bin/$$BUILD_DIR -lopenIndyMath1
}

win32 {
# x86_64
    contains(QMAKE_HOST.arch, x86_64) {
LIBS += \
    -L$$PWD/../../lib/OpenIndy-Core/lib/OpenIndy-Math/lib/armadillo-3.910.0/examples/lib_win64 -lblas_win64_MT \
    -L$$PWD/../../lib/OpenIndy-Core/lib/OpenIndy-Math/lib/armadillo-3.910.0/examples/lib_win64 -llapack_win64_MT
    } else {
# x86_32
LIBS += \
    -L$$PWD/../../lib/OpenIndy-Core/lib/OpenIndy-Math/lib/armadillo-3.910.0/examples/lib_win32 -lblas_win32_MT \
    -L$$PWD/../../lib/OpenIndy-Core/lib/OpenIndy-Math/lib/armadillo-3.910.0/examples/lib_win32 -llapack_win32_MT

    }
}

