
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

# splash screen
win32 {

    win32:CONFIG(release, debug|release): splash.path = $$PWD/../install/release
    win32:CONFIG(debug, debug|release): splash.path = $$PWD/../install/debug

    splash.files += $$PWD/../res/icons/splash.png

    INSTALLS += splash

}

linux {

    linux:CONFIG(release, debug|release): splash.path = $$PWD/../install/release
    linux:CONFIG(debug, debug|release): splash.path = $$PWD/../install/debug

    splash.files += $$PWD/../res/icons/splash.png

    INSTALLS += splash

}

win32 {
    CONFIG(debug, debug|release) {
        LIBS += -L$$PWD/../appui/bin/debug -lopenIndy23
    } else {
        LIBS += -L$$PWD/../appui/bin/release -lopenIndy23
    }
}
linux {
    CONFIG(debug, debug|release) {
        LIBS += -L$$PWD/../appui/bin/debug -lopenIndy23
    } else {
        LIBS += -L$$PWD/../appui/bin/release -lopenIndy23
    }
}
