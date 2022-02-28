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
# test dependencies
SOURCES += \
    $$PWD/../../controller/featuresorter.cpp \
    $$PWD/../../controller/config/featuretablesortingconfig.cpp

# test dependencies
HEADERS +=  \
    $$PWD/../../controller/featuresorter.h \
    $$PWD/../../controller/config/featuretablesortingconfig.h

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

QMAKE_EXTRA_TARGETS += run-test
#run-test.commands = \
#    echo %PATH% & \
#    echo %QT_PATH% & \
#    set PATH=%PATH%;%QT_PATH%;$$shell_path($$OUT_PWD/../../lib/OpenIndy-Core/lib/OpenIndy-Math/lib/armadillo-3.910.0/examples/lib_win64);$$shell_path($$OUT_PWD/../../lib/OpenIndy-Core/lib/OpenIndy-Math/bin/$$BUILD_DIR);$$shell_path($$OUT_PWD/../../lib/OpenIndy-Core/bin/$$BUILD_DIR) && \
#    echo %PATH% &
run-test.commands = \
   $$shell_path($$OUT_PWD/$$BUILD_DIR/$$TARGET) -o $$shell_path(../reports/$${TARGET}.xml),xunitxml
