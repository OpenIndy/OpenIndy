#-------------------------------------------------
#
# Project created by QtCreator 2013-10-12T15:43:14
#
#-------------------------------------------------
CONFIG += c++11
QT       += core gui sql svg serialport network opengl xml websockets multimedia

greaterThan(QT_MAJOR_VERSION, 4):

TARGET = openIndy
TEMPLATE = app

DEFINES += OI_MAIN_PROGRAM

# version number from git
include($$PWD/version.pri)

# general build configuration
include($$PWD/config.pri)

# external libraries
include($$PWD/dependencies.pri)

# include paths
INCLUDEPATH += \
    $$PWD/.. \
    $$PWD/../models \
    $$PWD/../models/treeitems \
    $$PWD/../controller \
    $$PWD/../controller/config \
    $$PWD/../controller/exchange \
    $$PWD/../controller/param \
    $$PWD/../controller/network \
    $$PWD/../ui \
    $$PWD/../ui/delegates \
    $$PWD/../ui/dialogs \
    $$PWD/../ui/widgets \
    $$PWD/../ui/actions \
    $$PWD/../ui/selectionmodels

# source files
SOURCES += \
    $$PWD/../main.cpp \
    $$PWD/../ui/mainwindow.cpp \
    $$PWD/../ui/dialogs/createfeaturedialog.cpp \
    $$PWD/../controller/console.cpp \
    $$PWD/../controller/controller.cpp \
    $$PWD/../controller/modelmanager.cpp \
    $$PWD/../controller/systemdbmanager.cpp \
    $$PWD/../models/featuretablemodel.cpp \
    $$PWD/../models/featuretableproxymodel.cpp \
    $$PWD/../controller/oimetadata.cpp \
    $$PWD/../controller/config/featuretablecolumnconfig.cpp \
    $$PWD/../controller/config/trafoparamtablecolumnconfig.cpp \
    $$PWD/../controller/config/parameterdisplayconfig.cpp \
    $$PWD/../models/trafoparamtableproxymodel.cpp \
    $$PWD/../controller/plugincopier.cpp \
    $$PWD/../controller/pluginloader.cpp \
    $$PWD/../ui/dialogs/pluginloaderdialog.cpp \
    $$PWD/../controller/exchange/dataexchanger.cpp \
    $$PWD/../ui/dialogs/importnominaldialog.cpp \
    $$PWD/../ui/dialogs/loadingdialog.cpp \
    $$PWD/../models/treeitems/featuretreeitem.cpp \
    $$PWD/../models/featuretreeviewmodel.cpp \
    $$PWD/../models/availableelementstreeviewproxymodel.cpp \
    $$PWD/../models/usedelementsmodel.cpp \
    $$PWD/../models/treeitems/functiontreeitem.cpp \
    $$PWD/../models/activefeaturefunctionsmodel.cpp \
    $$PWD/../models/functiontablemodel.cpp \
    $$PWD/../models/functiontableproxymodel.cpp \
    $$PWD/../ui/dialogs/featurefunctionsdialog.cpp \
    $$PWD/../ui/widgets/scalarparameterwidget.cpp \
    $$PWD/../ui/selectionmodels/multiselectionmodel.cpp \
    $$PWD/../ui/delegates/featuretabledelegate.cpp \
    $$PWD/../ui/delegates/trafoparamtabledelegate.cpp \
    $$PWD/../controller/config/measurementconfigmanager.cpp \
    $$PWD/../controller/config/sensorconfigurationmanager.cpp \
    $$PWD/../models/sensortablemodel.cpp \
    $$PWD/../models/sensortableproxymodel.cpp \
    $$PWD/../ui/dialogs/sensorconfigurationdialog.cpp \
    $$PWD/../models/sensorconfigurationmodel.cpp \
    $$PWD/../models/sensorconfigurationproxymodel.cpp \
    $$PWD/../ui/delegates/sensorconfigurationlistdelegate.cpp \
    $$PWD/../ui/dialogs/movesensordialog.cpp \
    $$PWD/../ui/dialogs/sensortaskinfodialog.cpp \
    $$PWD/../controller/featureupdater.cpp \
    $$PWD/../controller/trafocontroller.cpp \
    $$PWD/../controller/sortlistbyname.cpp \
    $$PWD/../ui/widgets/plugininfowidget.cpp \
    $$PWD/../ui/dialogs/settingsdialog.cpp \
    $$PWD/../models/treeitems/plugintreeitem.cpp \
    $$PWD/../models/plugintreeviewmodel.cpp \
    $$PWD/../ui/dialogs/pluginmanagerdialog.cpp \
    $$PWD/../ui/dialogs/watchwindowdialog.cpp \
    $$PWD/../controller/exchange/projectexchanger.cpp \
    $$PWD/../ui/dialogs/measurementconfigurationdialog.cpp \
    $$PWD/../models/measurementconfigurationmodel.cpp \
    $$PWD/../models/measurementconfigurationproxymodel.cpp \
    $$PWD/../ui/delegates/measurementconfigurationlistdelegate.cpp \
    $$PWD/../models/availablefunctionslistmodel.cpp \
    $$PWD/../models/availablefunctionslistproxymodel.cpp \
    $$PWD/../models/observationmodel.cpp \
    $$PWD/../models/observationproxymodel.cpp \
    $$PWD/../models/readingmodel.cpp \
    $$PWD/../models/readingproxymodel.cpp \
    $$PWD/../controller/config/observationtablecolumnconfig.cpp \
    $$PWD/../controller/config/readingtablecolumnconfig.cpp \
    $$PWD/../ui/dialogs/actualpropertiesdialog.cpp \
    $$PWD/../ui/dialogs/nominalpropertiesdialog.cpp \
    $$PWD/../ui/actions/toolaction.cpp \
    $$PWD/../controller/exchange/observationimporter.cpp \
    $$PWD/../ui/delegates/observationtabledelegate.cpp \
    $$PWD/../ui/delegates/readingtabledelegate.cpp \
    $$PWD/../ui/dialogs/aboutdialog.cpp \
    $$PWD/../controller/network/oirequesthandler.cpp \
    $$PWD/../controller/network/oiwebsocket.cpp \
    $$PWD/../controller/network/oiwebsocketserver.cpp \
    $$PWD/../ui/dialogs/trafoparampropertiesdialog.cpp \
    $$PWD/../models/functionstatisticmodel.cpp \
    $$PWD/../controller/network/oiwebsocketstreamer.cpp \
    $$PWD/../controller/config/featuretablesortingconfig.cpp \
    $$PWD/../controller/featuresorter.cpp \
    $$PWD/../ui/dialogs/stationpropertiesdialog.cpp \
    $$PWD/../models/sensoraccuracymodel.cpp \
    $$PWD/../models/sensorparametersmodel.cpp \
    $$PWD/../ui/delegates/sensoraccuracydelegate.cpp \
    $$PWD/../ui/delegates/sensorparametersdelegate.cpp \
    $$PWD/../models/bundlesystemsmodel.cpp \
    $$PWD/../models/bundletemplatesmodel.cpp \
    $$PWD/../models/treeitems/bundlegeometryitem.cpp \
    $$PWD/../models/treeitems/bundlestationitem.cpp \
    $$PWD/../models/bundlegeometriesmodel.cpp \
    $$PWD/../models/bundlestationsmodel.cpp \
    $$PWD/../models/functionweightstablemodel.cpp \
    $$PWD/../models/functionweightproxymodel.cpp \
    $$PWD/../models/functionweightdelegate.cpp \
    $$PWD/../ui/dialogs/exportdialog.cpp \
    $$PWD/../models/bundleparametertableproxymodel.cpp \
    $$PWD/../models/featuredifferencetablemodel.cpp \
    $$PWD/../models/featuredifferenceproxymodel.cpp \
    $$PWD/../controller/config/projectconfig.cpp

