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

#include "configuration.h"
#include "console.h"
#include "tablemodel.h"
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

#include "featureoverviewproxymodel.h"
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

    OiProjectData currentProject; //holds the currently opened OpenIndy-project

    OiFeatureState *myFeatureState;

    QList<MeasurementConfig> measurementConfigs; //all available measurement configs (saved & unsaved)
    MeasurementConfig lastmConfig;

    TableModel *tblModel; //base model for feature table
    FeatureOverviewProxyModel *featureOverviewModel; //filtered feature table (no trafoParams)
    TrafoParamProxyModel * trafoParamModel; //filtered feature table (only trafoParams)

    Console *c;
    Configuration conf;
    QSqlQueryModel *pluginsModel;
    QSqlQueryModel *neededElementsModel;

    QStandardItemModel *functionTreeViewModel; //model for treeview with functions of selected feature
    FeatureTreeViewModel *featureTreeViewModel; //model for treeview with all features
    AvailableElementsTreeViewProxyModel *availableElementsModel; //model for available elements with featureTreeViewModel as source model
    FeatureGraphicsTreeViewProxyModel *featureGraphicsModel; //model for treeview with features in graphics view with featureTreeViewModel as source model
    UsedElementsModel *usedElementsModel; //model for listview with elements that are used for a function
    PluginTreeViewModel *myPluginTreeViewModel; //model with all available plugins for plugin manager
    QStringListModel *myFeatureGroupsModel; //model with all available groups
    QStringListModel *myCoordinateSystemsModel; //model with coordinate systems

    QStringList getAvailableCreateFunctions(Configuration::FeatureTypes featureType); //all fit & construct functions for a feature type
    QString getDefaultFunction(Configuration::FeatureTypes featureType); //the default function or empty string for a feature type

    FeatureUpdater* getFeatureUpdater();

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

    void activeCoordinateSystemChanged();

public slots:
    void setUpFeatureGroupsModel();
    void setUpCoordinateSystemsModel();
    void setActiveGroup(QString group);

    void getNominalValues(NominalAttributeExchange nominalValue);
    int checkActiveFeatureIndex(int current, int index);

    void setActiveCoordSystem(QString CoordSysName);
    void addFeature(FeatureAttributesExchange fae);

    //sensor function
    void startMeasurement();
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
    void changeActiveStation(bool setSensor);
    void showResults(bool);
    void defaultLastmConfig();
    void savePluginData(PluginMetaData* metaInfo);
    void setSensorModel(Configuration::SensorTypes);
    void getSelectedPlugin(int index);
    void getTempSensor(int index);
    void setSelectedFeature(int featureIndex);
    void receiveSensorConfiguration(SensorConfiguration *sc, bool connect);
    void receiveFunctionId(int id);

    void setFunction();
    void createFunction(int index);
    void deleteFunctionFromFeature(int index);

    bool checkSensorValid();
    bool checkFeatureValid();

    void importFeatures(QList<FeatureWrapper*> f);

    void setSelectedFunction(int functionIndex, int neededElementIndex); //receive selected function from function plugin dialog
    void addElement2Function(FeatureTreeItem *element, int functionIndex, int elementIndex); //add element to the active function
    void removeElementFromFunction(FeatureTreeItem *element, int functionIndex, int elementIndex); //remove element from the active function

    //save & load an OpenIndy project
    bool saveProject();
    bool loadProject(OiProjectData &projectData);
    bool createProject(OiProjectData &projectData);

    void setFunctionConfiguration(int functionIndex, FunctionConfiguration config);

    void printToConsole(QString message);

    void deleteFeatures(QList<FeatureWrapper*>);

    void deleteFeaturesCallback(bool);

    //void groupNameChanged(QString oldValue, QString newValue);

    void checkAvailablePlugins();
    bool checkPluginAvailability(Configuration::FeatureTypes typeOfFeature);

    void updateFeatureMConfig();

   /* void createFeature(int featureType, QString name,QString group,  bool nominal, bool common,
                       CoordinateSystem *nominalSystem, CoordinateSystem *startSystem, CoordinateSystem *destSystem);
*/
    //void addNominalToActual(FeatureWrapper *fw);
    //void checkForNominals(FeatureWrapper *fw);

    //void sortFeatures();

private slots:
    void changeFunctionTreeViewModel();

private:
    void changeUsedElementsModel(int functionIndex, int elementIndex);
    bool checkCircleWarning(Feature *activeFeature, Feature *usedForActiveFeature);

    void initModels();
    void connectModels();
    //void createDefaultFeatures();

    FeatureUpdater myFeatureUpdater;

    QList<FeatureWrapper*> featuresToDelete;

    DeleteFeaturesFunctor *myDeleteFeaturesCallback;
};

#endif // CONTROLLER_H
