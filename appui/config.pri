
CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/../bin/debug
} else {
    DESTDIR = $$PWD/../bin/release
}

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui
