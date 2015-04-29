#-------------------------------------------------
#
# Project created by QtCreator 2013-10-12T15:43:14
#
#-------------------------------------------------

QT       += gui
QT += widgets
QT += serialport
QT += xml

TEMPLATE = lib
#TEMPLATE = app
TARGET = $$qtLibraryTarget(p_defaultPlugin)

CONFIG += plugin
CONFIG += c++11

DEFINES += DEFAULT_LIBRARY
DEFINES += WITH_FITTING_UTILS

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
INCLUDEPATH += ../../src/plugin/function
INCLUDEPATH += ../../src/plugin/sensor
INCLUDEPATH += ../../src/plugin/networkAdjustment
INCLUDEPATH += ../../src/plugin/simulation
INCLUDEPATH += ../../src/plugin/tool
INCLUDEPATH += ../../src/plugin/exchange
INCLUDEPATH += ../../src/util
INCLUDEPATH += ../../lib/openIndyLib/include

INCLUDEPATH += $$PWD/sensors/laserTracker/pseudoTracker
INCLUDEPATH += $$PWD/sensors/tachymeter/LeicaGeoCom
INCLUDEPATH += $$PWD/sensors/undefinedSensor/pseudoSensor
INCLUDEPATH += $$PWD/functions/construct
INCLUDEPATH += $$PWD/functions/fit
INCLUDEPATH += $$PWD/functions/objectTransformation
INCLUDEPATH += $$PWD/functions/systemTransformation
INCLUDEPATH += $$PWD/functions/generateFeature
INCLUDEPATH += $$PWD/functions/generateFeature/pointcloud_segmentation
INCLUDEPATH += $$PWD/simulations/simplePolarMeasurement
INCLUDEPATH += $$PWD/exchange

INCLUDEPATH += $$PWD/lib/fitting
INCLUDEPATH += $$PWD/lib/fitting/util

SOURCES += \
    functions/construct/p_intersectlineplane.cpp \
    functions/construct/p_intersectplaneplane.cpp \
    functions/construct/p_linefrompoints.cpp \
    functions/construct/p_planefrompoints.cpp \
    functions/construct/p_pointbetweentwopoints.cpp \
    functions/construct/p_pointfrompoints.cpp \
    functions/construct/p_spherefrompoints.cpp \
    functions/fit/p_bestfitcircle.cpp \
    functions/fit/p_bestfitline.cpp \
    functions/fit/p_bestfitplane.cpp \
    functions/fit/p_bestfitplane2.cpp \
    functions/fit/p_bestfitpoint.cpp \
    functions/fit/p_bestfitsphere.cpp \
    functions/fit/p_bestfitsphererauls.cpp \
    functions/objectTransformation/p_changeradius.cpp \
    functions/systemTransformation/materials.cpp \
    functions/systemTransformation/p_extendedtemperaturecompensation.cpp \
    functions/systemTransformation/p_helmert6param.cpp \
    functions/systemTransformation/p_helmert7Param.cpp \
    functions/systemTransformation/p_simpletemperaturecompensation.cpp \
    exchange/p_oiexchangeascii.cpp \
    sensors/laserTracker/pseudoTracker/p_pseudotracker.cpp \
    sensors/tachymeter/LeicaGeoCom/p_leicatachymeter.cpp \
    simulations/simplePolarMeasurement/simplepolarmeasurement.cpp \
    ../../src/coordinatesystem.cpp \
    ../../src/direction.cpp \
    ../../src/element.cpp \
    ../../src/feature.cpp \
    ../../src/featurecontainer.cpp \
    ../../src/featurewrapper.cpp \
    ../../src/geometry.cpp \
    ../../src/measurementconfig.cpp \
    ../../src/observation.cpp \
    ../../src/oijob.cpp \
    ../../src/oirequestresponse.cpp \
    ../../src/position.cpp \
    ../../src/radius.cpp \
    ../../src/reading.cpp \
    ../../src/residual.cpp \
    ../../src/sensorconfiguration.cpp \
    ../../src/sensorcontrol.cpp \
    ../../src/sensorlistener.cpp \
    ../../src/station.cpp \
    ../../src/statistic.cpp \
    ../../src/trafoparam.cpp \
    ../../src/geometry/circle.cpp \
    ../../src/geometry/cone.cpp \
    ../../src/geometry/cylinder.cpp \
    ../../src/geometry/ellipse.cpp \
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
    ../../src/geometry/slottedhole.cpp \
    ../../src/geometry/sphere.cpp \
    ../../src/geometry/torus.cpp \
    ../../src/plugin/pluginmetadata.cpp \
    ../../src/plugin/exchange/exchangedefinedformat.cpp \
    ../../src/plugin/exchange/exchangeinterface.cpp \
    ../../src/plugin/exchange/exchangesimpleascii.cpp \
    ../../src/plugin/function/function.cpp \
    ../../src/plugin/sensor/sensor.cpp \
    ../../src/plugin/simulation/simulationmodel.cpp \
    ../../src/plugin/tool/tool.cpp \
    ../../src/util/util.cpp \
    functions/objectTransformation/p_register.cpp \
    functions/objectTransformation/p_translatebyvalue.cpp \
    ../../src/featureattributes.cpp \
    lib/fitting/cfitting_approxfunc.cpp \
    lib/fitting/cfitting_circle.cpp \
    lib/fitting/cfitting_ellipse.cpp \
    lib/fitting/cfitting_it.cpp \
    lib/fitting/cfitting_matrix.cpp \
    lib/fitting/cfitting_plane.cpp \
    lib/fitting/cfitting_sphere.cpp \
    lib/fitting/cfitting_util.cpp \
    lib/fitting/util/Getopt.cpp \
    lib/fitting/util/iskeyword.cpp \
    lib/fitting/util/microtim.cpp \
    lib/fitting/util/mystring.cpp \
    lib/fitting/util/rho.cpp \
    p_factory.cpp \
    main.cpp

