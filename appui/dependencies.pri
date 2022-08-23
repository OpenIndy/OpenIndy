
include($$PWD/../lib/OpenIndy-Core/include.pri)

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

#-----------------
#copy dependencies
#-----------------
win32 {
# openIndyCore
CONFIG(release, debug|release) {
    openIndyCore.path = $$PWD/../install/release
    openIndyCore.files += $$PWD/../lib/OpenIndy-Core/install/release/openIndyCore1.dll
} else {
    openIndyCore.path = $$PWD/../install/debug
    openIndyCore.files += $$PWD/../lib/OpenIndy-Core/install/debug/openIndyCore1.dll
}
INSTALLS += openIndyCore

# openIndyMath
CONFIG(release, debug|release) {
    openIndyMath.path = $$PWD/../install/release
    openIndyMath.files += $$PWD/../lib/OpenIndy-Core/lib/OpenIndy-Math/install/release/openIndyMath1.dll
} else {
    openIndyMath.path = $$PWD/../install/debug
    openIndyMath.files += $$PWD/../lib/OpenIndy-Core/lib/OpenIndy-Math/install/debug/openIndyMath1.dll
}
INSTALLS += openIndyMath
}

linux {
# openIndyCore
CONFIG(release, debug|release) {
    openIndyCore.path = $$PWD/../install/release
    openIndyCore.files += $$PWD/../lib/OpenIndy-Core/install/release/libopenIndyCore.so*
} else {
    openIndyCore.path = $$PWD/../install/debug
    openIndyCore.files += $$PWD/../lib/OpenIndy-Core/install/debug/libopenIndyCore.so*
}
INSTALLS += openIndyCore

# openIndyMath
CONFIG(release, debug|release) {
    openIndyMath.path = $$PWD/../install/release
    openIndyMath.files += $$PWD/../lib/OpenIndy-Core/lib/OpenIndy-Math/install/release/libopenIndyMath.so*
} else {
    openIndyMath.path = $$PWD/../install/debug
    openIndyMath.files += $$PWD/../lib/OpenIndy-Core/lib/OpenIndy-Math/install/debug/libopenIndyMath.so*
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
    res_rt.files += $$PWD/../res-rt/config/*
    res_rt.files += $$PWD/../res-rt/example/*
    res_rt.files += $$PWD/../res-rt/plugins/*
} else {
    res_rt.path = $$PWD/../install/debug
    res_rt.files += $$PWD/../res-rt/config/*
    res_rt.files += $$PWD/../res-rt/example/*
    res_rt.files += $$PWD/../res-rt/plugins/*
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

linux {
    CONFIG(debug, debug|release) {
        LIBS += -L$$PWD/../lib/OpenIndy-Core/lib/OpenIndy-Math/bin/debug -lopenIndyMath
        LIBS += -L$$PWD/../lib/OpenIndy-Core/bin/debug -lopenIndyCore
    } else {
        LIBS += -L$$PWD/../lib/OpenIndy-Core/lib/OpenIndy-Math/bin/release -lopenIndyMath
        LIBS += -L$$PWD/../lib/OpenIndy-Core/bin/release -lopenIndyCore
    }

}
