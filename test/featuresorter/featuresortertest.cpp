#include <QString>
#include <QtTest>
#include <QPointer>
#include <QList>

#include <algorithm>
#include <random>

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
    void testFeatureSorter_eSortStandard();
    void testFeatureSorter_eSortBy_Name_ActNom();
    void testFeatureSorter_eSortBy_Group_Name_ActNom();

    void testSimpleSortByName();
    void testSimpleSortById();

private:
    QPointer<FeatureWrapper> createPoint(int id, QString name, bool isNominal, QString groupname = "");
    QString asString(QList<QPointer<FeatureWrapper>> features, bool withGroup = false);
    QList<QPointer<FeatureWrapper>> createPoints(QString data);
    QList<QPointer<FeatureWrapper>> createTestData(bool shuffle = false);
};

QPointer<FeatureWrapper> FeatureSorterTest::createPoint(int id, QString name, bool isNominal, QString groupname) {
    QPointer<FeatureWrapper> featurewrapper = new FeatureWrapper();

    QPointer<Point> point = new Point(id, isNominal, Position());
    point->setFeatureName(name);
    point->setGroupName(groupname);
    featurewrapper->setPoint(point);

    return featurewrapper;
}
QString FeatureSorterTest::asString(QList<QPointer<FeatureWrapper>> features, bool withGroup) {
    QString s;
    for( int i=0; i<features.count(); ++i ) {
        s.append(QVariant(features[i]->getFeature()->getId()).toString());
        s.append(", ");
        if(withGroup) {
            s.append(features[i]->getFeature()->getGroupName());
            s.append(", ");
        }
        s.append(features[i]->getFeature()->getFeatureName());
        s.append(", ");
        s.append(features[i]->getFeature()->getFeatureWrapper()->getGeometry()->getIsNominal() ? "nominal" : "actual");
        s.append("; ");
    }
    return s;
}

