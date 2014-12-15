#-------------------------------------------------
#
# Project created by QtCreator 2013-09-18T14:25:09
#
#-------------------------------------------------

QT       += core gui sql serialport network opengl xml

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
INCLUDEPATH += $$PWD/controller
INCLUDEPATH += $$PWD/controller/networking
INCLUDEPATH += $$PWD/src/geometry
INCLUDEPATH += $$PWD/src/oiemitter
INCLUDEPATH += $$PWD/src/plugin
INCLUDEPATH += $$PWD/src/plugin/oitool
INCLUDEPATH += $$PWD/src/simulation
INCLUDEPATH += $$PWD/controller/oiDataExchange
INCLUDEPATH += $$PWD/lib/openIndyLib/include
INCLUDEPATH += $$PWD/src/plugin/oiexchange

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/openIndyLib/bin/release/ -lopenIndyLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/openIndyLib/bin/debug/ -lopenIndyLib
else:unix: CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/openIndyLib/bin/debug -lopenIndyLib
else:unix: CONFIG(release, debug|release): LIBS += -L$$PWD/lib/openIndyLib/bin/release -lopenIndyLib

INCLUDEPATH += $$PWD/lib/openIndyLib/bin/debug
DEPENDPATH += $$PWD/lib/openIndyLib/bin/debug

INCLUDEPATH += $$PWD/lib/openIndyLib/bin/release
DEPENDPATH += $$PWD/lib/openIndyLib/bin/release

SOURCES += \
    ui/watchwindow.cpp \
    ui/tablemodel.cpp \
    ui/sensorplugindialog.cpp \
    ui/pluginloaderdialog.cpp \
    ui/measurementconfigdialog.cpp \
    ui/mainwindow.cpp \
    ui/main.cpp \
    ui/createfeature.cpp \
    controller/systemdbmanager.cpp \
    controller/pluginloader.cpp \
    controller/controller.cpp \
    src/trafoparam.cpp \
    src/statistic.cpp \
    src/sensorcontrol.cpp \
    src/reading.cpp \
    src/pluginmetadata.cpp \
    src/oimetadata.cpp \
    src/oiemitter/oiemitter.cpp \
    src/observation.cpp \
    src/measurementconfig.cpp \
    src/geometry.cpp \
    src/feature.cpp \
    src/coordinatesystem.cpp \
    controller/console.cpp \
    src/configuration.cpp \
    src/element.cpp \
    src/geometry/sphere.cpp \
    src/geometry/pointcloud.cpp \
    src/geometry/point.cpp \
    src/geometry/plane.cpp \
    src/geometry/paraboloid.cpp \
    src/geometry/nurbs.cpp \
    src/geometry/line.cpp \
    src/geometry/hyperboloid.cpp \
    src/geometry/ellipsoid.cpp \
    src/geometry/cylinder.cpp \
    src/geometry/cone.cpp \
    src/geometry/circle.cpp \
    ui/functionpluginloader.cpp \
    src/connectionconfig.cpp \
    ui/sensorinfodialog.cpp \
    src/featurewrapper.cpp \
    src/station.cpp \
    ui/glwidget.cpp \
    src/sensorconfiguration.cpp \
    ui/featuredata.cpp \
    ui/observationmodel.cpp \
    ui/readingmodel.cpp \
    ui/oiGraphixFactory/oigraphix_sphere.cpp \
    ui/oiGraphixFactory/oigraphix.cpp \
    src/geometry/scalarentityangle.cpp \
    src/geometry/scalarentitydistance.cpp \
    ui/movementdialog.cpp \
    src/unitconverter.cpp \
    ui/settingsdialog.cpp \
    ui/oiGraphixFactory/oigraphix_plane.cpp \
    ui/oiGraphixFactory/oigraphix_point.cpp \
    ui/oiGraphixFactory/oigraphix_line.cpp \
    ui/oiGraphixFactory/oigraphix_station.cpp \
    ui/scalarentitydialog.cpp \
    ui/featuretreeviewmodel.cpp \
    ui/featuretreeitem.cpp \
    ui/availableelementstreeviewproxymodel.cpp \
    ui/trafoparamproxymodel.cpp \
    ui/nominaldatadialog.cpp \
    ui/featuregraphicstreeviewproxymodel.cpp \
    ui/functionstatistic.cpp \
    controller/featureupdater.cpp \
    controller/sortlistbyname.cpp \
    src/residual.cpp \
    ui/usedelementsmodel.cpp \
    ui/edittrafoparamdialog.cpp \
    ui/matrixmodel.cpp \
    src/function.cpp \
    controller/oiDataExchange/oidataexchanger.cpp \
    controller/oiDataExchange/oiprojectdata.cpp \
    controller/oiDataExchange/oiprojectexchanger.cpp \
    src/functionconfiguration.cpp \
    src/geometry/scalarentitytemperature.cpp \
    src/geometry/scalarentitymeasurementseries.cpp \
    ui/customparameterwidget.cpp \
    src/elementdependencies.cpp \
    controller/deletefeaturesfunctor.cpp \
    ui/featureoverviewdelegate.cpp \
    ui/trafoparamdelegate.cpp \
    ui/plugintreeviewmodel.cpp \
    ui/plugintreeitem.cpp \
    controller/featureattributesexchange.cpp \
    controller/nominalattributeexchange.cpp \
    controller/displayattribute.cpp \
    src/sensorlistener.cpp \
    controller/guiconfiguration.cpp \
    ui/plugininfowidget.cpp \
    ui/oisimulationwidget.cpp \
    controller/oifeaturestate.cpp \
    controller/trafocontroller.cpp \
    ui/featureoverviewproxymodel.cpp \
    ui/stationinfodialog.cpp \
    ui/realtimedatadialog.cpp \
    ui/oiGraphixFactory/oigraphix_pointcloud.cpp \
    ui/watchwindowlistener.cpp \
    src/oiemitter/oifunctionemitter.cpp \
    src/oiemitter/oisensoremitter.cpp \
    src/oiemitter/oisimulationemitter.cpp \
    src/oiemitter/oinetworkadjustmentemitter.cpp \
    src/simulation/uncertaintycomponent.cpp \
    controller/simulationcontroller.cpp \
    ui/histogram.cpp \
    ui/simulationerrortablemodel.cpp \
    ui/simulationdelegate.cpp \
    ui/simulationtreeviewmodel.cpp \
    src/simulation/simulationdata.cpp \
    ui/oimultiselectionmodel.cpp \
    ui/oiloadingdialog.cpp \
    controller/oiconfigstate.cpp \
    controller/networking/oiserver.cpp \
    controller/networking/oinetworkconnection.cpp \
    controller/networking/oirequesthandler.cpp \
    controller/networking/oirequestresponse.cpp \
    ui/pointfeaturemodel.cpp \
    ui/pointfeaturefiltermodel.cpp \
    ui/observationproxymodel.cpp \
    ui/readingproxymodel.cpp \
    controller/plugincopier.cpp \
    ui/observationdelegate.cpp \
    src/plugin/oitool/oijob.cpp \
    ui/oitoolaction.cpp \
    src/plugin/oiexchange/oiexchangeobject.cpp \
    ui/exportnominaldialog.cpp \
    controller/oijobstate.cpp \
    ui/importnominaldialog.cpp \
    src/oiemitter/oiexchangeemitter.cpp \
    controller/oimodelmanager.cpp \
    ui/oisimpleasciiexchangemodel.cpp \
    ui/geometrytypesproxymodel.cpp

