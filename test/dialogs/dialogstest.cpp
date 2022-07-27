#include <QString>
#include <QtTest>
#include <QPointer>
#include <QList>
#include <QStringList>
#include <QStringListModel>
#include "createfeaturedialog.h"
#include "availablefunctionslistproxymodel.h"
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

    QList<sdb::Plugin> plugins;

    ModelManager::testInit(entityTypes, measurementConfigManager, plugins);

    CreateFeatureDialog dialog;

    QPointer<QComboBox> entityTypeCB = dialog.findChild<QComboBox *>("comboBox_entityType");
    qDebug() << entityTypeCB;
    qDebug() << entityTypeCB->currentIndex();
    QVERIFY(0 == entityTypeCB->currentIndex());
    QTest::mouseClick(entityTypeCB, Qt::LeftButton);
    QTest::qWait(500);

    QPointer<QListView> entityTypeLV = entityTypeCB->findChild<QListView *>();
    qDebug() << entityTypeLV;

    int r = 1;
    QModelIndex idx = entityTypeLV->model()->index(r,0);
    entityTypeLV->scrollTo(idx);

    QPoint itemPt = entityTypeLV->visualRect(idx).center();
    QString str = entityTypeLV->model()->index(r,0).data( Qt::DisplayRole ).toString();
    qDebug() << "clicking on" << str ;

    QTest::mouseClick(entityTypeLV->viewport(), Qt::LeftButton, 0, itemPt);
    QTest::qWait(500);
    // Reopen the combobox
    QTest::mouseClick(entityTypeCB, Qt::LeftButton);
    QTest::qWait(500);

    qDebug() << entityTypeCB->currentIndex();
    QVERIFY(1 == entityTypeCB->currentIndex());
}

QTEST_MAIN(DialogsTest)

#include "dialogstest.moc"
