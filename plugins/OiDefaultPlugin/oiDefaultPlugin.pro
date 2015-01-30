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
INCLUDEPATH += ../../src/oiemitter
INCLUDEPATH += ../../src/plugin
INCLUDEPATH += ../../src/plugin/oitool
INCLUDEPATH += ../../src/plugin/oiexchange
INCLUDEPATH += ../../src/simulation
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
INCLUDEPATH += $$PWD/oiTools
INCLUDEPATH += $$PWD/oiExchange

INCLUDEPATH += $$PWD/lib/fitting
INCLUDEPATH += $$PWD/lib/fitting/util

SOURCES += p_factory.cpp \
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
    functions/construct/p_intersectlineplane.cpp \
    functions/construct/p_intersectplaneplane.cpp \
    functions/construct/p_linefrompoints.cpp \
    functions/construct/p_planefrompoints.cpp \
    functions/construct/p_pointbetweentwopoints.cpp \
    functions/construct/p_pointfrompoints.cpp \
    functions/construct/p_spherefrompoints.cpp \
    functions/fit/p_bestfitline.cpp \
    functions/fit/p_bestfitplane.cpp \
    functions/fit/p_bestfitplane2.cpp \
    functions/fit/p_bestfitpoint.cpp \
    functions/fit/p_bestfitsphere.cpp \
    functions/objectTransformation/p_changeradius.cpp \
    functions/objectTransformation/p_projectInPlane.cpp \
    functions/objectTransformation/p_translateByLine.cpp \
    functions/objectTransformation/p_translateByPlane.cpp \
    functions/systemTransformation/p_helmert7Param.cpp \
    sensors/laserTracker/pseudoTracker/p_pseudotracker.cpp \
    sensors/tachymeter/LeicaGeoCom/p_leicatachymeter.cpp \
    sensors/undefinedSensor/pseudoSensor/p_pseudosensor.cpp \
    functions/objectTransformation/p_shiftplane.cpp \
    functions/fit/p_bestfitcircle.cpp \
    functions/systemTransformation/p_simpletemperaturecompensation.cpp \
    functions/systemTransformation/p_extendedtemperaturecompensation.cpp \
    functions/systemTransformation/materials.cpp \
    functions/systemTransformation/p_helmert6param.cpp \
    functions/fit/p_bestfitsphererauls.cpp \
    simulations/simplePolarMeasurement/simplepolarmeasurement.cpp \
    functions/generateFeature/p_pointcloudsegmentation.cpp \
    functions/generateFeature/pointcloud_segmentation/ps_cylindersegment.cpp \
    functions/generateFeature/pointcloud_segmentation/ps_generalmath.cpp \
    functions/generateFeature/pointcloud_segmentation/ps_node.cpp \
    functions/generateFeature/pointcloud_segmentation/ps_octree.cpp \
    functions/generateFeature/pointcloud_segmentation/ps_planesegment.cpp \
    functions/generateFeature/pointcloud_segmentation/ps_pointcloud.cpp \
    functions/generateFeature/pointcloud_segmentation/ps_shapesegment.cpp \
    functions/generateFeature/pointcloud_segmentation/ps_spheresegment.cpp \
    functions/generateFeature/pointcloud_segmentation/ps_point_pc.cpp \
    functions/generateFeature/ps_loadingdialog.cpp \
    ../../src/configuration.cpp \
    ../../src/connectionconfig.cpp \
    ../../src/coordinatesystem.cpp \
    ../../src/element.cpp \
    ../../src/elementdependencies.cpp \
    ../../src/feature.cpp \
    ../../src/featurewrapper.cpp \
    ../../src/function.cpp \
    ../../src/functionconfiguration.cpp \
    ../../src/geometry.cpp \
    ../../src/measurementconfig.cpp \
    ../../src/observation.cpp \
    ../../src/oimetadata.cpp \
    ../../src/pluginmetadata.cpp \
    ../../src/reading.cpp \
    ../../src/residual.cpp \
    ../../src/sensorconfiguration.cpp \
    ../../src/sensorcontrol.cpp \
    ../../src/sensorlistener.cpp \
    ../../src/station.cpp \
    ../../src/statistic.cpp \
    ../../src/trafoparam.cpp \
    ../../src/unitconverter.cpp \
    ../../src/simulation/simulationdata.cpp \
    ../../src/simulation/uncertaintycomponent.cpp \
    ../../src/oiemitter/oiemitter.cpp \
    ../../src/oiemitter/oifunctionemitter.cpp \
    ../../src/oiemitter/oinetworkadjustmentemitter.cpp \
    ../../src/oiemitter/oisensoremitter.cpp \
    ../../src/oiemitter/oisimulationemitter.cpp \
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
    ../../src/geometry/sphere.cpp \
    ../../src/plugin/oitool/oijob.cpp \
    oiTools/p_oireportcreator.cpp \
    ../../src/oiemitter/oiexchangeemitter.cpp \
    ../../src/plugin/oiexchange/oiexchangeobject.cpp \
    oiExchange/p_oiexchangeascii.cpp \
    oiExchange/p_oiexchangepts.cpp \
    ../../src/oirequestresponse.cpp