# header files
HEADERS  += \
    $$PWD/../ui/mainwindow.h \
    $$PWD/../ui/dialogs/createfeaturedialog.h \
    $$PWD/../controller/console.h \
    $$PWD/../controller/controller.h \
    $$PWD/../controller/modelmanager.h \
    $$PWD/../controller/systemdbmanager.h \
    $$PWD/../models/featuretablemodel.h \
    $$PWD/../models/featuretableproxymodel.h \
    $$PWD/../controller/config/featuretablecolumnconfig.h \
    $$PWD/../controller/config/trafoparamtablecolumnconfig.h \
    $$PWD/../controller/config/parameterdisplayconfig.h \
    $$PWD/../models/trafoparamtableproxymodel.h \
    $$PWD/../controller/plugincopier.h \
    $$PWD/../controller/pluginloader.h \
    $$PWD/../ui/dialogs/pluginloaderdialog.h \
    $$PWD/../controller/exchange/dataexchanger.h \
    $$PWD/../ui/dialogs/importnominaldialog.h \
    $$PWD/../controller/param/exchangeParams.h \
    $$PWD/../ui/dialogs/loadingdialog.h \
    $$PWD/../models/treeitems/featuretreeitem.h \
    $$PWD/../models/featuretreeviewmodel.h \
    $$PWD/../models/availableelementstreeviewproxymodel.h \
    $$PWD/../models/usedelementsmodel.h \
    $$PWD/../models/treeitems/functiontreeitem.h \
    $$PWD/../models/activefeaturefunctionsmodel.h \
    $$PWD/../models/functiontablemodel.h \
    $$PWD/../models/functiontableproxymodel.h \
    $$PWD/../ui/dialogs/featurefunctionsdialog.h \
    $$PWD/../ui/widgets/scalarparameterwidget.h \
    $$PWD/../ui/selectionmodels/multiselectionmodel.h \
    $$PWD/../ui/delegates/featuretabledelegate.h \
    $$PWD/../ui/delegates/trafoparamtabledelegate.h \
    $$PWD/../controller/config/measurementconfigmanager.h \
    $$PWD/../controller/config/sensorconfigurationmanager.h \
    $$PWD/../models/sensortablemodel.h \
    $$PWD/../models/sensortableproxymodel.h \
    $$PWD/../ui/dialogs/sensorconfigurationdialog.h \
    $$PWD/../models/sensorconfigurationmodel.h \
    $$PWD/../models/sensorconfigurationproxymodel.h \
    $$PWD/../ui/delegates/sensorconfigurationlistdelegate.h \
    $$PWD/../ui/dialogs/movesensordialog.h \
    $$PWD/../ui/dialogs/sensortaskinfodialog.h \
    $$PWD/../controller/featureupdater.h \
    $$PWD/../controller/trafocontroller.h \
    $$PWD/../controller/sortlistbyname.h \
    $$PWD/../ui/widgets/plugininfowidget.h \
    $$PWD/../ui/dialogs/settingsdialog.h \
    $$PWD/../models/treeitems/plugintreeitem.h \
    $$PWD/../models/plugintreeviewmodel.h \
    $$PWD/../ui/dialogs/pluginmanagerdialog.h \
    $$PWD/../ui/dialogs/watchwindowdialog.h \
    $$PWD/../controller/exchange/projectexchanger.h \
    $$PWD/../ui/dialogs/measurementconfigurationdialog.h \
    $$PWD/../models/measurementconfigurationmodel.h \
    $$PWD/../models/measurementconfigurationproxymodel.h \
    $$PWD/../ui/delegates/measurementconfigurationlistdelegate.h \
    $$PWD/../models/availablefunctionslistmodel.h \
    $$PWD/../models/availablefunctionslistproxymodel.h \
    $$PWD/../models/observationmodel.h \
    $$PWD/../models/observationproxymodel.h \
    $$PWD/../models/readingmodel.h \
    $$PWD/../models/readingproxymodel.h \
    $$PWD/../controller/config/observationtablecolumnconfig.h \
    $$PWD/../controller/config/readingtablecolumnconfig.h \
    $$PWD/../ui/dialogs/actualpropertiesdialog.h \
    $$PWD/../ui/dialogs/nominalpropertiesdialog.h \
    $$PWD/../ui/actions/toolaction.h \
    $$PWD/../controller/exchange/observationimporter.h \
    $$PWD/../ui/delegates/observationtabledelegate.h \
    $$PWD/../ui/delegates/readingtabledelegate.h \
    $$PWD/../ui/dialogs/aboutdialog.h \
    $$PWD/../controller/network/oirequesthandler.h \
    $$PWD/../controller/network/oiwebsocket.h \
    $$PWD/../controller/network/oiwebsocketserver.h \
    $$PWD/../ui/dialogs/trafoparampropertiesdialog.h \
    $$PWD/../models/functionstatisticmodel.h \
    $$PWD/../controller/network/oiwebsocketstreamer.h \
    $$PWD/../controller/config/featuretablesortingconfig.h \
    $$PWD/../controller/featuresorter.h \
    $$PWD/../ui/dialogs/stationpropertiesdialog.h \
    $$PWD/../models/sensoraccuracymodel.h \
    $$PWD/../models/sensorparametersmodel.h \
    $$PWD/../ui/delegates/sensoraccuracydelegate.h \
    $$PWD/../ui/delegates/sensorparametersdelegate.h \
    $$PWD/../models/bundlesystemsmodel.h \
    $$PWD/../models/bundletemplatesmodel.h \
    $$PWD/../models/treeitems/bundlegeometryitem.h \
    $$PWD/../models/treeitems/bundlestationitem.h \
    $$PWD/../models/bundlegeometriesmodel.h \
    $$PWD/../models/bundlestationsmodel.h \
    $$PWD/../models/functionweightstablemodel.h \
    $$PWD/../models/functionweightproxymodel.h \
    $$PWD/../models/functionweightdelegate.h \
    $$PWD/../ui/dialogs/exportdialog.h \
    $$PWD/../models/bundleparametertableproxymodel.h \
    $$PWD/../models/featuredifferencetablemodel.h \
    $$PWD/../models/featuredifferenceproxymodel.h \
    $$PWD/../controller/config/projectconfig.h

