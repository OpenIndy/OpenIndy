
include($$PWD/../lib/OpenIndy-Core/include.pri)

linux: LIBS = -lGLU

# openIndy
win32 {
CONFIG(release, debug|release) {
    openIndy.path = $$PWD/../install/release
    openIndy.files += $$PWD/../bin/release/openIndy.exe
} else {
    openIndy.path = $$PWD/../install/debug
    openIndy.files += $$PWD/../bin/debug/openIndy.exe
}
INSTALLS += openIndy
}

linux {
CONFIG(release, debug|release) {
    openIndy.path = $$PWD/../install/release
    openIndy.files += $$PWD/../bin/release/openIndy
} else {
    openIndy.path = $$PWD/../install/debug
    openIndy.files += $$PWD/../bin/debug/openIndy
}
INSTALLS += openIndy
}

#-----------------
#copy dependencies
#-----------------
win32 {
# openIndyCore
CONFIG(release, debug|release) {
    openIndyCore.path = $$PWD/../install/release
    openIndyCore.files += $$PWD/../lib/OpenIndy-Core/bin/release/openIndyCore1.dll
} else {
    openIndyCore.path = $$PWD/../install/debug
    openIndyCore.files += $$PWD/../lib/OpenIndy-Core/bin/debug/openIndyCore1.dll
}
INSTALLS += openIndyCore

# openIndyMath
CONFIG(release, debug|release) {
    openIndyMath.path = $$PWD/../install/release
    openIndyMath.files += $$PWD/../lib/OpenIndy-Core/lib/OpenIndy-Math/bin/release/openIndyMath1.dll
} else {
    openIndyMath.path = $$PWD/../install/debug
    openIndyMath.files += $$PWD/../lib/OpenIndy-Core/lib/OpenIndy-Math/bin/debug/openIndyMath1.dll
}
INSTALLS += openIndyMath
}

linux {
# openIndyCore
CONFIG(release, debug|release) {
    openIndyCore.path = $$PWD/../install/release
    openIndyCore.files += $$PWD/../lib/OpenIndy-Core/bin/release/libopenIndyCore.so*
} else {
    openIndyCore.path = $$PWD/../install/debug
    openIndyCore.files += $$PWD/../lib/OpenIndy-Core/bin/debug/libopenIndyCore.so*
}
INSTALLS += openIndyCore

# openIndyMath
CONFIG(release, debug|release) {
    openIndyMath.path = $$PWD/../install/release
    openIndyMath.files += $$PWD/../lib/OpenIndy-Core/lib/OpenIndy-Math/bin/release/libopenIndyMath.so*
} else {
    openIndyMath.path = $$PWD/../install/debug
    openIndyMath.files += $$PWD/../lib/OpenIndy-Core/lib/OpenIndy-Math/bin/debug/libopenIndyMath.so*
}
INSTALLS += openIndyMath
}

# oisystemdb
CONFIG(release, debug|release) {
    oisystemdb.path = $$PWD/../install/release
    oisystemdb.files += $$PWD/../db/oisystemdb.sqlite
} else {
    oisystemdb.path = $$PWD/../install/debug
    oisystemdb.files += $$PWD/../db/oisystemdb.sqlite
}
INSTALLS += oisystemdb

# res-rt
CONFIG(release, debug|release) {
    res_rt.path = $$PWD/../install/release
    res_rt.files += $$PWD/../res-rt/config
    res_rt.files += $$PWD/../res-rt/example
    res_rt.files += $$PWD/../res-rt/plugins
} else {
    res_rt.path = $$PWD/../install/debug
    res_rt.files += $$PWD/../res-rt/config
    res_rt.files += $$PWD/../res-rt/example
    res_rt.files += $$PWD/../res-rt/plugins
}
INSTALLS += res_rt

# blas / lapack
win32 {

    win32:CONFIG(release, debug|release): blaslapack.path = $$PWD/../install/release
    win32:CONFIG(debug, debug|release): blaslapack.path = $$PWD/../install/debug

    !contains(QT_ARCH, x86_64) {
        blaslapack.files += $$PWD/../lib/OpenIndy-Core/lib/OpenIndy-Math/lib/armadillo-3.910.0/examples/lib_win32/blas_win32_MT.dll
        blaslapack.files += $$PWD/../lib/OpenIndy-Core/lib/OpenIndy-Math/lib/armadillo-3.910.0/examples/lib_win32/lapack_win32_MT.dll
    } else {
        blaslapack.files += $$PWD/../lib/OpenIndy-Core/lib/OpenIndy-Math/lib/armadillo-3.910.0/examples/lib_win64/blas_win64_MT.dll
        blaslapack.files += $$PWD/../lib/OpenIndy-Core/lib/OpenIndy-Math/lib/armadillo-3.910.0/examples/lib_win64/lapack_win64_MT.dll
    }

    INSTALLS += blaslapack

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

linux {
    CONFIG(debug, debug|release) {
        LIBS += -L$$PWD/../lib/OpenIndy-Core/lib/OpenIndy-Math/bin/debug -lopenIndyMath
        LIBS += -L$$PWD/../lib/OpenIndy-Core/bin/debug -lopenIndyCore
    }
    CONFIG(release, debug|release) {
        LIBS += -L$$PWD/../lib/OpenIndy-Core/lib/OpenIndy-Math/bin/release -lopenIndyMath
        LIBS += -L$$PWD/../lib/OpenIndy-Core/bin/release -lopenIndyCore
    }

}
