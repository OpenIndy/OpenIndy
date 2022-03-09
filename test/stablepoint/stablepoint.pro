#-------------------------------------------------
#
# Project created by QtCreator 2020-01-28T14:06:30
#
#-------------------------------------------------
CONFIG   += c++11

QT       += testlib xml

QT       -= gui

TARGET = stablepointtest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += stablepointtest.cpp \
    mocksensor.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

CONFIG(debug, debug|release) {
    BUILD_DIR=debug
} else {
    BUILD_DIR=release
}

QMAKE_EXTRA_TARGETS += run-test
win32{
run-test.commands = $$shell_path($$OUT_PWD/$$BUILD_DIR/$$TARGET) -o $$shell_path(../reports/$${TARGET}.xml),xml
}else:linux{
run-test.commands = $$shell_path($$OUT_PWD/$$TARGET) -o $$shell_path(../reports/$${TARGET}.xml),xml
}



# test dependencies
include($$PWD/../../build/includes.pri)
include($$PWD/../../lib/OpenIndy-Core/build/includes.pri)
include($$PWD/../../lib/OpenIndy-Core/lib/OpenIndy-Math/build/includes.pri)

# test dependencies
SOURCES += \
    $$PWD/../../controller/stablepointlogic.cpp

# test dependencies
HEADERS +=  \
    $$PWD/../../controller/stablepointlogic.h \
    mocksensor.h

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

