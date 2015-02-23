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
INCLUDEPATH += $$PWD/ui/models
INCLUDEPATH += $$PWD/ui/delegates
INCLUDEPATH += $$PWD/controller
INCLUDEPATH += $$PWD/controller/networking
INCLUDEPATH += $$PWD/controller/oiStateManager
INCLUDEPATH += $$PWD/controller/oiDataExchange
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
    ui/watchwindow.cpp \
    ui/sensorplugindialog.cpp \
    ui/pluginloaderdialog.cpp \
    ui/measurementconfigdialog.cpp \
    ui/mainwindow.cpp \
    ui/main.cpp \
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
    src/sensorconfiguration.cpp \
    ui/featuredata.cpp \
    models/observationmodel.cpp \
    models/readingmodel.cpp \
    src/geometry/scalarentityangle.cpp \
    src/geometry/scalarentitydistance.cpp \
    ui/movementdialog.cpp \
    ui/settingsdialog.cpp \
    ui/scalarentitydialog.cpp \
    models/featuretreeviewmodel.cpp \
    models/featuretreeitem.cpp \
    models/availableelementstreeviewproxymodel.cpp \
    models/trafoparamproxymodel.cpp \
    ui/nominaldatadialog.cpp \
    models/featuregraphicstreeviewproxymodel.cpp \
    ui/functionstatistic.cpp \
    controller/sortlistbyname.cpp \
    src/residual.cpp \
    models/usedelementsmodel.cpp \
    ui/edittrafoparamdialog.cpp \
    models/matrixmodel.cpp \
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
    ui/delegates/featureoverviewdelegate.cpp \
    ui/delegates/trafoparamdelegate.cpp \
    models/plugintreeviewmodel.cpp \
    models/plugintreeitem.cpp \
    controller/nominalattributeexchange.cpp \
    controller/displayattribute.cpp \
    src/sensorlistener.cpp \
    controller/guiconfiguration.cpp \
    ui/plugininfowidget.cpp \
    ui/oisimulationwidget.cpp \
    controller/trafocontroller.cpp \
    ui/stationinfodialog.cpp \
    ui/realtimedatadialog.cpp \
    ui/watchwindowlistener.cpp \
    src/oiemitter/oifunctionemitter.cpp \
    src/oiemitter/oisensoremitter.cpp \
    src/oiemitter/oisimulationemitter.cpp \
    src/oiemitter/oinetworkadjustmentemitter.cpp \
    src/simulation/uncertaintycomponent.cpp \
    controller/simulationcontroller.cpp \
    ui/histogram.cpp \
    models/simulationerrortablemodel.cpp \
    ui/delegates/simulationdelegate.cpp \
    models/simulationtreeviewmodel.cpp \
    src/simulation/simulationdata.cpp \
    models/oimultiselectionmodel.cpp \
    ui/oiloadingdialog.cpp \
    controller/networking/oiserver.cpp \
    controller/networking/oinetworkconnection.cpp \
    controller/networking/oirequesthandler.cpp \
    src/oirequestresponse.cpp \
    models/pointfeaturemodel.cpp \
    models/pointfeaturefiltermodel.cpp \
    models/observationproxymodel.cpp \
    models/readingproxymodel.cpp \
    controller/plugincopier.cpp \
    ui/delegates/observationdelegate.cpp \
    #src/plugin/oitool/oijob.cpp \
    ui/oitoolaction.cpp \
    src/plugin/oiexchange/oiexchangeobject.cpp \
    ui/exportnominaldialog.cpp \
    #controller/oijobstate.cpp \
    ui/importnominaldialog.cpp \
    src/oiemitter/oiexchangeemitter.cpp \
    controller/oimodelmanager.cpp \
    models/oisimpleasciiexchangemodel.cpp \
    models/geometrytypesproxymodel.cpp \
    models/featuretablemodel.cpp \
    models/featuretableproxymodel.cpp \
    controller/networking/oiwebsocket.cpp \
    controller/networking/oiwebsocketserver.cpp \
    ui/createfeaturedialog.cpp \
    controller/featureattributes.cpp \
    controller/oiStateManager/featurecontainer.cpp \
    controller/oiStateManager/featureupdater.cpp \
    controller/oiStateManager/oiconfigstate.cpp \
    src/oiemitter/oifeaturestateemitter.cpp \
    controller/oiStateManager/oiunitconverter.cpp \
    controller/oiStateManager/oijob.cpp \
    src/util/util.cpp

