#include <QString>
#include <QtTest>
#include <QPointer>
#include <QList>
#include <QStringList>
#include <QStringListModel>
#include "createfeaturedialog.h"
#include "availablefunctionslistproxymodel.h"

// recomended readings:
// https://gist.github.com/peteristhegreat/cbd8eaa0e565d0b82dbfb5c7fdc61c8d
// https://vicrucann.github.io/tutorials/qttest-signals-qtreewidget/

using namespace oi;

class DialogsTest : public QObject
{
    Q_OBJECT

public:
    DialogsTest();

private Q_SLOTS:
    void initTestCase();
    void createPoint();
    void createCircle();
    void createPlane();
    void createLevel();
    void reuseDialogInstance();

private:


};

DialogsTest::DialogsTest() {

}

void DialogsTest::initTestCase() {
    // create in memory database
    QFile sqlFile1(INIT_SQL);
    sqlFile1.open(QFile::ReadOnly | QFile::Text);
    QTextStream in1(&sqlFile1);
    QStringList statements = in1.readAll().split(";");

    QFile sqlFile2(ELEMENT_SQL);
    sqlFile2.open(QFile::ReadOnly | QFile::Text);
    QTextStream in2(&sqlFile2);
    statements.append(in2.readAll().split(";"));

    SystemDbManager::initInMemoryDB(statements);


    QStringList entityTypes;
    entityTypes << "point" << "circle" << "plane"; // comboBox_entityType order

    QPointer<MeasurementConfigManager> measurementConfigManager = new MeasurementConfigManager();

    MeasurementConfig fastPointConfig;
    fastPointConfig.setName("measconfig-fastpoint-cartesian");
    fastPointConfig.setTypeOfReading(ReadingTypes::eCartesianReading);
    measurementConfigManager->addProjectMeasurementConfig(fastPointConfig); // add as project config ... seams ok
    measurementConfigManager->addSavedMeasurementConfig(fastPointConfig);

    MeasurementConfig fastPointConfigP;
    fastPointConfigP.setName("measconfig-fastpoint-polar");
    fastPointConfigP.setTypeOfReading(ReadingTypes::eCartesianReading);
    measurementConfigManager->addProjectMeasurementConfig(fastPointConfigP); // add as project config ... seams ok
    measurementConfigManager->addSavedMeasurementConfig(fastPointConfigP);

    MeasurementConfig levelConfig;
    levelConfig.setName("measconfig-level");
    levelConfig.setTypeOfReading(ReadingTypes::eLevelReading);
    measurementConfigManager->addProjectMeasurementConfig(levelConfig); // add as project config ... seams ok
    measurementConfigManager->addSavedMeasurementConfig(levelConfig);

    MeasurementConfig scanTimeConfig;
    scanTimeConfig.setName("measconfig-scantime-cartesian");
    scanTimeConfig.setTypeOfReading(ReadingTypes::eCartesianReading);
    scanTimeConfig.setTimeDependent(true);
    measurementConfigManager->addProjectMeasurementConfig(scanTimeConfig); // add as project config ... seams ok
    measurementConfigManager->addSavedMeasurementConfig(scanTimeConfig);

    MeasurementConfig scanTimeConfigP;
    scanTimeConfigP.setName("measconfig-scantime-polar");
    scanTimeConfigP.setTypeOfReading(ReadingTypes::ePolarReading);
    scanTimeConfigP.setTimeDependent(true);
    measurementConfigManager->addProjectMeasurementConfig(scanTimeConfigP); // add as project config ... seams ok
    measurementConfigManager->addSavedMeasurementConfig(scanTimeConfigP);

    MeasurementConfig scanDistanceConfig;
    scanDistanceConfig.setName("measconfig-scandistance-cartesian"); // not for point
    scanDistanceConfig.setTypeOfReading(ReadingTypes::eCartesianReading);
    scanDistanceConfig.setDistanceDependent(true);
    measurementConfigManager->addProjectMeasurementConfig(scanDistanceConfig); // add as project config ... seams ok
    measurementConfigManager->addSavedMeasurementConfig(scanDistanceConfig);


    MeasurementConfig scanDistanceConfigP;
    scanDistanceConfigP.setName("measconfig-scandistance-polar"); // not for point
    scanDistanceConfigP.setTypeOfReading(ReadingTypes::ePolarReading);
    scanDistanceConfigP.setDistanceDependent(true);
    measurementConfigManager->addProjectMeasurementConfig(scanDistanceConfigP); // add as project config ... seams ok
    measurementConfigManager->addSavedMeasurementConfig(scanDistanceConfigP);

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
    plane.name = "function-fitplanet";
    plane.iid = OiMetaData::iid_FitFunction;
    plane.applicableFor << FeatureTypes::ePlaneFeature;
    plane.neededElements << ElementTypes::eObservationElement;

    sdb::Function level;
    level.name = "function-fitlevel";
    level.iid = OiMetaData::iid_FitFunction;
    level.applicableFor << FeatureTypes::ePlaneFeature;
    level.neededElements << ElementTypes::eReadingLevelElement;

    sdb::Function planefrompoints;
    planefrompoints.name = "function-planefrompoints";
    planefrompoints.iid = OiMetaData::iid_ConstructFunction;
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
    for(int i=0; i<mConfigLV->model()->rowCount();i++) {
        qDebug() <<  mConfigLV->model()->index(i, 0).data( Qt::DisplayRole ).toString();
    }
    QVERIFY(4 == mConfigLV->model()->rowCount());
    QVERIFY("*measconfig-fastpoint-cartesian"   == mConfigLV->model()->index(0, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("*measconfig-fastpoint-polar"       == mConfigLV->model()->index(1, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("*measconfig-scantime-cartesian"    == mConfigLV->model()->index(2, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("*measconfig-scantime-polar"        == mConfigLV->model()->index(3, 0).data( Qt::DisplayRole ).toString());

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
    for(int i=0; i<mConfigLV->model()->rowCount();i++) {
        qDebug() <<  mConfigLV->model()->index(i, 0).data( Qt::DisplayRole ).toString();
    }
    QVERIFY(6 == mConfigLV->model()->rowCount());
    QVERIFY("*measconfig-fastpoint-cartesian"       == mConfigLV->model()->index(0, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("*measconfig-fastpoint-polar"           == mConfigLV->model()->index(1, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("*measconfig-scantime-cartesian"        == mConfigLV->model()->index(2, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("*measconfig-scantime-polar"            == mConfigLV->model()->index(3, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("*measconfig-scandistance-cartesian"    == mConfigLV->model()->index(4, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("*measconfig-scandistance-polar"        == mConfigLV->model()->index(5, 0).data( Qt::DisplayRole ).toString());
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
    QVERIFY("function-fitplanet" == functionLV->model()->index(functionCB->currentIndex(),0).data( Qt::DisplayRole ).toString());

    // check applicable measurement configs
    QPointer<QComboBox> mConfigCB = dialog.findChild<QComboBox *>("comboBox_mConfig");
    QPointer<QListView> mConfigLV = mConfigCB->findChild<QListView *>();
    for(int i=0; i<mConfigLV->model()->rowCount();i++) {
        qDebug() <<  mConfigLV->model()->index(i, 0).data( Qt::DisplayRole ).toString();
    }
    QVERIFY(6 == mConfigLV->model()->rowCount());
    QVERIFY("*measconfig-fastpoint-cartesian"       == mConfigLV->model()->index(0, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("*measconfig-fastpoint-polar"           == mConfigLV->model()->index(1, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("*measconfig-scantime-cartesian"        == mConfigLV->model()->index(2, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("*measconfig-scantime-polar"            == mConfigLV->model()->index(3, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("*measconfig-scandistance-cartesian"    == mConfigLV->model()->index(4, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("*measconfig-scandistance-polar"        == mConfigLV->model()->index(5, 0).data( Qt::DisplayRole ).toString());

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
    QVERIFY("function-fitplanet" == functionLV->model()->index(functionCB->currentIndex(),0).data( Qt::DisplayRole ).toString());


    int i=0;
    for(i=0; i<functionLV->model()->rowCount(); i++) {
        if("function-fitlevel" == functionLV->model()->index(i,0).data( Qt::DisplayRole ).toString()) {
            break;
        }
    }

    // select "function-fitlevel"
    QTest::mouseClick(functionCB, Qt::LeftButton);
    QTest::qWait(1000); // TODO spy

    QModelIndex idx = functionLV->model()->index(i,0);
    functionLV->scrollTo(idx);


    QPoint itemPt = functionLV->visualRect(idx).center();
    QString functionName = functionLV->model()->index(i,0).data( Qt::DisplayRole ).toString();
    qDebug() << "clicking on function" << functionName;
    QVERIFY("function-fitlevel" == functionName);

    QTest::mouseClick(functionLV->viewport(), Qt::LeftButton, 0, itemPt);
    QTest::qWait(1000);

    // check applicable measurement configs
    QPointer<QComboBox> mConfigCB = dialog.findChild<QComboBox *>("comboBox_mConfig");
    QPointer<QListView> mConfigLV = mConfigCB->findChild<QListView *>();
    for(int i=0; i<mConfigLV->model()->rowCount();i++) {
        qDebug() <<  mConfigLV->model()->index(i, 0).data( Qt::DisplayRole ).toString();
    }
    QVERIFY(1 == mConfigLV->model()->rowCount());
    QVERIFY("*measconfig-level" == mConfigLV->model()->index(0, 0).data( Qt::DisplayRole ).toString());

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
    QVERIFY("function-fitplanet" == functionLV->model()->index(functionCB->currentIndex(),0).data( Qt::DisplayRole ).toString());

    // check applicable measurement configs
    QPointer<QComboBox> mConfigCB = dialog.findChild<QComboBox *>("comboBox_mConfig");
    QPointer<QListView> mConfigLV = mConfigCB->findChild<QListView *>();
    for(int i=0; i<mConfigLV->model()->rowCount();i++) {
        qDebug() <<  mConfigLV->model()->index(i, 0).data( Qt::DisplayRole ).toString();
    }
    QVERIFY(6 == mConfigLV->model()->rowCount());
    QVERIFY("*measconfig-fastpoint-cartesian"       == mConfigLV->model()->index(0, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("*measconfig-fastpoint-polar"           == mConfigLV->model()->index(1, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("*measconfig-scantime-cartesian"        == mConfigLV->model()->index(2, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("*measconfig-scantime-polar"            == mConfigLV->model()->index(3, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("*measconfig-scandistance-cartesian"    == mConfigLV->model()->index(4, 0).data( Qt::DisplayRole ).toString());
    QVERIFY("*measconfig-scandistance-polar"        == mConfigLV->model()->index(5, 0).data( Qt::DisplayRole ).toString());

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
    QVERIFY("function-fitplanet" == functionLV->model()->index(functionCB->currentIndex(),0).data( Qt::DisplayRole ).toString());


    int i=0;
    for(i=0; i<functionLV->model()->rowCount(); i++) {
        if("function-fitlevel" == functionLV->model()->index(i,0).data( Qt::DisplayRole ).toString()) {
            break;
        }
    }

    // select "function-fitlevel"
    QTest::mouseClick(functionCB, Qt::LeftButton);
    QTest::qWait(1000); // TODO spy

    QModelIndex idx = functionLV->model()->index(i,0);
    functionLV->scrollTo(idx);


    QPoint itemPt = functionLV->visualRect(idx).center();
    QString functionName = functionLV->model()->index(i,0).data( Qt::DisplayRole ).toString();
    qDebug() << "clicking on function" << functionName;
    QVERIFY("function-fitlevel" == functionName);

    QTest::mouseClick(functionLV->viewport(), Qt::LeftButton, 0, itemPt);
    QTest::qWait(1000);

    // check applicable measurement configs
    mConfigCB = dialog.findChild<QComboBox *>("comboBox_mConfig");
    mConfigLV = mConfigCB->findChild<QListView *>();
    for(int i=0; i<mConfigLV->model()->rowCount();i++) {
        qDebug() <<  mConfigLV->model()->index(i, 0).data( Qt::DisplayRole ).toString();
    }
    QVERIFY(1 == mConfigLV->model()->rowCount());
    QVERIFY("*measconfig-level" == mConfigLV->model()->index(0, 0).data( Qt::DisplayRole ).toString());


}

QTEST_MAIN(DialogsTest)

#include "dialogstest.moc"
