#-------------------------------------------------
#
# Project created by QtCreator 2013-09-18T14:25:09
#
#-------------------------------------------------

QT       += core gui sql svg serialport network opengl xml websockets 3dcore 3drenderer 3dinput

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

#--------------------------
# get OpenIndy-Math version
#--------------------------

OpenIndyMath_VERSION = $$system(git --git-dir $$PWD/lib/OpenIndy-Core/lib/OpenIndy-Math/.git --work-tree $$PWD describe --always --tags) # get git version
OpenIndyMath_VERSION = $$replace(OpenIndyMath_VERSION, "-g"{1}\w*, ) # remove commit hash after tag name
OpenIndyMath_VERSION = $$replace(OpenIndyMath_VERSION, "-", ".") # remove remaining hyphen
OpenIndyMath_VERSION = $$replace(OpenIndyMath_VERSION, "\b[0-9a-f]{5,40}\b", ) # remove commit hash (only if no tag has been set yet)
DEFINES += OPENINDY_MATH_VERSION=\\\"$$OpenIndyMath_VERSION\\\"

isEmpty(OpenIndyMath_VERSION){
    message("no math version")
}else{
    message(OpenIndy-Math version:)
    message($$OpenIndyMath_VERSION)
}

#--------------------------
# get OpenIndy-Core version
#--------------------------

OpenIndyCore_VERSION = $$system(git --git-dir $$PWD/lib/OpenIndy-Core/.git --work-tree $$PWD describe --always --tags) # get git version
OpenIndyCore_VERSION = $$replace(OpenIndyCore_VERSION, "-g"{1}\w*, ) # remove commit hash after tag name
OpenIndyCore_VERSION = $$replace(OpenIndyCore_VERSION, "-", ".") # remove remaining hyphen
PluginVersion = $$replace(OpenIndyCore_VERSION, "[/.]", )
DEFINES += PLUGIN_INTERFACE_VERSION=$$PluginVersion
OpenIndyCore_VERSION = $$replace(OpenIndyCore_VERSION, "\b[0-9a-f]{5,40}\b", ) # remove commit hash (only if no tag has been set yet)
DEFINES += OPENINDY_CORE_VERSION=\\\"$$OpenIndyCore_VERSION\\\"

isEmpty(OpenIndyCore_VERSION){
    message("no core version")
}else{
    message(OpenIndy-Core version:)
    message($$OpenIndyCore_VERSION)
}

#---------------------
# get OpenIndy version
#---------------------

OpenIndy_VERSION = $$system(git --git-dir $$PWD/.git --work-tree $$PWD describe --always --tags) # get git version
OpenIndy_VERSION = $$replace(OpenIndy_VERSION, "-g"{1}\w*, ) # remove commit hash after tag name
OpenIndy_VERSION = $$replace(OpenIndy_VERSION, "-", ".") # remove remaining hyphen
OpenIndy_VERSION = $$replace(OpenIndy_VERSION, "\b[0-9a-f]{5,40}\b", ) # remove commit hash (only if no tag has been set yet)
DEFINES += OPENINDY_VERSION=\\\"$$OpenIndy_VERSION\\\"

isEmpty(OpenIndy_VERSION){
    message("no OpenIndy version")
}else{
    message(OpenIndy version:)
    message($$OpenIndy_VERSION)
    VERSION = $$OpenIndy_VERSION
}

RESOURCES += \
    res/res.qrc

win32:RC_FILE = $$PWD/res/openIndy.rc
unix:ICON = $$PWD/res/openIndy.icns

#-----------------------------------linux dependency---------------------------------------
linux: LIBS = -lGLU
#-----------------------------------Linear Algebra---------------------------------------

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
INCLUDEPATH += $$PWD/ui/actions
INCLUDEPATH += $$PWD/ui/selectionmodels
INCLUDEPATH += $$PWD/ui/3dengine

INCLUDEPATH += $$PWD/lib/OpenIndy-Core/include
INCLUDEPATH += $$PWD/lib/OpenIndy-Core/include/geometry
INCLUDEPATH += $$PWD/lib/OpenIndy-Core/include/util
INCLUDEPATH += $$PWD/lib/OpenIndy-Core/include/plugin
INCLUDEPATH += $$PWD/lib/OpenIndy-Core/include/plugin/exchange
INCLUDEPATH += $$PWD/lib/OpenIndy-Core/include/plugin/function
INCLUDEPATH += $$PWD/lib/OpenIndy-Core/include/plugin/networkAdjustment
INCLUDEPATH += $$PWD/lib/OpenIndy-Core/include/plugin/sensor
INCLUDEPATH += $$PWD/lib/OpenIndy-Core/include/plugin/simulation
INCLUDEPATH += $$PWD/lib/OpenIndy-Core/include/plugin/tool
INCLUDEPATH += $$PWD/lib/OpenIndy-Core/lib/OpenIndy-Math/include

#--------------
# OpenIndy-Core
#--------------

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/OpenIndy-Core/bin/release/ -lopenIndyCore1
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/OpenIndy-Core/bin/debug/ -lopenIndyCore1
else:unix:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/OpenIndy-Core/bin/release/ -lopenIndyCore1
else:unix:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/OpenIndy-Core/bin/debug/ -lopenIndyCore1

INCLUDEPATH += -L$$PWD/lib/OpenIndy-Core/bin/debug
DEPENDPATH += -L$$PWD/lib/OpenIndy-Core/bin/debug

