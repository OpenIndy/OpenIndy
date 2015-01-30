#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QDebug>
#include <QObject>
#include <QList>
#include <QThread>
#include <QMap>
#include <QtSql/QSqlQueryModel>
#include <QStandardItemModel>
#include <QDir>
#include <QApplication>
#include <QStringListModel>
#include <QDomDocument>
#include <QWidget>

#include "configuration.h"
#include "console.h"
#include "featuretablemodel.h"
#include "measurementconfig.h"
#include "pluginloader.h"
#include "oimetadata.h"
#include "systemdbmanager.h"

#include "featuretreeviewmodel.h"
#include "availableelementstreeviewproxymodel.h"
#include "featuregraphicstreeviewproxymodel.h"
#include "usedelementsmodel.h"

#include "point.h"
#include "pointcloud.h"
#include "plane.h"
#include "line.h"
#include "sphere.h"
#include "trafoparam.h"

#include "featurewrapper.h"
#include "scalarentityangle.h"
#include "scalarentitydistance.h"
#include "scalarentitytemperature.h"
#include "scalarentitymeasurementseries.h"

#include "featuretableproxymodel.h"
#include "trafoparamproxymodel.h"
#include "featuretreeitem.h"

#include "plugintreeviewmodel.h"

#include "featureupdater.h"

#include "oiprojectdata.h"

#include "oiemitter.h"

#include "deletefeaturesfunctor.h"

#include "featureattributesexchange.h"
#include "nominalattributeexchange.h"

#include "oifeaturestate.h"
#include "oiprojectdata.h"
#include "oiprojectexchanger.h"

#include "oiserver.h"
#include "oiwebsocketserver.h"
#include "oirequesthandler.h"

#include "pointfeaturemodel.h"
#include "pointfeaturefiltermodel.h"

#include "pi_oiexchangedefinedformat.h"
#include "pi_oiexchangesimpleascii.h"

#include "oimodelmanager.h"

class Feature;
class CoordinateSystem;
class Station;
class LaserTracker;

/*!
 * \brief The Controller class
 *
 * This class handles all controlls and is the interface for the GUI. GUI elements only need to
 * call functions from the controller.
 */
class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);

signals:

    //##################################################
    //signals to inform GUI about OpenIndy state changes
    //##################################################

    void featureSetChanged();
    void featureAttributesChanged();

    void activeFeatureChanged();
    void activeStationChanged();
    void activeCoordinateSystemChanged();
    void activeGroupChanged();

    void availableGroupNamesChanged();

public slots:

    //###########################################
    //actions to be performed (triggered by user)
    //###########################################

    void setActiveFeature(int featureId);
    void setActiveStation();
    void setActiveCoordinateSystem();

    void startMeasurement();
	void addMeasurement();
    void startMove(Reading *parameter);
    void startAim();
    void startConnect();
    void startDisconnect();
    void startToggleSight();
    void startInitialize();
    void startHome();
    void startCompensation();
    void startChangeMotorState();
    void startCustomAction(QString s);

    void recalcAll();
    void recalcActiveFeature();
    void recalcFeature(Feature *f);
    void recalcTrafoParam(TrafoParam *tp);

    bool saveProject();
    bool loadProject(QString projectName, QIODevice *myDevice);

private slots:

    void connectStateChanges();
    bool createDefaultProject();

private:

    //###################################################################
    //instances of manager classes to take care of OpenIndy state changes
    //###################################################################

    OiFeatureState *myFeatureState;
    FeatureUpdater *myFeatureUpdater;
    OiConfigState *myConfigState;
    OiModelManager *myModelManager;

    //----------------------------------------------------

public:


    Console *c;
    Configuration conf;

    QStringList getAvailableCreateFunctions(Configuration::FeatureTypes featureType); //all fit & construct functions for a feature type
    QString getDefaultFunction(Configuration::FeatureTypes featureType); //the default function or empty string for a feature type

    OiServer *openIndyServer;
    OiWebSocketServer *openIndyWebSocketServer;

signals:
    void changedStation();
    void featureAdded();
    void refreshGUI();
    void sendSQLModel(QSqlQueryModel*); //kommt raus wenn dialoge angepasst
    void sensorWorks(QString);
    void CoordSystemsModelChanged();

    void sendFunctionDescription(QString); //set description for function plugin loader dialog
    void sendAvailableElementsFilter(Configuration::ElementTypes typeOfElement, bool hideAll); //send filter for display available elements treeview
    void sendPositionOfActiveFeature(double x, double y, double z);

    void showMessageBox(QString title, QString message);
    void showMessageBoxForDecision(QString title, QString message, OiFunctor *func);
    void sendTempSensor(Sensor *s);
    void resetFeatureSelection();

    void sendExtraParameterForFunction(QMap<QString, int> intParameter, QMap<QString, double> doubleParameter,
                                       QMap<QString, QStringList> stringParameter, FunctionConfiguration config); //connected with function plugin loader

    void availableGroupsChanged(QMap<QString, int> availableGroups);

    void updateGeometryIcons(QStringList availableGeometries);

    void sendXmlRequest(OiRequestResponse *request); //connected to OiRequestHandler to do tasks like savin, loading etc.

    void isConnected(bool);

    void setSensorState(int sensorState, QString msg);

    void sensorDisconnected();

    void openOiToolWidget(OiTool* oiToolWidget);

    void showWatchWindow();
    void closeWatchWindow();

public slots:
    void setActiveGroup(QString group);

    void getNominalValues(NominalAttributeExchange nominalValue);
    int checkActiveFeatureIndex(int current, int index);

    void setActiveCoordSystem(QString CoordSysName);
    void addFeature(FeatureAttributesExchange fae);

    void emitShowWatchWindow();
    void emitCloseWatchWindow();

    void changeActiveStation(bool setSensor);
    void showResults(bool);

    void savePluginData(PluginMetaData* metaInfo);

    void setSelectedFeature(int featureIndex);
    void receiveSensorConfiguration(SensorConfiguration sc, bool connect);

    void createFunction(int index);
    void deleteFunctionFromFeature(int index);

    bool checkSensorValid();
    bool checkFeatureValid();

    void addElement2Function(FeatureTreeItem *element, int functionIndex, int elementIndex); //add element to the active function
    void removeElementFromFunction(FeatureTreeItem *element, int functionIndex, int elementIndex); //remove element from the active function

    bool receiveRequestResult(OiRequestResponse *request); //called from OiRequestHandler with the result of save or load task



    void setFunctionConfiguration(int functionIndex, FunctionConfiguration config);

    void printToConsole(QString message);

    void deleteFeatures(QList<FeatureWrapper*>);

    void deleteFeaturesCallback(bool);

    bool checkPluginAvailability(Configuration::FeatureTypes typeOfFeature);

    void updateFeatureMConfig();

    void sendIsConnected(bool b);

    void sendSensorState(int sState, QString msg);

    //database function
    QMultiMap<QString,QString> getOiTools();
    void loadOiToolWidget(QString pluginName,QString toolName);


    bool generateActualForNominal(FeatureWrapper* f);

private:
    bool checkCircleWarning(Feature *activeFeature, Feature *usedForActiveFeature);

    void connectModels();

    int lastRequestId; //id of the last OiRequest (save or load)

    QList<FeatureWrapper*> featuresToDelete;

    DeleteFeaturesFunctor *myDeleteFeaturesCallback;
};

#endif // CONTROLLER_H
