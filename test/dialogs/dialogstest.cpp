#include <QString>
#include <QtTest>
#include <QPointer>
#include <QList>
#include <QStringList>
#include <QStringListModel>
#include "createfeaturedialog.h"
#include "availablefunctionslistproxymodel.h"
#include "measurementconfigurationdialog.h"
#include "projectexchanger.h"
#include "controller.h"

// recomended readings:
// https://gist.github.com/peteristhegreat/cbd8eaa0e565d0b82dbfb5c7fdc61c8d
// https://vicrucann.github.io/tutorials/qttest-signals-qtreewidget/

using namespace oi;

QDebug operator<<(QDebug debug, const QDomDocument &dom) {
    QByteArray arr;
    QTextStream stream(&arr);
    dom.save(stream, 2 /*indent*/);
    stream.flush();
    debug.noquote() << arr;
    return debug;
}

class DialogsTest : public QObject
{
    Q_OBJECT

public:
    DialogsTest();

private Q_SLOTS:
    void initTestCase();

    // functions my change global list of MeasurementConfigs
    // !!! keep order !!!
    void measurementConfigurationModel();

    void createPoint();
    void createCircle();
    void createPlane();
    void createPlaneFromPoints();
    void createLevel();
    void reuseDialogInstance();

    void measurementConfigXML_RW();
    void measurementConfigFilter();

    void measurementConfigDialog();

    void projectSaveLoad();

private:

    void printMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);
    QStringList getNames(QSortFilterProxyModel *model);
    QStringList getNames(QAbstractListModel *model);
    QStringList getNames(QAbstractItemModel *model);
    QStringList getNames(QList<MeasurementConfig> list);
    QStringList getKeys(QList<MeasurementConfig> list);
};

DialogsTest::DialogsTest() {

}

void DialogsTest::printMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest) {
    qDebug() << msg;
}

void DialogsTest::initTestCase() {
    // create in memory database
    QFile sqlFile1(INIT_SQL);
    sqlFile1.open(QFile::ReadOnly | QFile::Text);
    QTextStream in1(&sqlFile1);
    QStringList statements = in1.readAll().split(";");

    SystemDbManager::initInMemoryDB(statements);


    QStringList entityTypes;
    entityTypes << "point" << "circle" << "plane"; // comboBox_entityType order

    QPointer<MeasurementConfigManager> measurementConfigManager = new MeasurementConfigManager();

    QObject::connect(measurementConfigManager.data(), &MeasurementConfigManager::sendMessage,
                     this, &DialogsTest::printMessage);
/*    QObject::connect(&ModelManager::getMeasurementConfigurationModel(), &MeasurementConfigurationModel::sendMessage,
                     this, &DialogsTest::printMessage);
    QObject::connect(&ModelManager::getMeasurementConfigurationProxyModel(), &MeasurementConfigurationProxyModel::sendMessage,
                     this, &DialogsTest::printMessage);*/

    // user config
    MeasurementConfig scanDistanceConfig;
    scanDistanceConfig.setName("measconfig-scandistance"); // not for point
    scanDistanceConfig.setMeasurementType(MeasurementTypes::eScanDistanceDependent_MeasurementType);
    scanDistanceConfig.setDistanceInterval(456);
    scanDistanceConfig.setMaxObservations(654);
    scanDistanceConfig.makeUserConfig();
    measurementConfigManager->saveUserConfig(scanDistanceConfig);

    MeasurementConfig fastPointConfig;
    fastPointConfig.setName("FastPoint");
    fastPointConfig.setMeasurementMode(MeasurementModes::eFast_MeasurementMode);
    fastPointConfig.setMeasurementType(MeasurementTypes::eSinglePoint_MeasurementType);
    fastPointConfig.makeUserConfig();
    measurementConfigManager->saveUserConfig(fastPointConfig);

    // project config
    MeasurementConfig fastPointConfigProject2;
    fastPointConfigProject2.setName("measconfig-fastpoint_project");
    fastPointConfigProject2.setMeasurementMode(MeasurementModes::eFast_MeasurementMode);
    fastPointConfigProject2.setMeasurementType(MeasurementTypes::eSinglePoint_MeasurementType);
    fastPointConfigProject2.makeProjectConfig();
    measurementConfigManager->addProjectConfig(fastPointConfigProject2);

    // create plugin with some functions
    QList<sdb::Plugin> plugins;
    sdb::Plugin testPlugin;
    testPlugin.name = "testplugin";
    testPlugin.author = "esc";


    sdb::Function point;
    point.name = "function-fitpoint";
    point.iid = OiMetaData::iid_FitFunction;
    point.applicableFor << FeatureTypes::ePointFeature;
    point.neededElements << ElementTypes::eObservationElement;

    sdb::Function plane;
    plane.name = "function-fitplane";
    plane.iid = OiMetaData::iid_FitFunction;
    plane.applicableFor << FeatureTypes::ePlaneFeature;
    plane.neededElements << ElementTypes::eObservationElement;

    sdb::Function level;
    level.name = "function-fitlevel";
    level.iid = OiMetaData::iid_FitFunction;
    level.applicableFor << FeatureTypes::ePlaneFeature << FeatureTypes::eLevelFeature;
    level.neededElements << ElementTypes::eObservationElement;

    sdb::Function planefrompoints;
    planefrompoints.name = "function-planefrompoints";
    planefrompoints.iid = OiMetaData::iid_ConstructFunction; // set measurement config combox to visible = false
    planefrompoints.applicableFor << FeatureTypes::ePlaneFeature;
    planefrompoints.neededElements << ElementTypes::eObservationElement;

    sdb::Function circle;
    circle.name = "function-fitcircle";
    circle.iid = OiMetaData::iid_FitFunction;
    circle.applicableFor << FeatureTypes::eCircleFeature;
    circle.neededElements << ElementTypes::eObservationElement;

    testPlugin.functions << plane << point << level << circle << planefrompoints;

    plugins << testPlugin;

    // add plugin to database
    SystemDbManager::addPlugin(testPlugin);


    // init ModelManager
    ModelManager::testInit(entityTypes, measurementConfigManager, plugins);

}

