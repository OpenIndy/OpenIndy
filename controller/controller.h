#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QPointer>
#include <QString>

#include "oijob.h"
#include "modelmanager.h"
#include "featuretablecolumnconfig.h"
#include "trafoparamtablecolumnconfig.h"
#include "parameterdisplayconfig.h"

/*!
 * \brief The Controller class
 */
class Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller(QObject *parent = 0);

public slots:

    //###################
    //set display configs
    //###################

    void setFeatureTableColumnConfig(const FeatureTableColumnConfig &config);
    void setTrafoParamColumnConfig(const TrafoParamTableColumnConfig &config);
    void setParameterDisplayConfig(const ParameterDisplayConfig &config);

signals:

    //#################################################
    //signals to inform about current job state changes
    //#################################################

    //active feature changes
    void activeFeatureChanged();
    void activeStationChanged();
    void activeCoordinateSystemChanged();

    //feature(s) added or removed
    void featureSetChanged();
    void coordSystemSetChanged();
    void stationSetChanged();
    void trafoParamSetChanged();
    void geometrySetChanged();

    //group(s) added or removed
    void availableGroupsChanged();
    void activeGroupChanged();

    //general feature attributes changed
    void featureAttributesChanged();

    //feature specific attributes changed
    void featureNameChanged(const int &featureId, const QString &oldName);
    void featureGroupChanged(const int &featureId, const QString &oldGroup);
    void featureCommentChanged(const int &featureId);
    void featureIsUpdatedChanged(const int &featureId);
    void featureIsSolvedChanged(const int &featureId);
    void featureFunctionsChanged(const int &featureId);
    void featureUsedForChanged(const int &featureId);
    void featurePreviouslyNeededChanged(const int &featureId);

    //geometry specific attributes changed
    void geometryIsCommonChanged(const int &featureId);
    void geometryNominalsChanged(const int &featureId);
    void geometryActualChanged(const int &featureId);
    void geometryObservationsChanged(const int &featureId);
    void geometryNominalSystemChanged(const int &featureId);
    void geometryStatisticChanged(const int &featureId);
    void geometrySimulationDataChanged(const int &featureId);
    void geometryMeasurementConfigChanged(const int &featureId);

    //coordinate system specific attributes changed
    void systemObservationsChanged(const int &featureId, const int &obsId);
    void systemTrafoParamsChanged(const int &featureId);
    void systemsNominalsChanged(const int &featureId);

    //station specific attributes changed
    void stationSensorChanged(const int &featureId);

    //transformation parameter specific attributes changed
    void trafoParamParametersChanged(const int &featureId);
    void trafoParamSystemsChanged(const int &featureId);
    void trafoParamIsUsedChanged(const int &featureId);
    void trafoParamValidTimeChanged(const int &featureId);
    void trafoParamIsMovementChanged(const int &featureId);

private:

    //##############
    //helper methods
    //##############

    void createDefaultJob();
    void setJob(const QPointer<OiJob> &job);

    void initDisplayConfigs();

private:

    //##################
    //general attributes
    //##################

    QPointer<OiJob> job;

};

#endif // CONTROLLER_H
