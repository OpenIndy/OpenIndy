#-------------------------------------------------
#
# Project created by QtCreator 2014-04-01T14:17:09
#
#-------------------------------------------------

QT += testlib
QT += core
QT -= gui
QT += serialport
CONFIG += testcase

TARGET = tst_plugindebuggertest
CONFIG   += console
CONFIG   -= app_bundle

DEFINES += WITH_FITTING_UTILS

TEMPLATE = app

INCLUDEPATH += ..
INCLUDEPATH += ../../../src
INCLUDEPATH += ../../../src/geometry
INCLUDEPATH += ../../../src/plugin
INCLUDEPATH += ../../../lib/openIndyLib/include/

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/bin/debug
} else {
    DESTDIR = $$PWD/bin/release
}

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../lib/openIndyLib/bin/release/ -lopenIndyLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../lib/openIndyLib/bin/debug/ -lopenIndyLib
else:unix: CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../lib/openIndyLib/bin/debug -lopenIndyLib
else:unix: CONFIG(release, debug|release): LIBS += -L$$PWD/../../../lib/openIndyLib/bin/release -lopenIndyLib

INCLUDEPATH += $$PWD/../../../lib/openIndyLib/bin/debug
DEPENDPATH += $$PWD/../../../lib/openIndyLib/bin/debug

INCLUDEPATH += $$PWD/../../../lib/openIndyLib/bin/release
DEPENDPATH += $$PWD/../../../lib/openIndyLib/bin/release

DEFINES += SRCDIR=\\\"$$PWD/\\\"

SOURCES += \
    main.cpp \
    oitestYourPluginClass.cpp \
    ../../../src/unitconverter.cpp \
    ../../../src/trafoparam.cpp \
    ../../../src/statistic.cpp \
    ../../../src/station.cpp \
    ../../../src/sensorcontrol.cpp \
    ../../../src/sensorconfiguration.cpp \
    ../../../src/residual.cpp \
    ../../../src/reading.cpp \
    ../../../src/pluginmetadata.cpp \
    ../../../src/oimetadata.cpp \
    ../../../src/oiemitter.cpp \
    ../../../src/observation.cpp \
    ../../../src/measurementconfig.cpp \
    ../../../src/geometry.cpp \
    ../../../src/functionconfiguration.cpp \
    ../../../src/function.cpp \
    ../../../src/featurewrapper.cpp \
    ../../../src/feature.cpp \
    ../../../src/elementdependencies.cpp \
    ../../../src/coordinatesystem.cpp \
    ../../../src/connectionconfig.cpp \
    ../../../src/configuration.cpp \
    ../../../src/geometry/sphere.cpp \
    ../../../src/geometry/scalarentitytemperature.cpp \
    ../../../src/geometry/scalarentitymeasurementseries.cpp \
    ../../../src/geometry/scalarentitydistance.cpp \
    ../../../src/geometry/scalarentityangle.cpp \
    ../../../src/geometry/pointcloud.cpp \
    ../../../src/geometry/point.cpp \
    ../../../src/geometry/plane.cpp \
    ../../../src/geometry/paraboloid.cpp \
    ../../../src/geometry/nurbs.cpp \
    ../../../src/geometry/line.cpp \
    ../../../src/geometry/hyperboloid.cpp \
    ../../../src/geometry/ellipsoid.cpp \
    ../../../src/geometry/cylinder.cpp \
    ../../../src/geometry/cone.cpp \
    ../../../src/geometry/circle.cpp


HEADERS += \
    AutoTest.h \
    ../lib/openIndyLib/include/oivec.h \
    ../lib/openIndyLib/include/oimat.h \
    ../lib/openIndyLib/include/linearalgebra.h \
    ../lib/openIndyLib/include/global.h \
    ../lib/openIndyLib/include/chooselalib.h \
    oitestYourPluginClass.h \
    ../../../src/unitconverter.h \
    ../../../src/trafoparam.h \
    ../../../src/statistic.h \
    ../../../src/station.h \
    ../../../src/sensorcontrol.h \
    ../../../src/sensorconfiguration.h \
    ../../../src/sensor.h \
    ../../../src/residual.h \
    ../../../src/reading.h \
    ../../../src/pluginmetadata.h \
    ../../../src/oimetadata.h \
    ../../../src/oiemitter.h \
    ../../../src/observation.h \
    ../../../src/measurementconfig.h \
    ../../../src/geometry.h \
    ../../../src/functionconfiguration.h \
    ../../../src/function.h \
    ../../../src/featurewrapper.h \
    ../../../src/feature.h \
    ../../../src/elementdependencies.h \
    ../../../src/element.h \
    ../../../src/coordinatesystem.h \
    ../../../src/connectionconfig.h \
    ../../../src/configuration.h \
    ../../../src/geometry/sphere.h \
    ../../../src/geometry/scalarentitytemperature.h \
    ../../../src/geometry/scalarentitymeasurementseries.h \
    ../../../src/geometry/scalarentitydistance.h \
    ../../../src/geometry/scalarentityangle.h \
    ../../../src/geometry/pointcloud.h \
    ../../../src/geometry/point.h \
    ../../../src/geometry/plane.h \
    ../../../src/geometry/paraboloid.h \
    ../../../src/geometry/nurbs.h \
    ../../../src/geometry/line.h \
    ../../../src/geometry/hyperboloid.h \
    ../../../src/geometry/ellipsoid.h \
    ../../../src/geometry/cylinder.h \
    ../../../src/geometry/cone.h \
    ../../../src/geometry/circle.h \
    ../../../src/plugin/pi_totalstation.h \
    ../../../src/plugin/pi_systemtransformation.h \
    ../../../src/plugin/pi_oiplugin.h \
    ../../../src/plugin/pi_objecttransformation.h \
    ../../../src/plugin/pi_networkadjustment.h \
    ../../../src/plugin/pi_lasertracker.h \
    ../../../src/plugin/pi_geodeticfunction.h \
    ../../../src/plugin/pi_fitfunction.h \
    ../../../src/plugin/pi_constructfunction.h

