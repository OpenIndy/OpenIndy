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

#include "featureovserviewproxymodel.h"
#include "trafoparamproxymodel.h"
#include "featuretreeitem.h"

#include "featureupdater.h"

#include "oiprojectdata.h"

#include "oiemitter.h"

#include "deletefeaturesfunctor.h"

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

    QList<FeatureWrapper*> features;
    FeatureWrapper *activeFeature;
    QList<CoordinateSystem*> coordSys;
    QList<Station*> stations;
    Station *activeStation;
    CoordinateSystem *activeCoordinateSystem;
    QMap<QString, int> availableGroups;

    MeasurementConfig *lastmConfig;
    TableModel *tblModel;
    LaserTracker *lt; //test
    Console *c;
    Configuration conf;
    QSqlQueryModel *pluginsModel;
    FeatureOvserviewProxyModel *featureOverviewModel;
    QSqlQueryModel *neededElementsModel;
    TrafoParamProxyModel * trafoParamModel;

    QStandardItemModel *functionTreeViewModel; //model for treeview with functions of selected feature
    FeatureTreeViewModel *featureTreeViewModel; //model for treeview with all features
    AvailableElementsTreeViewProxyModel *availableElementsModel; //model for available elements with featureTreeViewModel as source model
    FeatureGraphicsTreeViewProxyModel *featureGraphicsModel; //model for treeview with features in graphics view with featureTreeViewModel as source model
    UsedElementsModel *usedElementsModel; //model for listview with elements that are used for a function

signals:
    void changedStation();
    void featureAdded();
    void refreshGUI(FeatureWrapper *fW, Station *sT);
    void sendSQLModel(QSqlQueryModel*); //kommt raus wenn dialoge angepasst
    void sensorWorks(QString);
    void CoordSystemAdded();

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

public slots:

    void getNominalValues(double nomX, double nomY, double nomZ, double nomI, double nomJ, double nomK, double nomR, double nomSDE, double nomSAE, double nomSTE, double nomSMSE);
    void handleTrafoParamClicked(const QModelIndex &);
    int getActiveFeatureIndex(int index);
    int checkActiveFeatureIndex(int current, int index);

    void setActiveCoordSystem(QString CoordSysName);
    void addFeature(int count, int featureType, QString name, QString group, bool actualNominal, bool isCommonPoint, CoordinateSystem *nominalSystem);
    void addScalarEntity(int count, int featureType, QString name, QString group, bool actual, bool commonPoint, double value, CoordinateSystem *nominalSystem);
    void addTrafoParam(int count, int featureType, QString name,
                        CoordinateSystem *startSystem, CoordinateSystem *destSystem);

    //sensor function
    void startMeasurement();
    void startMove(Reading *parameter);
    void startAim();
    void startConnect();
    void startDisconnect();
    void startToggleSight();
    void sendCmdString(QString cmd);
    void startInitialize();
    void startHome();
    void startCompensation();
    void startChangeMotorState();

    void recalcActiveFeature();
    void recalcFeature(Feature *f);
    void recalcTrafoParam(TrafoParam *tp);
    void changeActiveStation();
    void showResults(bool);
    void defaultLastmConfig();
    void savePluginData(PluginMetaData* metaInfo);
    void setSensorModel(Configuration::SensorTypes);
    void getSelectedPlugin(int index);
    void getTempSensor(int index);
    void getSelectedFeature(int index);
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

    void loadProjectData(oiProjectData &data);
    void setFunctionConfiguration(int functionIndex, FunctionConfiguration config);

    void printToConsole(QString message);

    void deleteFeatures(QList<FeatureWrapper*>);
    void deleteFeaturesCallback(bool);

    void groupNameChanged(QString oldValue, QString newValue);

private:
    void changeFunctionTreeViewModel();
    void changeUsedElementsModel(int functionIndex, int elementIndex);
    bool checkCircleWarning(Feature *activeFeature, Feature *usedForActiveFeature);

    FeatureUpdater myFeatureUpdater;
    QList<FeatureWrapper*> featuresToDelete;

    DeleteFeaturesFunctor *myDeleteFeaturesCallback;
};

#endif // CONTROLLER_H