QList<QPointer<FeatureWrapper>> FeatureSorterTest::createPoints(QString data) {
    QList<QPointer<FeatureWrapper>> features;

    QTextStream stream(data.toUtf8());
    while(!stream.atEnd()) {
        QString row = stream.readLine();
        if(row.startsWith("#")) {
            continue;
        }

        QStringList columns = row.split("\t");
        if(columns.size() == 0) {
            continue;
        }

        // id    actual/nominal  groupname   featurename
        features.append(createPoint(columns.at(0).toInt(), columns.at(3), columns.at(1).startsWith("nominal"), columns.at(2)));

    }
    return features;
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

/**
 * @brief FeatureSorterTest::createTestData
 * @return suffled test features
 */
QList<QPointer<FeatureWrapper>> FeatureSorterTest::createTestData(bool shuffle) {
    // colum delim: "\t"
    // line ending: "\n"
    QString data("\
# id    actual/nominal  groupname   featurename\n\
100	actual	01_Jigs_S15	God_Hole-Rear_Left_20\n\
101	nominal PART	01_Jigs_S15	God_Hole-Rear_Left_20\n\
102	actual	01_Jigs_S15	God_Hole-Rear_Left_20-a\n\
103	nominal PART	01_Jigs_S15	God_Hole-Rear_Left_20-a\n\
104	actual	01_Jigs_S15	God_Hole-Rear_Left_20-b\n\
105	nominal PART	01_Jigs_S15	God_Hole-Rear_Left_20-b\n\
106	actual	01_Jigs_S15	Pin_Y_21\n\
107	nominal PART	01_Jigs_S15	Pin_Y_21\n\
108	actual	01_Jigs_S15	Pin_Y_21-a\n\
109	nominal PART	01_Jigs_S15	Pin_Y_21-a\n\
110	actual	01_Jigs_S15	Pin_Y_21-b\n\
111	nominal PART	01_Jigs_S15	Pin_Y_21-b\n\
112	actual	03_Jigs_S18-19	Eggcups-Rear_Left_16\n\
113	nominal PART	03_Jigs_S18-19	Eggcups-Rear_Left_16\n\
114	actual	00_common	com01\n\
115	actual	00_common	com02\n\
116	actual	00_common	com03\n\
117	actual	00_common	com04\n\
118	actual	00_common	com05\n\
119	actual	00_common	com06\n\
120	actual	00_common	com07\n\
121	actual	00_common	com08\n\
122	actual	00_common	com09\n\
123	actual	00_common	com10\n\
124	actual	03_Jigs_S18-19	Eggcups-Rear_Left_16_a\n\
125	nominal PART	03_Jigs_S18-19	Eggcups-Rear_Left_16_a\n\
126	actual	03_Jigs_S18-19	Eggcups-Rear_Left_16_b\n\
127	nominal PART	03_Jigs_S18-19	Eggcups-Rear_Left_16_b\n\
128	actual	00_System	Level\n\
129	actual	03_Jigs_S18-19	just punkt\n\
130	actual	Weight-Test S17-Eggcup - Front right side	5\n\
131	actual	Weight-Test S17-Eggcup - Front right side	9\n\
132	actual	Weight-Test S17-Eggcup - Front right side	7\n\
133	actual	Weight-Test S17-Eggcup - Front right side	10\n\
134	actual		Y_Achse\n\
");

    QList<QPointer<FeatureWrapper>> features = createPoints(data);

    qDebug() << asString(features);

    if(shuffle) {
        std::random_device rd;
        std::mt19937 generator(rd());
        std::shuffle(features.begin(), features.end(), generator);
    }

    return features;
}

void FeatureSorterTest::testFeatureSorter_eSortStandard()
{
    QList<QPointer<FeatureWrapper>> features = createTestData();
    qDebug() << asString(features);

    FeatureSorter freatureSorter;
    freatureSorter.setSortingMode(FeatureSorter::SortingMode::eSortStandard);

    std::sort(features.begin(), features.end(), freatureSorter);

    QString sorted = asString(features);
    qDebug() << sorted;

    QCOMPARE(sorted, QString("100, God_Hole-Rear_Left_20, actual; 101, God_Hole-Rear_Left_20, nominal; 102, God_Hole-Rear_Left_20-a, actual; 103, God_Hole-Rear_Left_20-a, nominal; 104, God_Hole-Rear_Left_20-b, actual; 105, God_Hole-Rear_Left_20-b, nominal; 106, Pin_Y_21, actual; 107, Pin_Y_21, nominal; 108, Pin_Y_21-a, actual; 109, Pin_Y_21-a, nominal; 110, Pin_Y_21-b, actual; 111, Pin_Y_21-b, nominal; 112, Eggcups-Rear_Left_16, actual; 113, Eggcups-Rear_Left_16, nominal; 114, com01, actual; 115, com02, actual; 116, com03, actual; 117, com04, actual; 118, com05, actual; 119, com06, actual; 120, com07, actual; 121, com08, actual; 122, com09, actual; 123, com10, actual; 124, Eggcups-Rear_Left_16_a, actual; 125, Eggcups-Rear_Left_16_a, nominal; 126, Eggcups-Rear_Left_16_b, actual; 127, Eggcups-Rear_Left_16_b, nominal; 128, Level, actual; 129, just punkt, actual; 130, 5, actual; 131, 9, actual; 132, 7, actual; 133, 10, actual; 134, Y_Achse, actual; "));

    qDebug().noquote() << sorted.replace(";", "\n");
}

void FeatureSorterTest::testFeatureSorter_eSortBy_Name_ActNom()
{
    QList<QPointer<FeatureWrapper>> features = createTestData(true);
    qDebug() << asString(features);

    FeatureSorter freatureSorter;
    freatureSorter.setSortingMode(FeatureSorter::SortingMode::eSortBy_Name_ActNom);

    std::sort(features.begin(), features.end(), freatureSorter);

    QString sorted = asString(features);
    qDebug() << sorted;

    QCOMPARE(sorted, QString("133, 10, actual; 130, 5, actual; 132, 7, actual; 131, 9, actual; 114, com01, actual; 115, com02, actual; 116, com03, actual; 117, com04, actual; 118, com05, actual; 119, com06, actual; 120, com07, actual; 121, com08, actual; 122, com09, actual; 123, com10, actual; 112, Eggcups-Rear_Left_16, actual; 113, Eggcups-Rear_Left_16, nominal; 124, Eggcups-Rear_Left_16_a, actual; 125, Eggcups-Rear_Left_16_a, nominal; 126, Eggcups-Rear_Left_16_b, actual; 127, Eggcups-Rear_Left_16_b, nominal; 100, God_Hole-Rear_Left_20, actual; 101, God_Hole-Rear_Left_20, nominal; 102, God_Hole-Rear_Left_20-a, actual; 103, God_Hole-Rear_Left_20-a, nominal; 104, God_Hole-Rear_Left_20-b, actual; 105, God_Hole-Rear_Left_20-b, nominal; 129, just punkt, actual; 128, Level, actual; 106, Pin_Y_21, actual; 107, Pin_Y_21, nominal; 108, Pin_Y_21-a, actual; 109, Pin_Y_21-a, nominal; 110, Pin_Y_21-b, actual; 111, Pin_Y_21-b, nominal; 134, Y_Achse, actual; "));

    qDebug().noquote() << sorted.replace(";", "\n");
}

void FeatureSorterTest::testFeatureSorter_eSortBy_Group_Name_ActNom()
{
    QList<QPointer<FeatureWrapper>> features = createTestData(true);
    qDebug() << asString(features);

    FeatureSorter freatureSorter;
    freatureSorter.setSortingMode(FeatureSorter::SortingMode::eSortBy_Group_Name_ActNom);

    std::sort(features.begin(), features.end(), freatureSorter);

    QString sorted = asString(features, true);
    qDebug() << sorted;

    QCOMPARE(sorted, QString("134, , Y_Achse, actual; 114, 00_common, com01, actual; 115, 00_common, com02, actual; 116, 00_common, com03, actual; 117, 00_common, com04, actual; 118, 00_common, com05, actual; 119, 00_common, com06, actual; 120, 00_common, com07, actual; 121, 00_common, com08, actual; 122, 00_common, com09, actual; 123, 00_common, com10, actual; 128, 00_System, Level, actual; 100, 01_Jigs_S15, God_Hole-Rear_Left_20, actual; 101, 01_Jigs_S15, God_Hole-Rear_Left_20, nominal; 102, 01_Jigs_S15, God_Hole-Rear_Left_20-a, actual; 103, 01_Jigs_S15, God_Hole-Rear_Left_20-a, nominal; 104, 01_Jigs_S15, God_Hole-Rear_Left_20-b, actual; 105, 01_Jigs_S15, God_Hole-Rear_Left_20-b, nominal; 106, 01_Jigs_S15, Pin_Y_21, actual; 107, 01_Jigs_S15, Pin_Y_21, nominal; 108, 01_Jigs_S15, Pin_Y_21-a, actual; 109, 01_Jigs_S15, Pin_Y_21-a, nominal; 110, 01_Jigs_S15, Pin_Y_21-b, actual; 111, 01_Jigs_S15, Pin_Y_21-b, nominal; 112, 03_Jigs_S18-19, Eggcups-Rear_Left_16, actual; 113, 03_Jigs_S18-19, Eggcups-Rear_Left_16, nominal; 124, 03_Jigs_S18-19, Eggcups-Rear_Left_16_a, actual; 125, 03_Jigs_S18-19, Eggcups-Rear_Left_16_a, nominal; 126, 03_Jigs_S18-19, Eggcups-Rear_Left_16_b, actual; 127, 03_Jigs_S18-19, Eggcups-Rear_Left_16_b, nominal; 129, 03_Jigs_S18-19, just punkt, actual; 133, Weight-Test S17-Eggcup - Front right side, 10, actual; 130, Weight-Test S17-Eggcup - Front right side, 5, actual; 132, Weight-Test S17-Eggcup - Front right side, 7, actual; 131, Weight-Test S17-Eggcup - Front right side, 9, actual; "));

    qDebug().noquote() << sorted.replace(";", "\n");

}

QTEST_APPLESS_MAIN(FeatureSorterTest)

#include "featuresortertest.moc"
