#-------------------------------------------------
#
# Project created by QtCreator 2013-09-18T14:25:09
#
#-------------------------------------------------

QT       += core gui sql serialport network opengl xml websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = openIndy
TEMPLATE = app

DEFINES += OI_MAIN_PROGRAM

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/bin/debug
} else {
    DESTDIR = $$PWD/bin/release
}

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

RESOURCES += \
    res/res.qrc

win32:RC_FILE = $$PWD/res/openIndy.rc
unix:ICON = $$PWD/res/openIndy.icns

#-----------------------------------linux dependency---------------------------------------
linux: LIBS = -lGLU
#-----------------------------------Linear Algebra---------------------------------------

INCLUDEPATH += $$PWD/src
INCLUDEPATH += $$PWD/ui
INCLUDEPATH += $$PWD/ui/delegates
INCLUDEPATH += $$PWD/controller
INCLUDEPATH += $$PWD/src/geometry
INCLUDEPATH += $$PWD/src/oiemitter
INCLUDEPATH += $$PWD/src/plugin
INCLUDEPATH += $$PWD/src/plugin/oitool
INCLUDEPATH += $$PWD/src/simulation
INCLUDEPATH += $$PWD/src/plugin/oiexchange
INCLUDEPATH += $$PWD/lib/openIndyLib/include
INCLUDEPATH += $$PWD/models
INCLUDEPATH += $$PWD/src/util

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/openIndyLib/bin/release/ -lopenIndyLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/openIndyLib/bin/debug/ -lopenIndyLib
else:unix: CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/openIndyLib/bin/debug -lopenIndyLib
else:unix: CONFIG(release, debug|release): LIBS += -L$$PWD/lib/openIndyLib/bin/release -lopenIndyLib

INCLUDEPATH += $$PWD/lib/openIndyLib/bin/debug
DEPENDPATH += $$PWD/lib/openIndyLib/bin/debug

INCLUDEPATH += $$PWD/lib/openIndyLib/bin/release
DEPENDPATH += $$PWD/lib/openIndyLib/bin/release

SOURCES += \
    controller/console.cpp \
    controller/controller.cpp \
    controller/featureattributes.cpp \
    controller/featurecontainer.cpp \
    controller/oijob.cpp \
    controller/oimodelmanager.cpp \
    controller/systemdbmanager.cpp \
    models/featuretablemodel.cpp \
    models/featuretableproxymodel.cpp \
    src/geometry/circle.cpp \
    src/geometry/ellipsoid.cpp \
    src/geometry/hyperboloid.cpp \
    src/geometry/line.cpp \
    src/geometry/nurbs.cpp \
    src/geometry/ellipse.cpp \
    src/geometry/slottedhole.cpp \
    src/geometry/plane.cpp \
    src/geometry/point.cpp \
    src/geometry/pointcloud.cpp \
    src/geometry/scalarentityangle.cpp \
    src/geometry/scalarentitydistance.cpp \
    src/geometry/scalarentitymeasurementseries.cpp \
    src/geometry/scalarentitytemperature.cpp \
    src/geometry/sphere.cpp \
    src/oiemitter/oiemitter.cpp \
    src/oiemitter/oiexchangeemitter.cpp \
    src/oiemitter/oifeaturestateemitter.cpp \
    src/oiemitter/oifunctionemitter.cpp \
    src/oiemitter/oinetworkadjustmentemitter.cpp \
    src/oiemitter/oisensoremitter.cpp \
    src/oiemitter/oisimulationemitter.cpp \
    src/plugin/oiexchange/oiexchangeobject.cpp \
    src/simulation/simulationdata.cpp \
    src/simulation/uncertaintycomponent.cpp \
    src/util/util.cpp \
    src/connectionconfig.cpp \
    src/coordinatesystem.cpp \
    src/element.cpp \
    src/feature.cpp \
    src/featurewrapper.cpp \
    src/function.cpp \
    src/geometry.cpp \
    src/measurementconfig.cpp \
    src/observation.cpp \
    src/oimetadata.cpp \
    src/oirequestresponse.cpp \
    src/pluginmetadata.cpp \
    src/reading.cpp \
    src/residual.cpp \
    src/sensorconfiguration.cpp \
    src/sensorcontrol.cpp \
    src/sensorlistener.cpp \
    src/station.cpp \
    src/statistic.cpp \
    src/trafoparam.cpp \
    ui/createfeaturedialog.cpp \
    ui/main.cpp \
    ui/mainwindow.cpp \
    ui/delegates/featureoverviewdelegate.cpp \
    ui/delegates/trafoparamdelegate.cpp \
    src/direction.cpp \
    src/position.cpp \
    src/radius.cpp \
    src/geometry/cone.cpp \
    src/geometry/cylinder.cpp \
    src/geometry/paraboloid.cpp \
    src/geometry/torus.cpp

