#-------------------------------------------------
#
# Project created by QtCreator 2013-10-12T15:43:14
#
#-------------------------------------------------
CONFIG += c++11
QT       += core gui widgets
QT       += sql serialport network xml websockets

greaterThan(QT_MAJOR_VERSION, 4):

TARGET = openIndy
TEMPLATE = app

DEFINES += OI_MAIN_PROGRAM
#DEFINES -= QT_MAIN_LIB

# version number from git
include($$PWD/../appui/version.pri)

# external libraries
# openIndy.dll
include($$PWD/dependencies.pri)
# openIndyCore.dll
include($$PWD/../appui/dependencies.pri)

# source files
SOURCES += \
    $$PWD/../main.cpp

# app icon
win32:RC_FILE = $$PWD/../res/openIndy.rc
linux:ICON = $$PWD/../res/openIndy.icns
