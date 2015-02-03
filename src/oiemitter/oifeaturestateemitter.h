#ifndef OIFEATURESTATEEMITTER_H
#define OIFEATURESTATEEMITTER_H

#include <QObject>

/*!
 * \brief The OiFeatureStateEmitter class
 * Emitter class that is used by all OpenIndy elements to inform about their state changes
 */
class OiFeatureStateEmitter : public QObject
{
    Q_OBJECT

private:
    explicit OiFeatureStateEmitter(QObject *parent = 0);

public:
    static const OiFeatureStateEmitter *getInstance();

signals:

    //##########################################################################
    //signals that OiFeatureState listens to to know about element state changes
    //##########################################################################

    //element specific signals
    void elementIdChanged();

    //feature specific signals
    void featureNameChanged(const int &featureId, const QString &oldName);
    void featureGroupChanged(const int &featureId, const QString &oldGroup);
    void featureCommentChanged(const int &featureId);
    void featureIsUpdatedChanged(const int &featureId);
    void featureIsSolvedChanged(const int &featureId);
    void featureFunctionListChanged(const int &featureId);
    void featureUsedForChanged(const int &featureId);
    void featurePreviouslyNeededChanged(const int &featureId);
    void featureDisplayColorChanged(const int &featureId);
    void featureIsDrawnChanged(const int &featureId);
    void featureIsActiveChanged(const int &featureId);
    void featureAboutToBeDeleted(const int &featureId);

    //geometry specific signals
    void geomIsCommonChanged(const int &featureId);
    void geomMyNominalsChanged(const int &featureId);
    void geomMyActualChanged(const int &featureId);
    void geomMyObservationsChanged(const int &featureId);
    void geomMyNominalSystemChanged(const int &featureId);
    void geomMyStatisticChanged(const int &featureId);
    void geomMyMeasurementConfigChanged(const int &featureId);
    void geomUsedReadingTypesChanged(const int &featureId);

    //station specific signals
    void stationIsActiveChanged(const int &featureId);

    //trafo param specific signals
    void trafoParamTransformationChanged(const int &featureId);
    void trafoParamCoordSysChanged(const int &featureId);
    void trafoParamIsUsedChanged(const int &featureId);
    void trafoParamValidTimeChanged(const int &featureId);
    void trafoParamIsMovementChanged(const int &featureId);

    //coordinate system specific signals
    void coordSysObservationsChanged(const int &featureId, const int &obsId);
    void coordSysTrafoParamsChanged(const int &featureId);
    void coordSysNominalsChanged(const int &featureId);
    void coordSysIsActiveChanged(const int &featureId);

private:
    OiFeatureStateEmitter *myStateEmitter;

    //######################################################################
    //methods called by the different elements to inform about state changes
    //######################################################################

    //element specific methods
    void emitElementIdChanged();

    //feature specific methods
    void emitFeatureNameChanged(const int &featureId, const QString &oldName);
    void emitFeatureGroupChanged(const int &featureId, const QString &oldGroup);
    void emitFeatureCommentChanged(const int &featureId);
    void emitFeatureIsUpdatedChanged(const int &featureId);
    void emitFeatureIsSolvedChanged(const int &featureId);
    void emitFeatureFunctionListChanged(const int &featureId);
    void emitFeatureUsedForChanged(const int &featureId);
    void emitFeaturePreviouslyNeededChanged(const int &featureId);
    void emitFeatureDisplayColorChanged(const int &featureId);
    void emitFeatureIsDrawnChanged(const int &featureId);
    void emitFeatureIsActiveChanged(const int &featureId);
    void emitFeatureAboutToBeDeleted(const int &featureId);

    //geometry specific methods
    void emitGeomIsCommonChanged(const int &featureId);
    void emitGeomMyNominalsChanged(const int &featureId);
    void emitGeomMyActualChanged(const int &featureId);
    void emitGeomMyObservationsChanged(const int &featureId);
    void emitGeomMyNominalSystemChanged(const int &featureId);
    void emitGeomMyStatisticChanged(const int &featureId);
    void emitGeomMyMeasurementConfigChanged(const int &featureId);
    void emitGeomUsedReadingTypesChanged(const int &featureId);

    //station specific methods
    void emitStationIsActiveChanged(const int &featureId);

    //trafo param specific signals
    void emitTrafoParamTransformationChanged(const int &featureId);
    void emitTrafoParamCoordSysChanged(const int &featureId);
    void emitTrafoParamIsUsedChanged(const int &featureId);
    void emitTrafoParamValidTimeChanged(const int &featureId);
    void emitTrafoParamIsMovementChanged(const int &featureId);

    //coordinate system specific signals
    void emitCoordSysObservationsChanged(const int &featureId, const int &obsId);
    void emitCoordSysTrafoParamsChanged(const int &featureId);
    void emitCoordSysNominalsChanged(const int &featureId);
    void emitCoordSysIsActiveChanged(const int &featureId);

};

#endif // OIFEATURESTATEEMITTER_H