HEADERS  += \
    controller/console.h \
    controller/controller.h \
    controller/featureattributes.h \
    controller/featurecontainer.h \
    controller/oijob.h \
    controller/oimodelmanager.h \
    controller/systemdbmanager.h \
    models/featuretablemodel.h \
    models/featuretableproxymodel.h \
    src/geometry/circle.h \
    src/geometry/ellipsoid.h \
    src/geometry/hyperboloid.h \
    src/geometry/line.h \
    src/geometry/nurbs.h \
    src/geometry/ellipse.h \
    src/geometry/slottedhole.h \
    src/geometry/plane.h \
    src/geometry/point.h \
    src/geometry/pointcloud.h \
    src/geometry/scalarentityangle.h \
    src/geometry/scalarentitydistance.h \
    src/geometry/scalarentitymeasurementseries.h \
    src/geometry/scalarentitytemperature.h \
    src/geometry/sphere.h \
    src/oiemitter/oiemitter.h \
    src/oiemitter/oiexchangeemitter.h \
    src/oiemitter/oifeaturestateemitter.h \
    src/oiemitter/oifunctionemitter.h \
    src/oiemitter/oinetworkadjustmentemitter.h \
    src/oiemitter/oisensoremitter.h \
    src/oiemitter/oisimulationemitter.h \
    src/plugin/oiexchange/oiexchangeobject.h \
    src/plugin/oiexchange/pi_oiexchangedefinedformat.h \
    src/plugin/oiexchange/pi_oiexchangeinterface.h \
    src/plugin/oiexchange/pi_oiexchangesimpleascii.h \
    src/plugin/oitool/pi_oitool.h \
    src/plugin/pi_constructfunction.h \
    src/plugin/pi_fitfunction.h \
    src/plugin/pi_generatefeaturefunction.h \
    src/plugin/pi_geodeticfunction.h \
    src/plugin/pi_lasertracker.h \
    src/plugin/pi_networkadjustment.h \
    src/plugin/pi_objecttransformation.h \
    src/plugin/pi_oiplugin.h \
    src/plugin/pi_systemtransformation.h \
    src/plugin/pi_totalstation.h \
    src/simulation/simulationdata.h \
    src/simulation/simulationmodel.h \
    src/simulation/uncertaintycomponent.h \
    src/util/types.h \
    src/util/util.h \
    src/connectionconfig.h \
    src/coordinatesystem.h \
    src/element.h \
    src/feature.h \
    src/featurewrapper.h \
    src/function.h \
    src/geometry.h \
    src/measurementconfig.h \
    src/observation.h \
    src/oimetadata.h \
    src/oirequestresponse.h \
    src/pluginmetadata.h \
    src/reading.h \
    src/residual.h \
    src/sensor.h \
    src/sensorconfiguration.h \
    src/sensorcontrol.h \
    src/sensorlistener.h \
    src/station.h \
    src/statistic.h \
    src/trafoparam.h \
    ui/createfeaturedialog.h \
    ui/mainwindow.h \
    ui/delegates/featureoverviewdelegate.h \
    ui/delegates/trafoparamdelegate.h \
    src/direction.h \
    src/position.h \
    src/radius.h \
    src/geometry/cone.h \
    src/geometry/cylinder.h \
    src/geometry/paraboloid.h \
    src/geometry/torus.h

FORMS    += \
    ui/createfeaturedialog.ui \
    ui/mainwindow.ui


