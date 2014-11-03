#ifndef OISTAKEOUT_H
#define OISTAKEOUT_H

#include <QObject>
#include <QList>
#include <limits>

#include "oifeaturestate.h"
#include "featurewrapper.h"
#include "configuration.h"

using namespace std;

class OiStakeOut : public QObject
{
    Q_OBJECT

public:
    enum StakeOutMode{
        eSequence,
        eNearest
    };

private:
    explicit OiStakeOut(QObject *parent = 0);

public:
    static int startStakeOut(StakeOutMode mode, bool allGeoms, QStringList groups = QStringList(), QList<int> geometries = QList<int>());
    static bool stopStakeOut(int id);
    static FeatureWrapper *getNextGeometry(int id);

private:
    static OiStakeOut *getStakeOut(int id);

    FeatureWrapper *getSequenceGeometry();
    FeatureWrapper *getNearestGeometry();

    static QList<OiStakeOut *> currentStakeOuts;

    int id; //stake out id to be able to identify the correct one
    QMap<int, bool> geometries; //all geometries that shall be staked out
    int lastGeometryId; //the last selected geometry during stake out
    StakeOutMode mode; //the stake out mode

};

#endif // OISTAKEOUT_H