HEADERS  += \
    ui/watchwindow.h \
    ui/tablemodel.h \
    ui/sensorplugindialog.h \
    ui/pluginloaderdialog.h \
    ui/measurementconfigdialog.h \
    ui/mainwindow.h \
    ui/createfeature.h \
    controller/systemdbmanager.h \
    controller/pluginloader.h \
    controller/controller.h \
    src/trafoparam.h \
    src/statistic.h \
    src/sensorcontrol.h \
    src/sensor.h \
    src/reading.h \
    src/pluginmetadata.h \
    src/oimetadata.h \
    src/oiemitter/oiemitter.h \
    src/observation.h \
    src/measurementconfig.h \
    src/geometry.h \
    src/function.h \
    src/feature.h \
    src/element.h \
    src/coordinatesystem.h \
    controller/console.h \
    src/configuration.h \
    src/plugin/pi_totalstation.h \
    src/plugin/pi_systemtransformation.h \
    src/plugin/pi_objecttransformation.h \
    src/plugin/pi_networkadjustment.h \
    src/plugin/pi_lasertracker.h \
    src/plugin/pi_geodeticfunction.h \
    src/plugin/pi_fitfunction.h \
    src/plugin/pi_constructfunction.h \
    src/geometry/sphere.h \
    src/geometry/pointcloud.h \
    src/geometry/point.h \
    src/geometry/plane.h \
    src/geometry/paraboloid.h \
    src/geometry/nurbs.h \
    src/geometry/line.h \
    src/geometry/hyperboloid.h \
    src/geometry/ellipsoid.h \
    src/geometry/cylinder.h \
    src/geometry/cone.h \
    src/geometry/circle.h \
    ui/functionpluginloader.h \
    src/connectionconfig.h \
    ui/sensorinfodialog.h \
    src/featurewrapper.h \
    src/station.h \
    ui/glwidget.h \
    src/sensorconfiguration.h \
    ui/featuredata.h \
    ui/observationmodel.h \
    ui/readingmodel.h \
    src/plugin/pi_oiplugin.h \
    lib/openIndyLib/include/oivec.h \
    lib/openIndyLib/include/oimat.h \
    lib/openIndyLib/include/linearalgebra.h \
    lib/openIndyLib/include/global.h \
    lib/openIndyLib/include/chooselalib.h \
    ui/oiGraphixFactory/oigraphix_sphere.h \
    ui/oiGraphixFactory/oigraphix_geometry.h \
    ui/oiGraphixFactory/oigraphix.h \
    src/geometry/scalarentityangle.h \
    src/geometry/scalarentitydistance.h  \
    ui/movementdialog.h \
    src/unitconverter.h \
    ui/settingsdialog.h  \
    ui/oiGraphixFactory/oigraphix_plane.h \
    ui/oiGraphixFactory/oigraphix_point.h \
    ui/oiGraphixFactory/oigraphix_line.h \
    ui/oiGraphixFactory/oigraphix_station.h \
    ui/scalarentitydialog.h \
    ui/featuretreeviewmodel.h \
    ui/featuretreeitem.h \
    ui/availableelementstreeviewproxymodel.h \
    ui/trafoparamproxymodel.h \
    ui/nominaldatadialog.h \
    ui/featuregraphicstreeviewproxymodel.h \
    ui/functionstatistic.h \
    controller/featureupdater.h \
    controller/sortlistbyname.h \
    src/residual.h \
    ui/usedelementsmodel.h \
    ui/edittrafoparamdialog.h \
    ui/matrixmodel.h \
    controller/oiDataExchange/oidataexchanger.h \
    controller/oiDataExchange/oiprojectdata.h \
    controller/oiDataExchange/oiprojectexchanger.h \
    src/functionconfiguration.h \
    src/geometry/scalarentitytemperature.h \
    src/geometry/scalarentitymeasurementseries.h \
    ui/customparameterwidget.h \
    src/elementdependencies.h \
    controller/oifunctor.h \
    controller/deletefeaturesfunctor.h \
    ui/featureoverviewdelegate.h \
    ui/trafoparamdelegate.h \
    ui/plugintreeviewmodel.h \
    ui/plugintreeitem.h \
    controller/featureattributesexchange.h \
    controller/nominalattributeexchange.h \
    controller/displayattribute.h \
    src/sensorlistener.h \
    controller/guiconfiguration.h \
    ui/plugininfowidget.h \
    ui/oisimulationwidget.h \
    controller/oifeaturestate.h \
    controller/trafocontroller.h \
    ui/featureoverviewproxymodel.h \
    ui/stationinfodialog.h \
    ui/realtimedatadialog.h \
    ui/oiGraphixFactory/oigraphix_pointcloud.h \
    ui/watchwindowlistener.h \
    src/oiemitter/oifunctionemitter.h \
    src/oiemitter/oisensoremitter.h \
    src/oiemitter/oisimulationemitter.h \
    src/oiemitter/oinetworkadjustmentemitter.h \
    src/simulation/simulationmodel.h \
    src/simulation/uncertaintycomponent.h \
    controller/simulationcontroller.h \
    ui/histogram.h \
    ui/simulationerrortablemodel.h \
    ui/simulationdelegate.h \
    ui/simulationtreeviewmodel.h \
    src/simulation/simulationdata.h \
    src/plugin/pi_generatefeaturefunction.h \
    ui/oimultiselectionmodel.h \
    ui/oiloadingdialog.h \
    controller/oiconfigstate.h \
    controller/networking/oiserver.h \
    controller/networking/oinetworkconnection.h \
    controller/networking/oirequesthandler.h \
    controller/networking/oirequestresponse.h \
    ui/pointfeaturemodel.h \
    ui/pointfeaturefiltermodel.h \
    ui/observationproxymodel.h \
    ui/readingproxymodel.h \
    controller/plugincopier.h \
    ui/observationdelegate.h \
    src/plugin/oitool/pi_oitool.h \
    src/plugin/oitool/oijob.h \
    ui/oitoolaction.h \
    src/plugin/oiexchange/oiexchangeascii.h \
    src/plugin/oiexchange/oiexchangeobject.h \
    src/plugin/oiexchange/pi_oiexchangeinterface.h \
    src/plugin/oiexchange/pi_oiexchangesimpleascii.h \
    ui/exportnominaldialog.h \
    controller/oijobstate.h \
    ui/importnominaldialog.h \
    src/oiemitter/oiexchangeemitter.h \
    src/plugin/oiexchange/pi_oiexchangedefinedformat.h \
    controller/oimodelmanager.h \
    ui/oisimpleasciiexchangemodel.h \
    ui/geometrytypesproxymodel.h

FORMS    += ui/mainwindow.ui \
    ui/measurementconfigdialog.ui \
    ui/watchwindow.ui \
    ui/pluginloaderdialog.ui \
    ui/createfeature.ui \
    ui/sensorplugindialog.ui \
    ui/functionpluginloader.ui \
    ui/sensorinfodialog.ui \
    ui/featuredata.ui \
    ui/movementdialog.ui \
    ui/settingsdialog.ui \
    ui/scalarentitydialog.ui \
    ui/nominaldatadialog.ui \
    ui/edittrafoparamdialog.ui \
    ui/oisimulationwidget.ui \
    ui/stationinfodialog.ui \
    ui/realtimedatadialog.ui \
    ui/oiloadingdialog.ui \
    ui/exportnominaldialog.ui \
    ui/importnominaldialog.ui