HEADERS  += \
    ui/watchwindow.h \
    ui/sensorplugindialog.h \
    ui/pluginloaderdialog.h \
    ui/measurementconfigdialog.h \
    ui/mainwindow.h \
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
    src/sensorconfiguration.h \
    ui/featuredata.h \
    models/observationmodel.h \
    models/readingmodel.h \
    src/plugin/pi_oiplugin.h \
    lib/openIndyLib/include/oivec.h \
    lib/openIndyLib/include/oimat.h \
    lib/openIndyLib/include/linearalgebra.h \
    lib/openIndyLib/include/global.h \
    lib/openIndyLib/include/chooselalib.h \
    src/geometry/scalarentityangle.h \
    src/geometry/scalarentitydistance.h  \
    ui/movementdialog.h \
    ui/settingsdialog.h  \
    ui/scalarentitydialog.h \
    models/featuretreeviewmodel.h \
    models/featuretreeitem.h \
    models/availableelementstreeviewproxymodel.h \
    models/trafoparamproxymodel.h \
    ui/nominaldatadialog.h \
    models/featuregraphicstreeviewproxymodel.h \
    ui/functionstatistic.h \
    controller/sortlistbyname.h \
    src/residual.h \
    models/usedelementsmodel.h \
    ui/edittrafoparamdialog.h \
    models/matrixmodel.h \
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
    ui/delegates/featureoverviewdelegate.h \
    ui/delegates/trafoparamdelegate.h \
    models/plugintreeviewmodel.h \
    models/plugintreeitem.h \
    controller/nominalattributeexchange.h \
    controller/displayattribute.h \
    src/sensorlistener.h \
    controller/guiconfiguration.h \
    ui/plugininfowidget.h \
    ui/oisimulationwidget.h \
    controller/trafocontroller.h \
    ui/stationinfodialog.h \
    ui/realtimedatadialog.h \
    ui/watchwindowlistener.h \
    src/oiemitter/oifunctionemitter.h \
    src/oiemitter/oisensoremitter.h \
    src/oiemitter/oisimulationemitter.h \
    src/oiemitter/oinetworkadjustmentemitter.h \
    src/simulation/simulationmodel.h \
    src/simulation/uncertaintycomponent.h \
    controller/simulationcontroller.h \
    ui/histogram.h \
    models/simulationerrortablemodel.h \
    ui/delegates/simulationdelegate.h \
    models/simulationtreeviewmodel.h \
    src/simulation/simulationdata.h \
    src/plugin/pi_generatefeaturefunction.h \
    models/oimultiselectionmodel.h \
    ui/oiloadingdialog.h \
    controller/networking/oiserver.h \
    controller/networking/oinetworkconnection.h \
    controller/networking/oirequesthandler.h \
    src/oirequestresponse.h \
    models/pointfeaturemodel.h \
    models/pointfeaturefiltermodel.h \
    models/observationproxymodel.h \
    models/readingproxymodel.h \
    controller/plugincopier.h \
    ui/delegates/observationdelegate.h \
    src/plugin/oitool/pi_oitool.h \
    #src/plugin/oitool/oijob.h \
    ui/oitoolaction.h \
    src/plugin/oiexchange/oiexchangeascii.h \
    src/plugin/oiexchange/oiexchangeobject.h \
    src/plugin/oiexchange/pi_oiexchangeinterface.h \
    src/plugin/oiexchange/pi_oiexchangesimpleascii.h \
    ui/exportnominaldialog.h \
    #controller/oijobstate.h \
    ui/importnominaldialog.h \
    src/oiemitter/oiexchangeemitter.h \
    src/plugin/oiexchange/pi_oiexchangedefinedformat.h \
    controller/oimodelmanager.h \
    models/oisimpleasciiexchangemodel.h \
    models/geometrytypesproxymodel.h \
    models/featuretablemodel.h \
    models/featuretableproxymodel.h \
    controller/networking/oiwebsocket.h \
    controller/networking/oiwebsocketserver.h \
    ui/createfeaturedialog.h \
    controller/featureattributes.h \
    controller/oiStateManager/featurecontainer.h \
    controller/oiStateManager/featureupdater.h \
    controller/oiStateManager/oiconfigstate.h \
    src/oiemitter/oifeaturestateemitter.h \
    controller/oiStateManager/oiunitconverter.h \
    src/util/types.h \
    controller/oiStateManager/oijob.h \
    src/util/util.h

FORMS    += ui/mainwindow.ui \
    ui/measurementconfigdialog.ui \
    ui/watchwindow.ui \
    ui/pluginloaderdialog.ui \
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
    ui/importnominaldialog.ui \
    ui/createfeaturedialog.ui


