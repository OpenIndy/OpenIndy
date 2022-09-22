#
CONFIG   += c++11

QT       += testlib xml widgets serialport network sql

QT       -= gui

TARGET = dialogstest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

# openIndy.dll
include(../../build/dependencies.pri)
# openIndyCore.dll
include(../../appui/dependencies.pri)

# OpenIndy/db/oisystemdb.sql
DEFINES += INIT_SQL=\\\"../../db/oisystemdb.sql\\\"
DEFINES += ELEMENT_SQL=\\\"../../db/element.sql\\\"
SOURCES += dialogstest.cpp

DEFINES += SRCDIR=$$shell_quote($$PWD)

CONFIG(debug, debug|release) {
    BUILD_DIR=debug
} else {
    BUILD_DIR=release
}

QMAKE_EXTRA_TARGETS += run-test
win32{
run-test.commands = $$shell_quote$$OUT_PWD/$$BUILD_DIR/$$TARGET) -o $$shell_path(../reports/$${TARGET}.xml),xml
}else:linux{
run-test.commands = $$shell_quote$$OUT_PWD/$$TARGET) -o $$shell_path(../reports/$${TARGET}.xml),xml
}