# ui files
FORMS    += \
    $$PWD/../ui/mainwindow.ui \
    $$PWD/../ui/dialogs/createfeaturedialog.ui \
    $$PWD/../ui/dialogs/pluginloaderdialog.ui \
    $$PWD/../ui/dialogs/importnominaldialog.ui \
    $$PWD/../ui/dialogs/loadingdialog.ui \
    $$PWD/../ui/dialogs/featurefunctionsdialog.ui \
    $$PWD/../ui/dialogs/sensorconfigurationdialog.ui \
    $$PWD/../ui/dialogs/movesensordialog.ui \
    $$PWD/../ui/dialogs/sensortaskinfodialog.ui \
    $$PWD/../ui/dialogs/settingsdialog.ui \
    $$PWD/../ui/dialogs/pluginmanagerdialog.ui \
    $$PWD/../ui/dialogs/watchwindowdialog.ui \
    $$PWD/../ui/dialogs/measurementconfigurationdialog.ui \
    $$PWD/../ui/dialogs/actualpropertiesdialog.ui \
    $$PWD/../ui/dialogs/nominalpropertiesdialog.ui \
    $$PWD/../ui/dialogs/aboutdialog.ui \
    $$PWD/../ui/dialogs/trafoparampropertiesdialog.ui \
    $$PWD/../ui/dialogs/stationpropertiesdialog.ui \
    $$PWD/../ui/dialogs/exportdialog.ui

# resource files
RESOURCES += \
    $$PWD/../res/res.qrc

# app icon
win32:RC_FILE = $$PWD/../res/openIndy.rc
unix:ICON = $$PWD/../res/openIndy.icns