HEADERS += p_factory.h \
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
    ../../lib/openIndyLib/include/global.h \
    ../../lib/openIndyLib/include/linearalgebra.h \
    ../../lib/openIndyLib/include/oimat.h \
    ../../lib/openIndyLib/include/oivec.h \
    functions/construct/p_intersectlineplane.h \
    functions/construct/p_intersectplaneplane.h \
    functions/construct/p_linefrompoints.h \
    functions/construct/p_planefrompoints.h \
    functions/construct/p_pointbetweentwopoints.h \
    functions/construct/p_pointfrompoints.h \
    functions/construct/p_spherefrompoints.h \
    functions/fit/p_bestfitline.h \
    functions/fit/p_bestfitplane.h \
    functions/fit/p_bestfitplane2.h \
    functions/fit/p_bestfitpoint.h \
    functions/fit/p_bestfitsphere.h \
    functions/objectTransformation/p_changeradius.h \
    functions/objectTransformation/p_projectInPlane.h \
    functions/objectTransformation/p_translateByLine.h \
    functions/objectTransformation/p_translateByPlane.h \
    functions/systemTransformation/p_helmert7Param.h \
    sensors/laserTracker/pseudoTracker/p_pseudotracker.h \
    sensors/tachymeter/LeicaGeoCom/p_leicatachymeter.h \
    sensors/undefinedSensor/pseudoSensor/p_pseudosensor.h \
    functions/objectTransformation/p_shiftplane.h \
    functions/fit/p_bestfitcircle.h \
    functions/systemTransformation/p_simpletemperaturecompensation.h \
    functions/systemTransformation/p_extendedtemperaturecompensation.h \
    functions/systemTransformation/materials.h \
    functions/systemTransformation/p_helmert6param.h \
    functions/fit/p_bestfitsphererauls.h \
    simulations/simplePolarMeasurement/simplepolarmeasurement.h \
    functions/generateFeature/p_pointcloudsegmentation.h \
    functions/generateFeature/pointcloud_segmentation/ps_cylindersegment.h \
    functions/generateFeature/pointcloud_segmentation/ps_generalmath.h \
    functions/generateFeature/pointcloud_segmentation/ps_node.h \
    functions/generateFeature/pointcloud_segmentation/ps_octree.h \
    functions/generateFeature/pointcloud_segmentation/ps_planesegment.h \
    functions/generateFeature/pointcloud_segmentation/ps_pointcloud.h \
    functions/generateFeature/pointcloud_segmentation/ps_shapesegment.h \
    functions/generateFeature/pointcloud_segmentation/ps_spheresegment.h \
    functions/generateFeature/pointcloud_segmentation/ps_point_pc.h \
    functions/generateFeature/ps_loadingdialog.h \
    ../../src/configuration.h \
    ../../src/connectionconfig.h \
    ../../src/coordinatesystem.h \
    ../../src/element.h \
    ../../src/elementdependencies.h \
    ../../src/feature.h \
    ../../src/featurewrapper.h \
    ../../src/function.h \
    ../../src/functionconfiguration.h \
    ../../src/geometry.h \
    ../../src/measurementconfig.h \
    ../../src/observation.h \
    ../../src/oimetadata.h \
    ../../src/pluginmetadata.h \
    ../../src/reading.h \
    ../../src/residual.h \
    ../../src/sensor.h \
    ../../src/sensorconfiguration.h \
    ../../src/sensorcontrol.h \
    ../../src/sensorlistener.h \
    ../../src/station.h \
    ../../src/statistic.h \
    ../../src/trafoparam.h \
    ../../src/unitconverter.h \
    ../../src/simulation/simulationdata.h \
    ../../src/simulation/simulationmodel.h \
    ../../src/simulation/uncertaintycomponent.h \
    ../../src/plugin/pi_constructfunction.h \
    ../../src/plugin/pi_fitfunction.h \
    ../../src/plugin/pi_generatefeaturefunction.h \
    ../../src/plugin/pi_geodeticfunction.h \
    ../../src/plugin/pi_lasertracker.h \
    ../../src/plugin/pi_networkadjustment.h \
    ../../src/plugin/pi_objecttransformation.h \
    ../../src/plugin/pi_oiplugin.h \
    ../../src/plugin/pi_systemtransformation.h \
    ../../src/plugin/pi_totalstation.h \
    ../../src/oiemitter/oiemitter.h \
    ../../src/oiemitter/oifunctionemitter.h \
    ../../src/oiemitter/oinetworkadjustmentemitter.h \
    ../../src/oiemitter/oisensoremitter.h \
    ../../src/oiemitter/oisimulationemitter.h \
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
    ../../src/plugin/oitool/oijob.h \
    ../../src/plugin/oitool/pi_oitool.h \
    oiTools/p_oireportcreator.h \
    ../../src/oiemitter/oiexchangeemitter.h \
    ../../src/plugin/oiexchange/oiexchangeobject.h \
    ../../src/plugin/oiexchange/pi_oiexchangeinterface.h \
    ../../src/plugin/oiexchange/pi_oiexchangesimpleascii.h \
    ../../src/plugin/oiexchange/pi_oiexchangedefinedformat.h \
    oiExchange/p_oiexchangeascii.h \
    oiExchange/p_oiexchangepts.h \
    ../../src/oirequestresponse.h

OTHER_FILES += metaInfo.json

FORMS += \
    functions/generateFeature/ps_loadingdialog.ui
