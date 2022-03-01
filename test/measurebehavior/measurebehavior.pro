#-------------------------------------------------
#
# Project created by QtCreator 2020-01-28T14:06:30
#
#-------------------------------------------------
CONFIG   += c++11

QT       += testlib xml widgets gui serialport network sql websockets multimedia

TARGET = measurebehaviortest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += measurebehaviortest.cpp \
    testcontrollersensoractions.cpp \
    testmeasurebehaviorlogic.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

include($$PWD/../../build/includes.pri)
include($$PWD/../../lib/OpenIndy-Core/build/includes.pri)
include($$PWD/../../lib/OpenIndy-Core/lib/OpenIndy-Math/build/includes.pri)


CONFIG(debug, debug|release) {
    BUILD_DIR=debug
} else {
    BUILD_DIR=release
}


linux-g++ {
LIBS += \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*measurebehaviordialog.o \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*measurebehaviorlogic.o

LIBS += \
    -L$$PWD/../../lib/OpenIndy-Core/bin/$$BUILD_DIR -lopenIndyCore
# \
#     -L$$PWD/../../lib/OpenIndy-Core/lib/OpenIndy-Math/bin/$$BUILD_DIR -lopenIndyMath  \
#    -L$$PWD/../../lib/s3dFitting/bin/$$BUILD_DIR -ls3dFitting

} else:win32-g++ {
LIBS += \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*measurebehaviordialog.o \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*measurebehaviorlogic.o

LIBS += \
    -L$$PWD/../../lib/OpenIndy-Core/bin/$$BUILD_DIR -lopenIndyCore1
# \
#    -L$$PWD/../../lib/OpenIndy-Core/lib/OpenIndy-Math/bin/$$BUILD_DIR -lopenIndyMath1 \
#    -L$$PWD/../../lib/s3dFitting/bin/$$BUILD_DIR -ls3dFitting1

} else:win32 {

# the most but not main.obj
LIBS += \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*measurebehaviordialog.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*measurebehaviorlogic.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*controllersensoractions.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*aboutdialog.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*activefeaturefunctionsmodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*actualpropertiesdialog.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*availableelementstreeviewproxymodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*availablefunctionslistmodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*availablefunctionslistproxymodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*bundlegeometriesmodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*bundlegeometryitem.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*bundleparametertableproxymodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*bundlestationitem.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*bundlestationsmodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*bundlesystemsmodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*bundletemplatesmodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*clipboardutil.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*console.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*controller.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*createfeaturedialog.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*dataexchanger.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*exportdialog.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*featuredifferenceproxymodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*featuredifferencetablemodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*featurefunctionsdialog.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*featuresorter.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*featuretablecolumnconfig.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*featuretabledelegate.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*featuretablemodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*featuretableproxymodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*featuretablesortingconfig.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*featuretreeitem.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*featuretreeviewmodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*featureupdater.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*functionstatisticmodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*functiontablemodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*functiontableproxymodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*functiontreeitem.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*functionweightdelegate.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*functionweightproxymodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*functionweightstablemodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*importnominaldialog.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*loadingdialog.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*mainwindow.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*measurementconfigmanager.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*measurementconfigurationdialog.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*measurementconfigurationlistdelegate.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*measurementconfigurationmodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*measurementconfigurationproxymodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*modelmanager.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*movesensordialog.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*multiselectionmodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*nominalpropertiesdialog.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*observationimporter.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*observationmodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*observationproxymodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*observationtablecolumnconfig.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*observationtabledelegate.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*oimetadata.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*oirequesthandler.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*oiwebsocket.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*oiwebsocketserver.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*oiwebsocketstreamer.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*parameterdisplayconfig.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*plugincopier.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*plugininfowidget.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*pluginloader.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*pluginloaderdialog.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*pluginmanagerdialog.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*plugintreeitem.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*plugintreeviewmodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*projectconfig.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*projectexchanger.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*readingmodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*readingproxymodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*readingtablecolumnconfig.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*readingtabledelegate.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*scalarparameterwidget.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*sensoraccuracydelegate.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*sensoraccuracymodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*sensorconfigurationdialog.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*sensorconfigurationlistdelegate.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*sensorconfigurationmanager.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*sensorconfigurationmodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*sensorconfigurationproxymodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*sensorparametersdelegate.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*sensorparametersmodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*sensortablemodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*sensortableproxymodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*sensortaskinfodialog.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*settingsdialog.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*showlicensesdialog.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*simplepluginloader.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*sortlistbyname.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*stablepointlogic.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*stationpropertiesdialog.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*systemdbmanager.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*toolaction.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*trafocontroller.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*trafoparampropertiesdialog.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*trafoparamtablecolumnconfig.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*trafoparamtabledelegate.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*trafoparamtableproxymodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*uiutil.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*usedelementsmodel.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*watchwindowdialog.obj \
    $$PWD/../../bin/$$BUILD_DIR/.obj/*watchwindowutil.obj

LIBS += \
    -L$$PWD/../../lib/OpenIndy-Core/bin/$$BUILD_DIR -lopenIndyCore1
# \
#    -L$$PWD/../../lib/OpenIndy-Core/lib/OpenIndy-Math/bin/$$BUILD_DIR -lopenIndyMath1 \
#    -L$$PWD/../../lib/s3dFitting/bin/$$BUILD_DIR -ls3dFitting1
}


# test dependencies as libraries
linux-g++ {

LIBS += \
    -L$$PWD/../../lib/OpenIndy-Core/bin/$$BUILD_DIR -lopenIndyCore \
    -L$$PWD/../../lib/OpenIndy-Core/lib/OpenIndy-Math/bin/$$BUILD_DIR -lopenIndyMath

} else : _win32-g++ {

LIBS += \
    -L$$PWD/../../lib/OpenIndy-Core/bin/$$BUILD_DIR -lopenIndyCore1 \
    -L$$PWD/../../lib/OpenIndy-Core/lib/OpenIndy-Math/bin/$$BUILD_DIR -lopenIndyMath1

} else : win32 {

LIBS += \
    -L$$PWD/../../lib/OpenIndy-Core/bin/$$BUILD_DIR -lopenIndyCore1 \
    -L$$PWD/../../lib/OpenIndy-Core/lib/OpenIndy-Math/bin/$$BUILD_DIR -lopenIndyMath1
}

win32 {
# x86_64
    contains(QMAKE_HOST.arch, x86_64) {
LIBS += \
    -L$$PWD/../../lib/OpenIndy-Core/lib/OpenIndy-Math/lib/armadillo-3.910.0/examples/lib_win64 -lblas_win64_MT \
    -L$$PWD/../../lib/OpenIndy-Core/lib/OpenIndy-Math/lib/armadillo-3.910.0/examples/lib_win64 -llapack_win64_MT
    } else {
# x86_32
LIBS += \
    -L$$PWD/../../lib/OpenIndy-Core/lib/OpenIndy-Math/lib/armadillo-3.910.0/examples/lib_win32 -lblas_win32_MT \
    -L$$PWD/../../lib/OpenIndy-Core/lib/OpenIndy-Math/lib/armadillo-3.910.0/examples/lib_win32 -llapack_win32_MT

    }
}

HEADERS += \
    testcontrollersensoractions.h \
    testmeasurebehaviorlogic.h

QMAKE_EXTRA_TARGETS += run-test
run-test.commands = \
   $$shell_path($$OUT_PWD/$$BUILD_DIR/$$TARGET) -o $$shell_path(../reports/$${TARGET}.xml),xml
