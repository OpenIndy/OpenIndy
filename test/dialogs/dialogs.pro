#
CONFIG   += c++11

QT       += testlib xml widgets serialport network sql

QT       -= gui

TARGET = dialogstest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

# openIndy.dll
include($$PWD/../../build/dependencies.pri)
# openIndyCore.dll
include($$PWD/../../appui/dependencies.pri)

# OpenIndy/db/oisystemdb.sql
DEFINES += INIT_SQL=\\\"$$PWD/../../db/oisystemdb.sql\\\"
DEFINES += ELEMENT_SQL=\\\"$$PWD/../../db/element.sql\\\"
SOURCES += dialogstest.cpp

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