HEADERS += \
    functions/construct/p_intersectlineplane.h \
    functions/construct/p_intersectplaneplane.h \
    functions/construct/p_linefrompoints.h \
    functions/construct/p_planefrompoints.h \
    functions/construct/p_pointbetweentwopoints.h \
    functions/construct/p_pointfrompoints.h \
    functions/construct/p_spherefrompoints.h \
    functions/fit/p_bestfitcircle.h \
    functions/fit/p_bestfitline.h \
    functions/fit/p_bestfitplane.h \
    functions/fit/p_bestfitplane2.h \
    functions/fit/p_bestfitpoint.h \
    functions/fit/p_bestfitsphere.h \
    functions/fit/p_bestfitsphererauls.h \
    functions/objectTransformation/p_changeradius.h \
    functions/systemTransformation/materials.h \
    functions/systemTransformation/p_extendedtemperaturecompensation.h \
    functions/systemTransformation/p_helmert6param.h \
    functions/systemTransformation/p_helmert7Param.h \
    functions/systemTransformation/p_simpletemperaturecompensation.h \
    exchange/p_oiexchangeascii.h \
    sensors/laserTracker/pseudoTracker/p_pseudotracker.h \
    sensors/tachymeter/LeicaGeoCom/p_leicatachymeter.h \
    simulations/simplePolarMeasurement/simplepolarmeasurement.h \
    ../../src/coordinatesystem.h \
    ../../src/direction.h \
    ../../src/element.h \
    ../../src/feature.h \
    ../../src/featurecontainer.h \
    ../../src/featurewrapper.h \
    ../../src/geometry.h \
    ../../src/measurementconfig.h \
    ../../src/observation.h \
    ../../src/oijob.h \
    ../../src/oirequestresponse.h \
    ../../src/position.h \
    ../../src/radius.h \
    ../../src/reading.h \
    ../../src/residual.h \
    ../../src/sensorconfiguration.h \
    ../../src/sensorcontrol.h \
    ../../src/sensorlistener.h \
    ../../src/station.h \
    ../../src/statistic.h \
    ../../src/trafoparam.h \
    ../../src/geometry/circle.h \
    ../../src/geometry/cone.h \
    ../../src/geometry/cylinder.h \
    ../../src/geometry/ellipse.h \
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
    ../../src/geometry/slottedhole.h \
    ../../src/geometry/sphere.h \
    ../../src/geometry/torus.h \
    ../../src/plugin/plugin.h \
    ../../src/plugin/pluginmetadata.h \
    ../../src/plugin/exchange/exchangedefinedformat.h \
    ../../src/plugin/exchange/exchangeinterface.h \
    ../../src/plugin/exchange/exchangesimpleascii.h \
    ../../src/plugin/function/constructfunction.h \
    ../../src/plugin/function/fitfunction.h \
    ../../src/plugin/function/function.h \
    ../../src/plugin/function/generatefeaturefunction.h \
    ../../src/plugin/function/geodeticfunction.h \
    ../../src/plugin/function/objecttransformation.h \
    ../../src/plugin/function/systemtransformation.h \
    ../../src/plugin/networkAdjustment/networkadjustment.h \
    ../../src/plugin/sensor/lasertracker.h \
    ../../src/plugin/sensor/sensor.h \
    ../../src/plugin/sensor/totalstation.h \
    ../../src/plugin/simulation/simulationmodel.h \
    ../../src/plugin/tool/tool.h \
    ../../src/util/types.h \
    ../../src/util/util.h \
    functions/objectTransformation/p_translatebyvalue.h \
    functions/objectTransformation/p_register.h \
    ../../src/featureattributes.h \
    lib/fitting/cfitting_approxfunc.h \
    lib/fitting/cfitting_circle.h \
    lib/fitting/cfitting_ellipse.h \
    lib/fitting/cfitting_it.h \
    lib/fitting/cfitting_matrix.h \
    lib/fitting/cfitting_plane.h \
    lib/fitting/cfitting_sphere.h \
    lib/fitting/cfitting_util.h \
    lib/fitting/util/getopt.h \
    lib/fitting/util/iskeyword.h \
    lib/fitting/util/microtim.h \
    lib/fitting/util/mystring.h \
    lib/fitting/util/rho.h \
    p_factory.h \
    ../../lib/openIndyLib/include/global.h \
    ../../lib/openIndyLib/include/linearalgebra.h \
    ../../lib/openIndyLib/include/oimat.h \
    ../../lib/openIndyLib/include/oivec.h \
    ../../lib/openIndyLib/include/chooselalib.h

OTHER_FILES += metaInfo.json

FORMS += \
    functions/generateFeature/ps_loadingdialog.ui
