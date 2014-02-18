#-------------------------------------------------
#
# Project created by QtCreator 2013-09-18T14:25:09
#
#-------------------------------------------------

QT       += core sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = openIndyLib
TEMPLATE = lib

DEFINES += OI_LIB

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/bin/debug
} else {
    DESTDIR = $$PWD/bin/release
}

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/src
INCLUDEPATH += $$PWD/include

SOURCES += src/oivec.cpp \
    src/oimat.cpp \
    src/linearalgebra.cpp \
    src/laarmadillo.cpp \
    src/chooselalib.cpp

HEADERS  += include/global.h \
    src/types.h \
    include/oivec.h \
    include/oimat.h \
    include/linearalgebra.h \
    include/chooselalib.h \
    src/laarmadillo.h \
    ../armadillo-3.910.0/include/armadillo \


#----------------------------Specific Includes for linux---------------------------------

linux {
    LIBS += -L/usr/local/lib -lblas
    LIBS += -L/usr/local/lib -llapack
}

#----------------------------Specific Includes for mac os---------------------------------

#/Library/Frameworks
mac: LIBS +=  -framework Accelerate
#mac: LIBS += -llapack -lblas

#----------------------------Specific Includes for windows---------------------------------

win32 {

    ## Windows common build here

    !contains(QMAKE_HOST.arch, x86_64) {
        message("x86 build")

        ## Windows x86 (32bit) specific build here

LIBS += -L$$PWD/../armadillo-3.910.0/examples/lib_win32/ -lblas_win32_MT

INCLUDEPATH += $$PWD/../armadillo-3.910.0/examples/lib_win32
DEPENDPATH += $$PWD/../armadillo-3.910.0/examples/lib_win32

LIBS += -L$$PWD/../armadillo-3.910.0/examples/lib_win32/ -llapack_win32_MT

INCLUDEPATH += $$PWD/../armadillo-3.910.0/examples/lib_win32
DEPENDPATH += $$PWD/../armadillo-3.910.0/examples/lib_win32

    } else {
        message("x86_64 build")

LIBS += -L$$PWD/../armadillo-3.910.0/examples/lib_win64/ -lblas_win64_MT

INCLUDEPATH += $$PWD/../armadillo-3.910.0/examples/lib_win64
DEPENDPATH += $$PWD/../armadillo-3.910.0/examples/lib_win64

LIBS += -L$$PWD/../armadillo-3.910.0/examples/lib_win64/ -llapack_win64_MT

INCLUDEPATH += $$PWD/../armadillo-3.910.0/examples/lib_win64
DEPENDPATH += $$PWD/../armadillo-3.910.0/examples/lib_win64

    }
}

INCLUDEPATH += $$PWD/../armadillo-3.910.0/include
