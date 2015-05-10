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
INCLUDEPATH += $$PWD/src/geometry
INCLUDEPATH += $$PWD/src/plugin
INCLUDEPATH += $$PWD/src/plugin/function
INCLUDEPATH += $$PWD/src/plugin/sensor
INCLUDEPATH += $$PWD/src/plugin/networkAdjustment
INCLUDEPATH += $$PWD/src/plugin/simulation
INCLUDEPATH += $$PWD/src/plugin/tool
INCLUDEPATH += $$PWD/src/plugin/exchange
INCLUDEPATH += $$PWD/src/util

INCLUDEPATH += $$PWD/models
INCLUDEPATH += $$PWD/models/treeitems
INCLUDEPATH += $$PWD/controller
INCLUDEPATH += $$PWD/controller/config
INCLUDEPATH += $$PWD/controller/exchange
INCLUDEPATH += $$PWD/controller/param
INCLUDEPATH += $$PWD/ui
INCLUDEPATH += $$PWD/ui/delegates
INCLUDEPATH += $$PWD/ui/dialogs
INCLUDEPATH += $$PWD/ui/widgets
INCLUDEPATH += $$PWD/ui/selectionmodels

INCLUDEPATH += $$PWD/lib/openIndyLib/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/openIndyLib/bin/release/ -lopenIndyLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/openIndyLib/bin/debug/ -lopenIndyLib
else:unix: CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/openIndyLib/bin/debug -lopenIndyLib
else:unix: CONFIG(release, debug|release): LIBS += -L$$PWD/lib/openIndyLib/bin/release -lopenIndyLib

INCLUDEPATH += $$PWD/lib/openIndyLib/bin/debug
DEPENDPATH += $$PWD/lib/openIndyLib/bin/debug

INCLUDEPATH += $$PWD/lib/openIndyLib/bin/release
DEPENDPATH += $$PWD/lib/openIndyLib/bin/release

