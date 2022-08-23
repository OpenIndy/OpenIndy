
include($$PWD/../appui/includes.pri)
DEPENDPATH  += $$PWD/../appui

linux: LIBS = -lGLU

# openIndy
win32 {
    CONFIG(release, debug|release) {
        target.path = $$PWD/../install/release
    } else {
        target.path = $$PWD/../install/debug
    }
    INSTALLS += target
}

linux {
    CONFIG(release, debug|release) {
        target.path = $$PWD/../install/release
    } else {
        target.path = $$PWD/../install/debug
    }
INSTALLS += target
}

win32 {
    CONFIG(debug, debug|release) {
        LIBS += -L$$PWD/../appui/bin/debug -lopenIndy22
    } else {
        LIBS += -L$$PWD/../appui/bin/release -lopenIndy22
    }
}
linux {
    CONFIG(debug, debug|release) {
        LIBS += -L$$PWD/../appui/bin/debug -lopenIndy22
    } else {
        LIBS += -L$$PWD/../appui/bin/release -lopenIndy22
    }
}