void DialogsTest::createPoint() {

    // create dialog
    CreateFeatureDialog dialog;

    // comboBox_entityType currently not usesd, type is set directly
    dialog.setFeatureType(FeatureTypes::ePointFeature);
    dialog.show(); // to call: void showEvent(QShowEvent *event); and initialize dialog
    QSignalSpy spy_initialized(&dialog, SIGNAL(initialized()));
    spy_initialized.wait(500);

    // check function
    QPointer<QComboBox> functionCB;
    QPointer<QListView> functionLV;

    // check for default function
    functionCB = dialog.findChild<QComboBox *>("comboBox_function");
    functionLV = functionCB->findChild<QListView *>();
    qDebug() << "rowCount" << functionLV->model()->rowCount();
    qDebug() << functionLV->model()->index(0,0).data( Qt::DisplayRole ).toString();
    qDebug() << "currentIndex" << functionCB->currentIndex();
    QVERIFY("function-fitpoint" == functionLV->model()->index(0,0).data( Qt::DisplayRole ).toString());

    // check applicable measurement configs
    QPointer<QComboBox> mConfigCB = dialog.findChild<QComboBox *>("comboBox_mConfig");
    QPointer<QListView> mConfigLV = mConfigCB->findChild<QListView *>();
    qDebug() << getNames(mConfigLV->model());
    QVERIFY(7 == mConfigLV->model()->rowCount());
    QVERIFY("FastPoint [user]"                  == mConfigLV->model()->index(0, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("FastPoint"                         == mConfigLV->model()->index(1, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("measconfig-fastpoint_project"      == mConfigLV->model()->index(2, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("measconfig-scandistance [user]"    == mConfigLV->model()->index(3, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("PrecisePoint"                      == mConfigLV->model()->index(4, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("StdPoint"                          == mConfigLV->model()->index(5, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("StdTwoSide"                        == mConfigLV->model()->index(6, 0).data( Qt::DisplayRole ).toString());

}

void DialogsTest::createCircle() {
    // create dialog
    CreateFeatureDialog dialog;

    // comboBox_entityType currently not usesd, type is set directly
    dialog.setFeatureType(FeatureTypes::eCircleFeature);
    dialog.show(); // to call: void showEvent(QShowEvent *event); and initialize dialog
    QSignalSpy spy_initialized(&dialog, SIGNAL(initialized()));
    spy_initialized.wait(500);

    // check function
    QPointer<QComboBox> functionCB;
    QPointer<QListView> functionLV;

    // check for default function
    functionCB = dialog.findChild<QComboBox *>("comboBox_function");
    functionLV = functionCB->findChild<QListView *>();
    qDebug() << "rowCount" << functionLV->model()->rowCount();
    qDebug() << functionLV->model()->index(0,0).data( Qt::DisplayRole ).toString();
    qDebug() << "currentIndex" << functionCB->currentIndex();
    QVERIFY("function-fitcircle" == functionLV->model()->index(0,0).data( Qt::DisplayRole ).toString());

    // check applicable measurement configs
    QPointer<QComboBox> mConfigCB = dialog.findChild<QComboBox *>("comboBox_mConfig");
    QPointer<QListView> mConfigLV = mConfigCB->findChild<QListView *>();
    qDebug() << getNames(mConfigLV->model());
    QVERIFY(7 == mConfigLV->model()->rowCount());
    QVERIFY("FastPoint [user]"                  == mConfigLV->model()->index(0, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("FastPoint"                         == mConfigLV->model()->index(1, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("measconfig-fastpoint_project"      == mConfigLV->model()->index(2, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("measconfig-scandistance [user]"    == mConfigLV->model()->index(3, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("PrecisePoint"                      == mConfigLV->model()->index(4, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("StdPoint"                          == mConfigLV->model()->index(5, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("StdTwoSide"                        == mConfigLV->model()->index(6, 0).data( Qt::DisplayRole ).toString());

}

void DialogsTest::createPlane() {
    // create dialog
    CreateFeatureDialog dialog;

    // comboBox_entityType currently not usesd, type is set directly
    dialog.setFeatureType(FeatureTypes::ePlaneFeature);
    dialog.show(); // to call: void showEvent(QShowEvent *event); and initialize dialog
    QSignalSpy spy_initialized(&dialog, SIGNAL(initialized()));
    spy_initialized.wait(500);

    // check function
    QPointer<QComboBox> functionCB;
    QPointer<QListView> functionLV;

    // check for default function
    functionCB = dialog.findChild<QComboBox *>("comboBox_function");
    functionLV = functionCB->findChild<QListView *>();
    qDebug() << "rowCount" << functionLV->model()->rowCount();
    qDebug() << functionLV->model()->index(0,0).data( Qt::DisplayRole ).toString();
    qDebug() << "currentIndex" << functionCB->currentIndex();
    QVERIFY("function-fitplane" == functionLV->model()->index(functionCB->currentIndex(),0).data( Qt::DisplayRole ).toString());

    // check applicable measurement configs
    QPointer<QComboBox> mConfigCB = dialog.findChild<QComboBox *>("comboBox_mConfig");
    QPointer<QListView> mConfigLV = mConfigCB->findChild<QListView *>();
    qDebug() << getNames(mConfigLV->model());
    QVERIFY(7 == mConfigLV->model()->rowCount());
    QVERIFY("FastPoint [user]"                  == mConfigLV->model()->index(0, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("FastPoint"                         == mConfigLV->model()->index(1, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("measconfig-fastpoint_project"      == mConfigLV->model()->index(2, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("measconfig-scandistance [user]"    == mConfigLV->model()->index(3, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("PrecisePoint"                      == mConfigLV->model()->index(4, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("StdPoint"                          == mConfigLV->model()->index(5, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("StdTwoSide"                        == mConfigLV->model()->index(6, 0).data( Qt::DisplayRole ).toString());

}

void DialogsTest::createPlaneFromPoints() {
    // create dialog
    CreateFeatureDialog dialog;

    // comboBox_entityType currently not usesd, type is set directly
    dialog.setFeatureType(FeatureTypes::ePlaneFeature);
    dialog.show(); // to call: void showEvent(QShowEvent *event); and initialize dialog
    QSignalSpy spy_initialized(&dialog, SIGNAL(initialized()));
    spy_initialized.wait(500);

    // check function
    QPointer<QComboBox> functionCB;
    QPointer<QListView> functionLV;

    // check for default function
    functionCB = dialog.findChild<QComboBox *>("comboBox_function");
    functionLV = functionCB->findChild<QListView *>();
    qDebug() << "rowCount" << functionLV->model()->rowCount();
    qDebug() << functionLV->model()->index(0,0).data( Qt::DisplayRole ).toString();
    qDebug() << "currentIndex" << functionCB->currentIndex();
    QVERIFY("function-fitplane" == functionLV->model()->index(functionCB->currentIndex(),0).data( Qt::DisplayRole ).toString());

    int i=0;
    for(i=0; i<functionLV->model()->rowCount(); i++) {
        if("function-planefrompoints" == functionLV->model()->index(i,0).data( Qt::DisplayRole ).toString()) {
            break;
        }
    }

    // select "function-planefrompoints"
    QTest::mouseClick(functionCB, Qt::LeftButton);
    QTest::qWait(500); // TODO spy

    QModelIndex idx = functionLV->model()->index(i,0);
    functionLV->scrollTo(idx);


    QPoint itemPt = functionLV->visualRect(idx).center();
    QString functionName = functionLV->model()->index(i,0).data( Qt::DisplayRole ).toString();
    qDebug() << "clicking on function" << functionName;
    QVERIFY("function-planefrompoints" == functionName);

    QTest::mouseClick(functionLV->viewport(), Qt::LeftButton, 0, itemPt);
    QTest::qWait(500);

    // check applicable measurement configs
    QPointer<QComboBox> mConfigCB = dialog.findChild<QComboBox *>("comboBox_mConfig");
    QPointer<QListView> mConfigLV = mConfigCB->findChild<QListView *>();
    qDebug() << getNames(mConfigLV->model());

    QVERIFY(0 == mConfigLV->model()->rowCount());

}

void DialogsTest::createLevel() {
    // create dialog
    CreateFeatureDialog dialog;

    // comboBox_entityType currently not usesd, type is set directly
    dialog.setFeatureType(FeatureTypes::ePlaneFeature);
    dialog.show(); // to call: void showEvent(QShowEvent *event); and initialize dialog
    QSignalSpy spy_initialized(&dialog, SIGNAL(initialized()));
    spy_initialized.wait(500);

    // check function
    QPointer<QComboBox> functionCB;
    QPointer<QListView> functionLV;

    // check for default function
    functionCB = dialog.findChild<QComboBox *>("comboBox_function");
    functionLV = functionCB->findChild<QListView *>();
    qDebug() << "rowCount" << functionLV->model()->rowCount();
    qDebug() << functionLV->model()->index(0,0).data( Qt::DisplayRole ).toString();
    qDebug() << "currentIndex" << functionCB->currentIndex();
    QVERIFY(3 == functionLV->model()->rowCount());
    QVERIFY("function-fitplane" == functionLV->model()->index(functionCB->currentIndex(),0).data( Qt::DisplayRole ).toString());


    int i=0;
    for(i=0; i<functionLV->model()->rowCount(); i++) {
        if("function-fitlevel" == functionLV->model()->index(i,0).data( Qt::DisplayRole ).toString()) {
            break;
        }
    }

    // select "function-fitlevel"
    QTest::mouseClick(functionCB, Qt::LeftButton);
    QTest::qWait(500); // TODO spy

    QModelIndex idx = functionLV->model()->index(i,0);
    functionLV->scrollTo(idx);


    QPoint itemPt = functionLV->visualRect(idx).center();
    QString functionName = functionLV->model()->index(i,0).data( Qt::DisplayRole ).toString();
    qDebug() << "clicking on function" << functionName;
    QVERIFY("function-fitlevel" == functionName);

    QTest::mouseClick(functionLV->viewport(), Qt::LeftButton, 0, itemPt);
    QTest::qWait(500);

    // check applicable measurement configs
    QPointer<QComboBox> mConfigCB = dialog.findChild<QComboBox *>("comboBox_mConfig");
    QPointer<QListView> mConfigLV = mConfigCB->findChild<QListView *>();
    qDebug() << getNames(mConfigLV->model());
    QVERIFY(1 == mConfigLV->model()->rowCount());
    QVERIFY("level" == mConfigLV->model()->index(0, 0).data( Qt::DisplayRole ).toString());

}

void DialogsTest::reuseDialogInstance() {
    // create dialog
    CreateFeatureDialog dialog;

    // comboBox_entityType currently not usesd, type is set directly
    dialog.setFeatureType(FeatureTypes::ePlaneFeature);
    dialog.show(); // to call: void showEvent(QShowEvent *event); and initialize dialog
    dialog.activateWindow();
    QSignalSpy spy_initialized(&dialog, SIGNAL(initialized()));
    spy_initialized.wait(500);

    // check function
    QPointer<QComboBox> functionCB;
    QPointer<QListView> functionLV;

    // check for default function
    functionCB = dialog.findChild<QComboBox *>("comboBox_function");
    functionLV = functionCB->findChild<QListView *>();
    qDebug() << "rowCount" << functionLV->model()->rowCount();
    qDebug() << functionLV->model()->index(0,0).data( Qt::DisplayRole ).toString();
    qDebug() << "currentIndex" << functionCB->currentIndex();
    QVERIFY("function-fitplane" == functionLV->model()->index(functionCB->currentIndex(),0).data( Qt::DisplayRole ).toString());

    // check applicable measurement configs
    QPointer<QComboBox> mConfigCB = dialog.findChild<QComboBox *>("comboBox_mConfig");
    QPointer<QListView> mConfigLV = mConfigCB->findChild<QListView *>();
    qDebug() << getNames(mConfigLV->model());
    QVERIFY(7 == mConfigLV->model()->rowCount());
    QVERIFY("FastPoint [user]"                  == mConfigLV->model()->index(0, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("FastPoint"                         == mConfigLV->model()->index(1, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("measconfig-fastpoint_project"      == mConfigLV->model()->index(2, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("measconfig-scandistance [user]"    == mConfigLV->model()->index(3, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("PrecisePoint"                      == mConfigLV->model()->index(4, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("StdPoint"                          == mConfigLV->model()->index(5, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("StdTwoSide"                        == mConfigLV->model()->index(6, 0).data( Qt::DisplayRole ).toString());

    dialog.close();
    QTest::qWait(500);
    dialog.show();
    dialog.activateWindow();

    // check for default function
    functionCB = dialog.findChild<QComboBox *>("comboBox_function");
    functionLV = functionCB->findChild<QListView *>();
    qDebug() << "rowCount" << functionLV->model()->rowCount();
    qDebug() << functionLV->model()->index(0,0).data( Qt::DisplayRole ).toString();
    qDebug() << "currentIndex" << functionCB->currentIndex();
    QVERIFY(3 == functionLV->model()->rowCount());
    QVERIFY("function-fitplane" == functionLV->model()->index(functionCB->currentIndex(),0).data( Qt::DisplayRole ).toString());


    int i=0;
    for(i=0; i<functionLV->model()->rowCount(); i++) {
        if("function-fitlevel" == functionLV->model()->index(i,0).data( Qt::DisplayRole ).toString()) {
            break;
        }
    }

    // select "function-fitlevel"
    QTest::mouseClick(functionCB, Qt::LeftButton);
    QTest::qWait(500); // TODO spy

    QModelIndex idx = functionLV->model()->index(i,0);
    functionLV->scrollTo(idx);


    QPoint itemPt = functionLV->visualRect(idx).center();
    QString functionName = functionLV->model()->index(i,0).data( Qt::DisplayRole ).toString();
    qDebug() << "clicking on function" << functionName;
    QVERIFY("function-fitlevel" == functionName);

    QTest::mouseClick(functionLV->viewport(), Qt::LeftButton, 0, itemPt);
    QTest::qWait(500);

    // check applicable measurement configs
    mConfigCB = dialog.findChild<QComboBox *>("comboBox_mConfig");
    mConfigLV = mConfigCB->findChild<QListView *>();
    qDebug() << getNames(mConfigLV->model());
    QVERIFY(1 == mConfigLV->model()->rowCount());
    QVERIFY("level" == mConfigLV->model()->index(0, 0).data( Qt::DisplayRole ).toString());


}

void DialogsTest::measurementConfigXML_RW() {
    MeasurementConfig config = ModelManager::getMeasurementConfigManager()->getProjectConfig("measconfig-scantime");

    QDomDocument xml("measurementConfig");
    QDomElement root = config.toOpenIndyXML(xml);
    xml.appendChild(root);

    MeasurementConfig fromXML;
    fromXML.fromOpenIndyXML(xml.documentElement());

    QVERIFY(config.getMeasurementMode() == fromXML.getMeasurementMode());
    QVERIFY(config.getMeasurementType() == fromXML.getMeasurementType());
    QVERIFY(config.getMeasureTwoSides() == fromXML.getMeasureTwoSides());
    QVERIFY(config.getMaxObservations() == fromXML.getMaxObservations());
    QVERIFY(config.getTimeInterval() == fromXML.getTimeInterval());
    QVERIFY(config.getDistanceInterval() == fromXML.getDistanceInterval());

}

QStringList DialogsTest::getNames(QSortFilterProxyModel *model) {

    QStringList names;
    for(int row=0; row<model->rowCount(); row++) {
        names <<  model->index(row, 0).data( Qt::DisplayRole ).toString();
    }
    return names;
}

QStringList DialogsTest::getNames(QAbstractListModel *model) {

    QStringList names;
    for(int row=0; row<model->rowCount(); row++) {
        names <<  model->index(row, 0).data( Qt::DisplayRole ).toString();
    }
    return names;
}

QStringList DialogsTest::getNames(QAbstractItemModel *model) {

    QStringList names;
    for(int row=0; row<model->rowCount(); row++) {
        names <<  model->index(row, 0).data( Qt::DisplayRole ).toString();
    }
    return names;
}

QStringList DialogsTest::getNames(QList<MeasurementConfig> list) {

    QStringList names;
    foreach(const MeasurementConfig mc, list) {
        names <<  mc.getName();
    }
    return names;
}

QStringList DialogsTest::getKeys(QList<MeasurementConfig> list) {
    QStringList keys;
    foreach(const MeasurementConfig mc, list) {
        Key key = mc.getKey();
        QString t;
        switch(key.getConfigType()) {
        case eUserConfig:
            t = "user";
            break;
        case eProjectConfig:
            t = "project";
            break;
        case eStandardConfig:
            t = "standard";
            break;
        }

        keys << QString("(%1, %2)").arg(key.getName()).arg(t);
    }
    return keys;
}

void DialogsTest::measurementConfigFilter() {
    MeasurementConfigurationProxyModel *proxy = &ModelManager::getMeasurementConfigurationProxyModel(); // global test instance
    MeasurementConfigurationModel *sourceModel = static_cast<MeasurementConfigurationModel *>(proxy->sourceModel());

    QStringList names;

    names = getNames(sourceModel);
    qDebug() << "source model:   " << names;

    names = getNames(proxy);
    qDebug() << "current:        " << names;
    proxy->setFilter(true);
    names = getNames(proxy);
    qDebug() << "all:            " << names;

    QList<ElementTypes> neededElements;
    QList<FeatureTypes> applicableFor;
    neededElements.append(ElementTypes::eObservationElement);
    applicableFor.append(FeatureTypes::ePointFeature);
    proxy->setFilter(neededElements, FeatureTypes::ePlaneFeature, applicableFor);
    names = getNames(proxy);
    qDebug() << "point:          " << names;

    neededElements.clear();
    applicableFor.clear();
    neededElements.append(ElementTypes::eObservationElement);
    applicableFor.append(FeatureTypes::ePlaneFeature);
    proxy->setFilter(neededElements, FeatureTypes::ePlaneFeature, applicableFor);
    names = getNames(proxy);
    qDebug() << "plane:          " << names;

    applicableFor.clear();
    applicableFor.append(FeatureTypes::eLevelFeature);
    proxy->setFilter(neededElements, FeatureTypes::ePlaneFeature, applicableFor);
    names = getNames(proxy);
    qDebug() << "level:          " << names;

    proxy->setFilterProjectConfig();
    names = getNames(proxy);
    qDebug() << "project config: " << names;
    QVERIFY(6 == proxy->rowCount());
    QVERIFY("FastPoint"                         == proxy->index(0, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("level"                             == proxy->index(1, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("measconfig-fastpoint_project"      == proxy->index(2, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("PrecisePoint"                      == proxy->index(3, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("StdPoint"                          == proxy->index(4, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("StdTwoSide"                        == proxy->index(5, 0).data( Qt::DisplayRole ).toString());

    proxy->setFilterUserConfig();
    names = getNames(proxy);
    qDebug() << "user config:    " << names;
    QVERIFY(2 == proxy->rowCount());
    QVERIFY("FastPoint"                    == proxy->index(0, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("measconfig-scandistance"      == proxy->index(1, 0).data( Qt::DisplayRole ).toString());

    proxy->setFilter(true);
    names = getNames(proxy);
    qDebug() << "all:            " << names;
    QVERIFY(8 == proxy->rowCount());
    QVERIFY("FastPoint"                         == proxy->index(0, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("FastPoint"                         == proxy->index(1, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("level"                             == proxy->index(2, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("measconfig-fastpoint_project"      == proxy->index(3, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("measconfig-scandistance"           == proxy->index(4, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("PrecisePoint"                      == proxy->index(5, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("StdPoint"                          == proxy->index(6, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("StdTwoSide"                        == proxy->index(7, 0).data( Qt::DisplayRole ).toString());
}

void DialogsTest::measurementConfigDialog() {

    MeasurementConfig projectFastPointConfig = ModelManager::getMeasurementConfigManager()->getProjectConfig("measconfig-fastpoint");

    // create dialog
    MeasurementConfigurationDialog dialog;

    //MeasurementConfig config = measurementConfigManager->getProjectConfig(projectFastPointConfig.getName());
    dialog.setMeasurementConfiguration(projectFastPointConfig);
    dialog.show(); // to call: void showEvent(QShowEvent *event); and initialize dialog
    QSignalSpy spy_initialized(&dialog, SIGNAL(initialized()));
    spy_initialized.wait(500);

    // add new config
    QPointer<QWidget> addPB = dialog.findChild<QWidget*>("pushButton_add");

    QTest::mouseClick(addPB, Qt::LeftButton);
    QTest::qWait(500); // TODO spy

    QPointer<QListView> userConfigsLV = dialog.findChild<QListView*>("listView_userConfigs");
    qDebug() <<  getNames(userConfigsLV->model());

    QVERIFY(3 == userConfigsLV->model()->rowCount());
    QVERIFY("FastPoint"                 == userConfigsLV->model()->index(0, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("measconfig-scandistance"   == userConfigsLV->model()->index(1, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("new config"                == userConfigsLV->model()->index(2, 0).data( Qt::DisplayRole ).toString());

    // select project config: "PrecisePoint"
    QPointer<QTabWidget> tabW = dialog.findChild<QTabWidget*>("tabWidget");
    tabW->setCurrentIndex(0); // TODO change tab by mouseClick ???

    QPointer<QListView> projectConfigsLV = dialog.findChild<QListView*>("listView_projectConfigs");
    qDebug() <<  getNames(projectConfigsLV->model());

    QVERIFY(6 == userConfigsLV->model()->rowCount());
    QVERIFY("FastPoint"                     == userConfigsLV->model()->index(0, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("level"                         == userConfigsLV->model()->index(1, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("measconfig-fastpoint_project"  == userConfigsLV->model()->index(2, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("PrecisePoint"                  == userConfigsLV->model()->index(3, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("StdPoint"                      == userConfigsLV->model()->index(4, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("StdTwoSide"                    == userConfigsLV->model()->index(5, 0).data( Qt::DisplayRole ).toString());


    // find index for "PrecisePoint"
    int i=0;
    for(i=0; i<projectConfigsLV->model()->rowCount(); i++) {
        if("PrecisePoint" == projectConfigsLV->model()->index(i,0).data( Qt::DisplayRole ).toString()) {
            break;
        }
    }

    // right click "PrecisePoint"
    QModelIndex idx = projectConfigsLV->model()->index(i,0);
    projectConfigsLV->scrollTo(idx);
    QPoint itemPt = projectConfigsLV->visualRect(idx).center();
    QTest::mouseClick(projectConfigsLV->viewport(), Qt::LeftButton, 0, itemPt);
    QTest::qWait(500);
    QPointer<QLabel> configNameL = dialog.findChild<QLabel*>("label_configName");
    QVERIFY("PrecisePoint" == configNameL->text());

    // clean up
    ModelManager::getMeasurementConfigManager()->removeUserConfig("new config");
}

void DialogsTest::measurementConfigurationModel() {
    MeasurementConfigurationProxyModel *proxy = &ModelManager::getMeasurementConfigurationProxyModel(); // global test instance
    MeasurementConfigurationModel *sourceModel = static_cast<MeasurementConfigurationModel *>(proxy->sourceModel());

    proxy->setFilter(true);
    qDebug() << getNames(proxy).join(", ");
    QVERIFY("FastPoint, FastPoint, level, measconfig-fastpoint_project, measconfig-scandistance, PrecisePoint, StdPoint, StdTwoSide" == getNames(proxy).join(", "));

    proxy->setFilterUserConfig();

    qDebug() << getNames(proxy);
    QVERIFY("FastPoint, measconfig-scandistance" == getNames(proxy).join(", "));
    // rename
    QString value("new_name");
    QModelIndex index = proxy->index(0,0);
    MeasurementConfig backup = sourceModel->getMeasurementConfig(proxy->mapToSource(index));
    proxy->setData(index, value, Qt::EditRole);

    proxy->setFilterUserConfig();
    qDebug() << getNames(proxy);
    qDebug() << getNames(proxy).join(", ");
    QVERIFY("measconfig-scandistance, new_name" == getNames(proxy).join(", "));

    // remove
    index = proxy->index(1,0);
    sourceModel->removeMeasurementConfig(proxy->mapToSource(index));

    proxy->setFilterUserConfig();
    qDebug() << getNames(proxy).join(", ");
    QVERIFY("measconfig-scandistance" == getNames(proxy).join(", "));


    // clean up / previouse content
    ModelManager::getMeasurementConfigManager()->saveUserConfig(backup);
    // proxy->setFilterUserConfig();
    qDebug() << getNames(proxy).join(", ");
    QVERIFY("FastPoint, measconfig-scandistance" == getNames(proxy).join(", "));

    proxy->setFilter(true);
    qDebug() << getNames(proxy).join(", ");
    QVERIFY("FastPoint, FastPoint, level, measconfig-fastpoint_project, measconfig-scandistance, PrecisePoint, StdPoint, StdTwoSide" == getNames(proxy).join(", "));
}

void DialogsTest::projectSaveLoad() {

    OiJob jobS;

    QPointer<FeatureWrapper> stationFeature = new FeatureWrapper();
    QPointer<Station> station = new Station();
    station->setFeatureName("STATION01");
    stationFeature->setStation(station);
    jobS.addFeature(stationFeature);

    QPointer<FeatureWrapper> systemFeature = new FeatureWrapper();
    QPointer<CoordinateSystem> system = new CoordinateSystem();
    system->setFeatureName("PART");
    systemFeature->setCoordinateSystem(system);
    jobS.addFeature(systemFeature);

    //activate features
    station->setActiveStationState(true);
    station->getCoordinateSystem()->setActiveCoordinateSystemState(true);

    //create feature attributes
    FeatureAttributes attr;
    attr.count = 1;
    attr.typeOfFeature = eCoordinateSystemFeature;
    attr.name = "Bundle01";
    attr.isBundleSystem = true;

    //add feature
    jobS.addFeatures(attr);

    FeatureAttributes point1;
    point1.count = 1;
    point1.isActual = true;
    point1.typeOfFeature = ePointFeature;
    point1.name = "Point01";
    point1.measurementConfig = ModelManager::getMeasurementConfigManager()->getProjectConfig("measconfig-fastpoint_project");
    QVERIFY(point1.measurementConfig.isValid());
    point1.functionPlugin = QPair<QString, QString>("function1", "path");
    jobS.addFeatures(point1);// job is reponsible
    QPointer<FeatureWrapper> feature = jobS.getFeaturesByName(point1.name).at(0);
    // feature->getFeature()->addFunction(function); // controller ist responsible
    feature->getGeometry()->setMeasurementConfig(point1.measurementConfig); // controller ist responsible


    ProjectExchanger projectExchanger;
    projectExchanger.setMeasurementConfigManager(ModelManager::getMeasurementConfigManager());

    QDomDocument project = projectExchanger.saveProject(&jobS);

    DEBUG_PRETTY_PRINT_DOM(project);

    QString name = getNames(ModelManager::getMeasurementConfigManager()->getConfigs()).join(", ");
    qDebug() << name;
    // project, standard & user configs
    QVERIFY("FastPoint, FastPoint, PrecisePoint, StdPoint, StdTwoSide, level, measconfig-scandistance, measconfig-fastpoint_project" == name);

    // clean up
    foreach(const MeasurementConfig mc, ModelManager::getMeasurementConfigManager()->getConfigs()) {
        if( mc.isProjectConfig()
            && !mc.isStandardConfig()) {
            ModelManager::getMeasurementConfigManager()->removeProjectConfig(mc.getName());
        }
    }

    name = getNames(ModelManager::getMeasurementConfigManager()->getConfigs()).join(", ");
    qDebug() << name;
    // standard & user configs
    QVERIFY("FastPoint, FastPoint, PrecisePoint, StdPoint, StdTwoSide, level, measconfig-scandistance" == name);

    QPointer<OiJob> jobL = projectExchanger.loadProject(project);


    name = getNames(ModelManager::getMeasurementConfigManager()->getConfigs()).join(", ");
    qDebug() << name;
    // project, standard & user configs
    QVERIFY("FastPoint, FastPoint, PrecisePoint, StdPoint, StdTwoSide, level, measconfig-scandistance, measconfig-fastpoint_project" == name);

}


QTEST_MAIN(DialogsTest)

#include "dialogstest.moc"