SOURCES += \
    main.cpp \
    ui/mainwindow.cpp \
    ui/dialogs/createfeaturedialog.cpp \
    controller/console.cpp \
    controller/controller.cpp \
    src/featureattributes.cpp \
    controller/modelmanager.cpp \
    controller/systemdbmanager.cpp \
    models/featuretablemodel.cpp \
    models/featuretableproxymodel.cpp \
    src/featurecontainer.cpp \
    src/oijob.cpp \
    src/coordinatesystem.cpp \
    src/direction.cpp \
    src/element.cpp \
    src/feature.cpp \
    src/featurewrapper.cpp \
    src/geometry.cpp \
    src/measurementconfig.cpp \
    src/observation.cpp \
    controller/oimetadata.cpp \
    src/oirequestresponse.cpp \
    src/position.cpp \
    src/radius.cpp \
    src/reading.cpp \
    src/residual.cpp \
    src/sensorconfiguration.cpp \
    src/sensorcontrol.cpp \
    src/sensorlistener.cpp \
    src/station.cpp \
    src/statistic.cpp \
    src/trafoparam.cpp \
    src/util/util.cpp \
    src/geometry/circle.cpp \
    src/geometry/cone.cpp \
    src/geometry/cylinder.cpp \
    src/geometry/ellipse.cpp \
    src/geometry/ellipsoid.cpp \
    src/geometry/hyperboloid.cpp \
    src/geometry/line.cpp \
    src/geometry/nurbs.cpp \
    src/geometry/paraboloid.cpp \
    src/geometry/plane.cpp \
    src/geometry/point.cpp \
    src/geometry/pointcloud.cpp \
    src/geometry/scalarentityangle.cpp \
    src/geometry/scalarentitydistance.cpp \
    src/geometry/scalarentitymeasurementseries.cpp \
    src/geometry/scalarentitytemperature.cpp \
    src/geometry/slottedhole.cpp \
    src/geometry/sphere.cpp \
    src/geometry/torus.cpp \
    src/plugin/pluginmetadata.cpp \
    src/plugin/tool/tool.cpp \
    src/plugin/simulation/simulationmodel.cpp \
    src/plugin/function/function.cpp \
    src/plugin/sensor/sensor.cpp \
    src/plugin/exchange/exchangedefinedformat.cpp \
    src/plugin/exchange/exchangesimpleascii.cpp \
    src/plugin/exchange/exchangeinterface.cpp \
    controller/config/featuretablecolumnconfig.cpp \
    controller/config/trafoparamtablecolumnconfig.cpp \
    controller/config/parameterdisplayconfig.cpp \
    models/trafoparamtableproxymodel.cpp \
    controller/plugincopier.cpp \
    controller/pluginloader.cpp \
    ui/dialogs/pluginloaderdialog.cpp \
    controller/exchange/dataexchanger.cpp \
    ui/dialogs/importnominaldialog.cpp \
    ui/dialogs/loadingdialog.cpp \
    models/treeitems/featuretreeitem.cpp \
    models/featuretreeviewmodel.cpp \
    models/availableelementstreeviewproxymodel.cpp \
    models/usedelementsmodel.cpp \
    models/treeitems/functiontreeitem.cpp \
    models/activefeaturefunctionsmodel.cpp \
    models/functiontablemodel.cpp \
    models/functiontableproxymodel.cpp \
    ui/dialogs/featurefunctionsdialog.cpp \
    ui/widgets/scalarparameterwidget.cpp \
    ui/selectionmodels/multiselectionmodel.cpp \
    ui/delegates/featuretabledelegate.cpp \
    ui/delegates/trafoparamtabledelegate.cpp \
    controller/config/measurementconfigmanager.cpp \
    controller/config/sensorconfigurationmanager.cpp \
    models/sensortablemodel.cpp \
    models/sensortableproxymodel.cpp \
    ui/dialogs/sensorconfigurationdialog.cpp \
    models/sensorconfigurationmodel.cpp \
    models/sensorconfigurationproxymodel.cpp \
    ui/delegates/sensorconfigurationlistdelegate.cpp \
    ui/dialogs/movesensordialog.cpp \
    ui/dialogs/sensortaskinfodialog.cpp \
    controller/featureupdater.cpp \
    controller/trafocontroller.cpp \
    controller/sortlistbyname.cpp \
    ui/widgets/plugininfowidget.cpp \
    ui/dialogs/settingsdialog.cpp \
    models/treeitems/plugintreeitem.cpp \
    models/plugintreeviewmodel.cpp \
    ui/dialogs/pluginmanagerdialog.cpp

