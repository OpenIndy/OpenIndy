#include <QString>
#include <QtTest>
#include <QPointer>
#include <QList>

#include <algorithm>

#include "feature.h"
#include "featurewrapper.h"
#include "featuresorter.h"

using namespace oi;
using namespace oi::math;


class FeatureSorterTest : public QObject
{
    Q_OBJECT

public:
    FeatureSorterTest();

private Q_SLOTS:
    void testSimpleSortByName();
    void testSimpleSortById();


    QPointer<FeatureWrapper> createPoint(int id, QString name, bool isNominal);
    QString asString(QList<QPointer<FeatureWrapper>> features);
};

QPointer<FeatureWrapper> FeatureSorterTest::createPoint(int id, QString name, bool isNominal) {
    QPointer<FeatureWrapper> featurewrapper = new FeatureWrapper();

    QPointer<Point> point = new Point(id, isNominal, Position());
    point->setFeatureName(name);
    featurewrapper->setPoint(point);

    return featurewrapper;
}
QString FeatureSorterTest::asString(QList<QPointer<FeatureWrapper>> features) {
    QString s;
    for( int i=0; i<features.count(); ++i ) {
        s.append(QVariant(features[i]->getFeature()->getId()).toString());
        s.append(", ");
        s.append(features[i]->getFeature()->getFeatureName());
        s.append(", ");
        s.append(features[i]->getFeature()->getFeatureWrapper()->getGeometry()->getIsNominal() ? "nominal" : "actual");
        s.append("; ");
    }
    return s;
}

FeatureSorterTest::FeatureSorterTest()
{

}

// basic test
void FeatureSorterTest::testSimpleSortByName()
{
    QList<QPointer<FeatureWrapper>> features;
    features.append(createPoint(1, "B", false));
    features.append(createPoint(2, "B", true));
    features.append(createPoint(3, "A", false));

    qDebug() << asString(features);

    struct {
        bool operator()(QPointer<FeatureWrapper> a, QPointer<FeatureWrapper> b) const {
            return a->getFeature()->getFeatureName() < b->getFeature()->getFeatureName();
        }
    } customLessThan;

    std::sort(features.begin(), features.end(), customLessThan);

    QString sorted = asString(features);
    qDebug() << sorted;

    QCOMPARE(sorted, QString("3, A, actual; 1, B, actual; 2, B, nominal; "));
}

// basic test
void FeatureSorterTest::testSimpleSortById()
{
    QList<QPointer<FeatureWrapper>> features;
    features.append(createPoint(4, "B", false));
    features.append(createPoint(5, "B", true));
    features.append(createPoint(3, "A", false));

    qDebug() << asString(features);

    struct {
        bool operator()(QPointer<FeatureWrapper> a, QPointer<FeatureWrapper> b) const {
            return a->getFeature()->getId() < b->getFeature()->getId();
        }
    } customLessThan;

    std::sort(features.begin(), features.end(), customLessThan);

    QString sorted = asString(features);
    qDebug() << sorted;

    QCOMPARE(sorted, QString("3, A, actual; 4, B, actual; 5, B, nominal; "));
}


QTEST_APPLESS_MAIN(FeatureSorterTest)

#include "featuresortertest.moc"
