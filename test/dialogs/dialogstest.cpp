#include <QString>
#include <QtTest>
#include <QPointer>
#include <QList>
#include <QStringList>
#include <QStringListModel>
#include "createfeaturedialog.h"
#include "availablefunctionslistproxymodel.h"

using namespace oi;

class DialogsTest : public QObject
{
    Q_OBJECT

public:
    DialogsTest();

private Q_SLOTS:
    void initial();

private:


};

DialogsTest::DialogsTest() {

}

// https://gist.github.com/peteristhegreat/cbd8eaa0e565d0b82dbfb5c7fdc61c8d
// https://vicrucann.github.io/tutorials/qttest-signals-qtreewidget/
void DialogsTest::initial()
{


    QStringList entityTypes;
    entityTypes << "point" << "circle" << "plane" << "level" << "cylinder";

    QPointer<MeasurementConfigManager> measurementConfigManager = new MeasurementConfigManager();

    // create plugin with some functions
    QList<sdb::Plugin> plugins;
    sdb::Plugin testPlugin;
    testPlugin.name = "testplugin";
    testPlugin.author = "esc";

    QString iid = OiMetaData::iid_FitFunction;
    sdb::Function point;
    point.name = "fitpoint";
    point.iid = iid;
    point.applicableFor << FeatureTypes::ePointFeature;

    sdb::Function plane;
    plane.name = "fitplane";
    point.iid = iid;
    plane.applicableFor << FeatureTypes::ePlaneFeature;

    sdb::Function level;
    level.name = "fitlevel";
    point.iid = iid;
    level.applicableFor << FeatureTypes::ePlaneFeature;

    sdb::Function circle;
    circle.name = "fitcircle";
    point.iid = iid;
    circle.applicableFor << FeatureTypes::eCircleFeature;

    testPlugin.functions << plane << point << level << circle;

    plugins << testPlugin;

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

    // add plugin to database
    SystemDbManager::addPlugin(testPlugin);

    // init ModelManager
    ModelManager::testInit(entityTypes, measurementConfigManager, plugins);

    // create dialog
    CreateFeatureDialog dialog;

    // check function

    QPointer<QComboBox> functionCB = dialog.findChild<QComboBox *>("comboBox_function");
    QPointer<QListView> functionLV = functionCB->findChild<QListView *>();
    qDebug() << functionLV->model()->rowCount();
    qDebug() << functionLV->model()->index(0,0).data( Qt::DisplayRole ).toString();

    // select circle
    QPointer<QComboBox> entityTypeCB = dialog.findChild<QComboBox *>("comboBox_entityType");
    qDebug() << entityTypeCB;
    qDebug() << entityTypeCB->currentIndex();
    QVERIFY(0 == entityTypeCB->currentIndex());
    QTest::mouseClick(entityTypeCB, Qt::LeftButton);
    QTest::qWait(1000);

    QPointer<QListView> entityTypeLV = entityTypeCB->findChild<QListView *>();
    qDebug() << entityTypeLV;

    int r = 1;
    QModelIndex idx = entityTypeLV->model()->index(r,0);
    entityTypeLV->scrollTo(idx);

    QPoint itemPt = entityTypeLV->visualRect(idx).center();
    QString str = entityTypeLV->model()->index(r,0).data( Qt::DisplayRole ).toString();
    qDebug() << "clicking on" << str ;

    QTest::mouseClick(entityTypeLV->viewport(), Qt::LeftButton, 0, itemPt);
    QTest::qWait(1000);
    // Reopen the combobox
    QTest::mouseClick(entityTypeCB, Qt::LeftButton);
    QTest::qWait(1000);

    qDebug() << entityTypeCB->currentIndex();
    QVERIFY(1 == entityTypeCB->currentIndex());
}

QTEST_MAIN(DialogsTest)

#include "dialogstest.moc"