HEADERS  += \
    ui/mainwindow.h \
    ui/dialogs/createfeaturedialog.h \
    controller/console.h \
    controller/controller.h \
    src/featureattributes.h \
    controller/modelmanager.h \
    controller/systemdbmanager.h \
    models/featuretablemodel.h \
    models/featuretableproxymodel.h \
    src/featurecontainer.h \
    src/oijob.h \
    src/coordinatesystem.h \
    src/direction.h \
    src/element.h \
    src/feature.h \
    src/featurewrapper.h \
    src/geometry.h \
    src/measurementconfig.h \
    src/observation.h \
    controller/oimetadata.h \
    src/oirequestresponse.h \
    src/position.h \
    src/radius.h \
    src/reading.h \
    src/residual.h \
    src/sensorconfiguration.h \
    src/sensorcontrol.h \
    src/sensorlistener.h \
    src/station.h \
    src/statistic.h \
    src/trafoparam.h \
    src/util/types.h \
    src/util/util.h \
    src/geometry/circle.h \
    src/geometry/cone.h \
    src/geometry/cylinder.h \
    src/geometry/ellipse.h \
    src/geometry/ellipsoid.h \
    src/geometry/hyperboloid.h \
    src/geometry/line.h \
    src/geometry/nurbs.h \
    src/geometry/paraboloid.h \
    src/geometry/plane.h \
    src/geometry/point.h \
    src/geometry/pointcloud.h \
    src/geometry/scalarentityangle.h \
    src/geometry/scalarentitydistance.h \
    src/geometry/scalarentitymeasurementseries.h \
    src/geometry/scalarentitytemperature.h \
    src/geometry/slottedhole.h \
    src/geometry/sphere.h \
    src/geometry/torus.h \
    src/plugin/pluginmetadata.h \
    src/plugin/tool/tool.h \
    src/plugin/simulation/simulationmodel.h \
    src/plugin/sensor/lasertracker.h \
    src/plugin/sensor/sensor.h \
    src/plugin/sensor/totalstation.h \
    src/plugin/networkAdjustment/networkadjustment.h \
    src/plugin/function/constructfunction.h \
    src/plugin/function/fitfunction.h \
    src/plugin/function/function.h \
    src/plugin/function/generatefeaturefunction.h \
    src/plugin/function/geodeticfunction.h \
    src/plugin/function/objecttransformation.h \
    src/plugin/function/systemtransformation.h \
    src/plugin/exchange/exchangedefinedformat.h \
    src/plugin/exchange/exchangeinterface.h \
    src/plugin/exchange/exchangesimpleascii.h \
    lib/openIndyLib/include/chooselalib.h \
    lib/openIndyLib/include/global.h \
    lib/openIndyLib/include/linearalgebra.h \
    lib/openIndyLib/include/oimat.h \
    lib/openIndyLib/include/oivec.h \
    controller/config/featuretablecolumnconfig.h \
    controller/config/trafoparamtablecolumnconfig.h \
    controller/config/parameterdisplayconfig.h \
    models/trafoparamtableproxymodel.h \
    src/plugin/plugin.h \
    controller/plugincopier.h \
    controller/pluginloader.h \
    ui/dialogs/pluginloaderdialog.h \
    controller/exchange/dataexchanger.h \
    ui/dialogs/importnominaldialog.h \
    controller/param/exchangeParams.h \
    ui/dialogs/loadingdialog.h \
    models/treeitems/featuretreeitem.h \
    models/featuretreeviewmodel.h \
    models/availableelementstreeviewproxymodel.h \
    models/usedelementsmodel.h \
    models/treeitems/functiontreeitem.h \
    models/activefeaturefunctionsmodel.h \
    models/functiontablemodel.h \
    models/functiontableproxymodel.h \
    ui/dialogs/featurefunctionsdialog.h \
    ui/widgets/scalarparameterwidget.h \
    ui/selectionmodels/multiselectionmodel.h \
    ui/delegates/featuretabledelegate.h \
    ui/delegates/trafoparamtabledelegate.h \
    controller/config/measurementconfigmanager.h \
    controller/config/sensorconfigurationmanager.h \
    models/sensortablemodel.h \
    models/sensortableproxymodel.h \
    ui/dialogs/sensorconfigurationdialog.h \
    models/sensorconfigurationmodel.h \
    models/sensorconfigurationproxymodel.h \
    ui/delegates/sensorconfigurationlistdelegate.h \
    ui/dialogs/movesensordialog.h \
    ui/dialogs/sensortaskinfodialog.h \
    controller/featureupdater.h \
    controller/trafocontroller.h \
    controller/sortlistbyname.h \
    ui/widgets/plugininfowidget.h \
    ui/dialogs/settingsdialog.h \
    models/treeitems/plugintreeitem.h \
    models/plugintreeviewmodel.h \
    ui/dialogs/pluginmanagerdialog.h

FORMS    += \
    ui/mainwindow.ui \
    ui/dialogs/createfeaturedialog.ui \
    ui/dialogs/pluginloaderdialog.ui \
    ui/dialogs/importnominaldialog.ui \
    ui/dialogs/loadingdialog.ui \
    ui/dialogs/featurefunctionsdialog.ui \
    ui/dialogs/sensorconfigurationdialog.ui \
    ui/dialogs/movesensordialog.ui \
    ui/dialogs/sensortaskinfodialog.ui \
    ui/dialogs/settingsdialog.ui \
    ui/dialogs/pluginmanagerdialog.ui