INCLUDEPATH += -L$$PWD/lib/OpenIndy-Core/bin/release
DEPENDPATH += -L$$PWD/lib/OpenIndy-Core/bin/release

#--------------
# OpenIndy-Math
#--------------

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/OpenIndy-Core/lib/OpenIndy-Math/bin/release/ -lopenIndyMath1
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/OpenIndy-Core/lib/OpenIndy-Math/bin/debug/ -lopenIndyMath1
else:unix:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/OpenIndy-Core/lib/OpenIndy-Math/bin/release/ -lopenIndyMath1
else:unix:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/OpenIndy-Core/lib/OpenIndy-Math/bin/debug/ -lopenIndyMath1

INCLUDEPATH += -L$$PWD/lib/OpenIndy-Core/lib/OpenIndy-Math/bin/debug
DEPENDPATH += -L$$PWD/lib/OpenIndy-Core/lib/OpenIndy-Math/bin/debug

INCLUDEPATH += -L$$PWD/lib/OpenIndy-Core/lib/OpenIndy-Math/bin/release
DEPENDPATH += -L$$PWD/lib/OpenIndy-Core/lib/OpenIndy-Math/bin/release

SOURCES += \
    main.cpp \
    ui/mainwindow.cpp \
    ui/dialogs/createfeaturedialog.cpp \
    controller/console.cpp \
    controller/controller.cpp \
    controller/modelmanager.cpp \
    controller/systemdbmanager.cpp \
    models/featuretablemodel.cpp \
    models/featuretableproxymodel.cpp \
    controller/oimetadata.cpp \
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
    ui/dialogs/pluginmanagerdialog.cpp \
    ui/dialogs/watchwindowdialog.cpp \
    controller/exchange/projectexchanger.cpp \
    ui/dialogs/measurementconfigurationdialog.cpp \
    models/measurementconfigurationmodel.cpp \
    models/measurementconfigurationproxymodel.cpp \
    ui/delegates/measurementconfigurationlistdelegate.cpp \
    models/availablefunctionslistmodel.cpp \
    models/availablefunctionslistproxymodel.cpp \
    models/observationmodel.cpp \
    models/observationproxymodel.cpp \
    models/readingmodel.cpp \
    models/readingproxymodel.cpp \
    controller/config/observationtablecolumnconfig.cpp \
    controller/config/readingtablecolumnconfig.cpp \
    ui/dialogs/actualpropertiesdialog.cpp \
    ui/dialogs/nominalpropertiesdialog.cpp \
    ui/actions/toolaction.cpp \
    controller/exchange/observationimporter.cpp \
    ui/delegates/observationtabledelegate.cpp \
    ui/delegates/readingtabledelegate.cpp \
    ui/dialogs/aboutdialog.cpp \
    ui/widgets/glviewwidget.cpp \
    ui/3dengine/glwindow.cpp \
    ui/3dengine/scenebuilder.cpp

HEADERS  += \
    ui/mainwindow.h \
    ui/dialogs/createfeaturedialog.h \
    controller/console.h \
    controller/controller.h \
    controller/modelmanager.h \
    controller/systemdbmanager.h \
    models/featuretablemodel.h \
    models/featuretableproxymodel.h \
    controller/config/featuretablecolumnconfig.h \
    controller/config/trafoparamtablecolumnconfig.h \
    controller/config/parameterdisplayconfig.h \
    models/trafoparamtableproxymodel.h \
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
    ui/dialogs/pluginmanagerdialog.h \
    ui/dialogs/watchwindowdialog.h \
    controller/exchange/projectexchanger.h \
    ui/dialogs/measurementconfigurationdialog.h \
    models/measurementconfigurationmodel.h \
    models/measurementconfigurationproxymodel.h \
    ui/delegates/measurementconfigurationlistdelegate.h \
    models/availablefunctionslistmodel.h \
    models/availablefunctionslistproxymodel.h \
    models/observationmodel.h \
    models/observationproxymodel.h \
    models/readingmodel.h \
    models/readingproxymodel.h \
    controller/config/observationtablecolumnconfig.h \
    controller/config/readingtablecolumnconfig.h \
    ui/dialogs/actualpropertiesdialog.h \
    ui/dialogs/nominalpropertiesdialog.h \
    ui/actions/toolaction.h \
    controller/exchange/observationimporter.h \
    ui/delegates/observationtabledelegate.h \
    ui/delegates/readingtabledelegate.h \
    lib/OpenIndy-Core/lib/OpenIndy-Math/include/chooselalib.h \
    lib/OpenIndy-Core/lib/OpenIndy-Math/include/global.h \
    lib/OpenIndy-Core/lib/OpenIndy-Math/include/linearalgebra.h \
    lib/OpenIndy-Core/lib/OpenIndy-Math/include/oimat.h \
    lib/OpenIndy-Core/lib/OpenIndy-Math/include/oivec.h \
    ui/dialogs/aboutdialog.h \
    ui/widgets/glviewwidget.h \
    ui/3dengine/glwindow.h \
    ui/3dengine/scenebuilder.h

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
    ui/dialogs/pluginmanagerdialog.ui \
    ui/dialogs/watchwindowdialog.ui \
    ui/dialogs/measurementconfigurationdialog.ui \
    ui/dialogs/actualpropertiesdialog.ui \
    ui/dialogs/nominalpropertiesdialog.ui \
    ui/dialogs/aboutdialog.ui
