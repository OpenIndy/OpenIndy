#include <QString>
#include <QtTest>
#include <QPointer>
#include <QList>
#include "createfeaturedialog.h"

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

void DialogsTest::initial()
{

    CreateFeatureDialog dialog;

    qDebug() << "initial";
}

QTEST_APPLESS_MAIN(DialogsTest)

#include "dialogstest.moc"
