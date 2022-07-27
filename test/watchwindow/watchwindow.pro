#-------------------------------------------------
#
# Project created by QtCreator 2020-01-28T14:06:30
#
#-------------------------------------------------
CONFIG   += c++11

QT       += testlib xml

QT       -= gui

TARGET = tst_watchwindowtest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_watchwindowtest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"


# openIndy.dll
include($$PWD/../../build/dependencies.pri)
# openIndyCore.dll
include($$PWD/../../appui/dependencies.pri)


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
