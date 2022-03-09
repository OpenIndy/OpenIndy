#-------------------------------------------------
#
# Project created by QtCreator 2020-01-28T14:06:30
#
#-------------------------------------------------
CONFIG   += c++11

QT       += testlib xml widgets gui serialport network sql websockets multimedia

TARGET = measurebehaviortest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += measurebehaviortest.cpp \
    testcontrollersensoractions.cpp \
    testmeasurebehaviorlogic.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

include($$PWD/../../build/includes.pri)
include($$PWD/../../lib/OpenIndy-Core/build/includes.pri)
include($$PWD/../../lib/OpenIndy-Core/lib/OpenIndy-Math/build/includes.pri)


CONFIG(debug, debug|release) {
    BUILD_DIR=debug
} else {
    BUILD_DIR=release
}


linux-g++ {
LIBS += \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*measurebehaviordialog.o \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*measurebehaviorlogic.o \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*featuretablemodel.o \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*parameterdisplayconfig.o \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*systemdbmanager.o \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*console.o \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*oimetadata.o \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*measurementconfigmanager.o \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*pluginloader.o \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*plugincopier.o \


LIBS += \
    -L$$PWD/../../lib/OpenIndy-Core/bin/$$BUILD_DIR -lopenIndyCore
# \
#     -L$$PWD/../../lib/OpenIndy-Core/lib/OpenIndy-Math/bin/$$BUILD_DIR -lopenIndyMath  \
#    -L$$PWD/../../lib/s3dFitting/bin/$$BUILD_DIR -ls3dFitting

} else:win32-g++ {
LIBS += \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*measurebehaviordialog.o \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*measurebehaviorlogic.o \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*featuretablemodel.o \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*parameterdisplayconfig.o \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*systemdbmanager.o \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*console.o \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*oimetadata.o \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*measurementconfigmanager.o \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*pluginloader.o \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*plugincopier.o \


LIBS += \
    -L$$PWD/../../lib/OpenIndy-Core/bin/$$BUILD_DIR -lopenIndyCore1
# \
#    -L$$PWD/../../lib/OpenIndy-Core/lib/OpenIndy-Math/bin/$$BUILD_DIR -lopenIndyMath1 \
#    -L$$PWD/../../lib/s3dFitting/bin/$$BUILD_DIR -ls3dFitting1

} else:win32 {

# the most but not main.obj
LIBS += \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*measurebehaviordialog.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*measurebehaviorlogic.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*featuretablemodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*parameterdisplayconfig.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*systemdbmanager.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*console.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*oimetadata.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*measurementconfigmanager.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*pluginloader.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*plugincopier.obj \

LIBS += \
    -L$$PWD/../../lib/OpenIndy-Core/bin/$$BUILD_DIR -lopenIndyCore1
# \
#    -L$$PWD/../../lib/OpenIndy-Core/lib/OpenIndy-Math/bin/$$BUILD_DIR -lopenIndyMath1 \
#    -L$$PWD/../../lib/s3dFitting/bin/$$BUILD_DIR -ls3dFitting1
}


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

HEADERS += \
    testcontrollersensoractions.h \
    testmeasurebehaviorlogic.h

QMAKE_EXTRA_TARGETS += run-test
win32{
run-test.commands = $$shell_path($$OUT_PWD/$$BUILD_DIR/$$TARGET) -o $$shell_path(../reports/$${TARGET}.xml),xml
}else:linux{
run-test.commands = $$shell_path($$OUT_PWD/$$TARGET) -o $$shell_path(../reports/$${TARGET}.xml),xml
}
