
include($$PWD/../lib/OpenIndy-Core/include.pri)

#linux: LIBS = -lGLU

#-----------------
#copy dependencies
#-----------------

# blas / lapack
win32 {

    win32:CONFIG(release, debug|release): blaslapack.path = $$PWD/../bin/release
    win32:CONFIG(debug, debug|release): blaslapack.path = $$PWD/../bin/debug

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

    win32:CONFIG(release, debug|release): splash.path = $$PWD/../bin/release
    win32:CONFIG(debug, debug|release): splash.path = $$PWD/../bin/debug

    splash.files += $$PWD/../res/icons/splash.png

    INSTALLS += splash

}

unix {

    unix:CONFIG(release, debug|release): splash.path = $$PWD/../bin/release
    unix:CONFIG(debug, debug|release): splash.path = $$PWD/../bin/debug

    splash.files += $$PWD/../res/icons/splash.png

    INSTALLS += splash

}
