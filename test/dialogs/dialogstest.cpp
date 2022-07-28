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

    // create plugin with some functions
    QList<sdb::Plugin> plugins;
    sdb::Plugin testPlugin;
    testPlugin.name = "testplugin";
    testPlugin.author = "esc";

    sdb::Function point;
    point.name = "fitpoint";
    point.iid = OiMetaData::iid_FitFunction;
    point.applicableFor << FeatureTypes::ePointFeature;

    sdb::Function plane;
    plane.name = "fitplane";
    plane.iid = OiMetaData::iid_FitFunction;
    plane.applicableFor << FeatureTypes::ePlaneFeature;

    sdb::Function level;
    level.name = "fitlevel";
    level.iid = OiMetaData::iid_FitFunction;
    level.applicableFor << FeatureTypes::ePlaneFeature;

    sdb::Function planefrompoints;
    planefrompoints.name = "planefrompoints";
    planefrompoints.iid = OiMetaData::iid_ConstructFunction;
    planefrompoints.applicableFor << FeatureTypes::ePlaneFeature;

    sdb::Function circle;
    circle.name = "fitcircle";
    circle.iid = OiMetaData::iid_FitFunction;
    circle.applicableFor << FeatureTypes::eCircleFeature;

    testPlugin.functions << plane << point << level << circle << planefrompoints;

    plugins << testPlugin;

    // add plugin to database
    SystemDbManager::addPlugin(testPlugin);


    // init ModelManager
    ModelManager::testInit(entityTypes, measurementConfigManager, plugins);

}

void DialogsTest::createPoint()
{

    // create dialog
    CreateFeatureDialog dialog;

    // comboBox_entityType currently not usesd, type is set directly
    dialog.setFeatureType(FeatureTypes::ePointFeature);
    dialog.show(); // to call: void showEvent(QShowEvent *event);
    QTest::qWait(200); // ensure that all signals are processed

    // check function
    QPointer<QComboBox> functionCB;
    QPointer<QListView> functionLV;

    // check for default function
    functionCB = dialog.findChild<QComboBox *>("comboBox_function");
    functionLV = functionCB->findChild<QListView *>();
    qDebug() << functionLV->model()->rowCount();
    qDebug() << functionLV->model()->index(0,0).data( Qt::DisplayRole ).toString();
    QVERIFY("fitpoint" == functionLV->model()->index(0,0).data( Qt::DisplayRole ).toString());

}

void DialogsTest::createCircle() {
    // create dialog
    CreateFeatureDialog dialog;

    // comboBox_entityType currently not usesd, type is set directly
    dialog.setFeatureType(FeatureTypes::eCircleFeature);
    dialog.show(); // to call: void showEvent(QShowEvent *event);
    QTest::qWait(200); // ensure that all signals are processed

    // check function
    QPointer<QComboBox> functionCB;
    QPointer<QListView> functionLV;

    // check for default function
    functionCB = dialog.findChild<QComboBox *>("comboBox_function");
    functionLV = functionCB->findChild<QListView *>();
    qDebug() << functionLV->model()->rowCount();
    qDebug() << functionLV->model()->index(0,0).data( Qt::DisplayRole ).toString();
    QVERIFY("fitcircle" == functionLV->model()->index(0,0).data( Qt::DisplayRole ).toString());
}

void DialogsTest::createPlane() {
    // create dialog
    CreateFeatureDialog dialog;

    // comboBox_entityType currently not usesd, type is set directly
    dialog.setFeatureType(FeatureTypes::ePlaneFeature);
    dialog.show(); // to call: void showEvent(QShowEvent *event);
    QTest::qWait(200); // ensure that all signals are processed

    // check function
    QPointer<QComboBox> functionCB;
    QPointer<QListView> functionLV;

    // check for default function
    functionCB = dialog.findChild<QComboBox *>("comboBox_function");
    functionLV = functionCB->findChild<QListView *>();
    qDebug() << functionLV->model()->rowCount();
    qDebug() << functionLV->model()->index(0,0).data( Qt::DisplayRole ).toString();
    QVERIFY("fitplane" == functionLV->model()->index(0,0).data( Qt::DisplayRole ).toString());
}

void DialogsTest::createLevel() {
}


QTEST_MAIN(DialogsTest)

#include "dialogstest.moc"
