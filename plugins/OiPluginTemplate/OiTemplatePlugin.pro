#-------------------------------------------------
#
# Project created by QtCreator 2013-10-12T15:43:14
#
#-------------------------------------------------

QT       -= gui
QT += serialport

TEMPLATE = lib
TARGET = $$qtLibraryTarget(p_example)

CONFIG += plugin

DEFINES += DEFAULT_LIBRARY

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/bin/debug
} else {
    DESTDIR = $$PWD/bin/release
}

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/openIndyLib/bin/release/ -lopenIndyLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/openIndyLib/bin/debug/ -lopenIndyLib
else:unix: LIBS += -L$$PWD/../../lib/openIndyLib/bin/debug -lopenIndyLib

INCLUDEPATH += -L$$PWD/../../lib/openIndyLib/bin/debug
DEPENDPATH += -L$$PWD/../../lib/openIndyLib/bin/debug

INCLUDEPATH += -L$$PWD/../../lib/openIndyLib/bin/release
DEPENDPATH += -L$$PWD/../../lib/openIndyLib/bin/release

INCLUDEPATH += $$PWD
INCLUDEPATH += ../../src
INCLUDEPATH += ../../src/geometry
INCLUDEPATH += ../../src/plugin
INCLUDEPATH += ../../lib/openIndyLib/include

SOURCES += p_factory.cpp \
    ../../src/configuration.cpp \
    ../../src/connectionconfig.cpp \
    ../../src/console.cpp \
    ../../src/coordinatesystem.cpp \
    ../../src/feature.cpp \
    ../../src/function.cpp \
    ../../src/functionconfiguration.cpp \
    ../../src/geometry.cpp \
    ../../src/measurementconfig.cpp \
    ../../src/observation.cpp \
    ../../src/oiemitter.cpp \
    ../../src/oimetadata.cpp \
    ../../src/pluginmetadata.cpp \
    ../../src/reading.cpp \
    ../../src/residual.cpp \
    ../../src/sensorconfiguration.cpp \
    ../../src/sensorcontrol.cpp \
    ../../src/station.cpp \
    ../../src/statistic.cpp \
    ../../src/trafoparam.cpp \
    ../../src/unitconverter.cpp \
    ../../src/geometry/circle.cpp \
    ../../src/geometry/cone.cpp \
    ../../src/geometry/cylinder.cpp \
    ../../src/geometry/ellipsoid.cpp \
    ../../src/geometry/hyperboloid.cpp \
    ../../src/geometry/line.cpp \
    ../../src/geometry/nurbs.cpp \
    ../../src/geometry/paraboloid.cpp \
    ../../src/geometry/plane.cpp \
    ../../src/geometry/point.cpp \
    ../../src/geometry/pointcloud.cpp \
    ../../src/geometry/scalarentityangle.cpp \
    ../../src/geometry/scalarentitydistance.cpp \
    ../../src/geometry/scalarentitymeasurementseries.cpp \
    ../../src/geometry/scalarentitytemperature.cpp \
    ../../src/geometry/sphere.cpp

HEADERS += p_factory.h \
    ../../lib/openIndyLib/include/global.h \
    ../../lib/openIndyLib/include/linearalgebra.h \
    ../../lib/openIndyLib/include/oimat.h \
    ../../lib/openIndyLib/include/oivec.h \
    ../../src/configuration.h \
    ../../src/connectionconfig.h \
    ../../src/console.h \
    ../../src/coordinatesystem.h \
    ../../src/element.h \
    ../../src/feature.h \
    ../../src/function.h \
    ../../src/functionconfiguration.h \
    ../../src/geometry.h \
    ../../src/measurementconfig.h \
    ../../src/observation.h \
    ../../src/oiemitter.h \
    ../../src/oimetadata.h \
    ../../src/pluginmetadata.h \
    ../../src/reading.h \
    ../../src/residual.h \
    ../../src/sensor.h \
    ../../src/sensorconfiguration.h \
    ../../src/sensorcontrol.h \
    ../../src/station.h \
    ../../src/statistic.h \
    ../../src/trafoparam.h \
    ../../src/unitconverter.h \
    ../../src/geometry/circle.h \
    ../../src/geometry/cone.h \
    ../../src/geometry/cylinder.h \
    ../../src/geometry/ellipsoid.h \
    ../../src/geometry/hyperboloid.h \
    ../../src/geometry/line.h \
    ../../src/geometry/nurbs.h \
    ../../src/geometry/paraboloid.h \
    ../../src/geometry/plane.h \
    ../../src/geometry/point.h \
    ../../src/geometry/pointcloud.h \
    ../../src/geometry/scalarentityangle.h \
    ../../src/geometry/scalarentitydistance.h \
    ../../src/geometry/scalarentitymeasurementseries.h \
    ../../src/geometry/scalarentitytemperature.h \
    ../../src/geometry/sphere.h \
    ../../src/plugin/pi_constructfunction.h \
    ../../src/plugin/pi_fitfunction.h \
    ../../src/plugin/pi_geodeticfunction.h \
    ../../src/plugin/pi_lasertracker.h \
    ../../src/plugin/pi_networkadjustment.h \
    ../../src/plugin/pi_objecttransformation.h \
    ../../src/plugin/pi_oiplugin.h \
    ../../src/plugin/pi_systemtransformation.h \
    ../../src/plugin/pi_totalstation.h

OTHER_FILES += metaInfo.json
