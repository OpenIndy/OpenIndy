#ifndef FEATURE_H
#define FEATURE_H

#include <QObject>
#include <QPointer>
#include <QList>
#include <QString>
#include <QtXml>

#include "element.h"

class FeatureWrapper;
class Function;

/*!
 * \brief The Feature class
 */
class Feature : public Element
{
    friend class OiJob;
    friend class Feature;
    Q_OBJECT

public:
    explicit Feature(QObject *parent = 0);

    Feature(const Feature &copy, QObject *parent = 0);

    Feature &operator=(const Feature &copy);

    virtual ~Feature();

    //############################################
    //get the feature wrapped by a feature wrapper
    //############################################

    const QPointer<FeatureWrapper> &getFeatureWrapper() const;

    //#############################
    //get or set feature attributes
    //#############################

    const QString &getFeatureName() const;
    void setFeatureName(const QString &name);

    const QString &getGroupName() const;
    void setGroupName(const QString &group);

    const QString &getComment() const;
    void setComment(const QString &comment);

    const bool &getIsUpdated() const;
    void setIsUpdated(const bool &isUpdated);

    const bool &getIsSolved() const;
    void setIsSolved(const bool &isSolved);

    const QList<QPointer<Function> > &getFunctions() const;
    void addFunction(const QPointer<Function> &function);
    void removeFunction(const int &index);

    const bool &getIsActiveFeature() const;
    void setActiveFeatureState(const bool &isActiveFeature);

    const QList<QPointer<FeatureWrapper> > &getUsedFor() const;
    bool addUsedFor(const QPointer<FeatureWrapper> &feature);
    bool removeUsedFor(const QPointer<FeatureWrapper> &feature);

    const QList<QPointer<FeatureWrapper> > &getPreviouslyNeeded() const;
    bool addPreviouslyNeeded(const QPointer<FeatureWrapper> &feature);
    bool removePreviouslyNeeded(const QPointer<FeatureWrapper> &feature);

    //###########################
    //reexecute the function list
    //###########################

    virtual void recalc();

    //#################
    //save and load XML
    //#################

    virtual QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    virtual bool fromOpenIndyXML(QDomElement &xmlElem);

    //###############
    //display methods
    //###############

    //general attributes
    virtual QString getDisplayType() const;
    virtual QString getDisplayIsSolved() const;
    virtual QString getDisplayIsUpdated() const;
    virtual QString getDisplayFunctions() const;
    virtual QString getDisplayUsedFor() const;
    virtual QString getDisplayPreviouslyNeeded() const;
    virtual QString getDisplayStDev(const UnitType &type, const int &digits) const;

    //geometry specific attributes
    virtual QString getDisplayMeasurementConfig() const;
    virtual QString getDisplayObservations() const;

    //unknown geometry parameters
    virtual QString getDisplayX(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    virtual QString getDisplayY(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    virtual QString getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    virtual QString getDisplayPrimaryI(const int &digits, const bool &showDiff = false) const;
    virtual QString getDisplayPrimaryJ(const int &digits, const bool &showDiff = false) const;
    virtual QString getDisplayPrimaryK(const int &digits, const bool &showDiff = false) const;
    virtual QString getDisplayRadiusA(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    virtual QString getDisplayRadiusB(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    virtual QString getDisplaySecondaryI(const int &digits, const bool &showDiff = false) const;
    virtual QString getDisplaySecondaryJ(const int &digits, const bool &showDiff = false) const;
    virtual QString getDisplaySecondaryK(const int &digits, const bool &showDiff = false) const;
    virtual QString getDisplayAperture(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    virtual QString getDisplayA(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    virtual QString getDisplayB(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    virtual QString getDisplayC(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    virtual QString getDisplayAngle(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    virtual QString getDisplayDistance(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    virtual QString getDisplayMeasurementSeries(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    virtual QString getDisplayTemperature(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    virtual QString getDisplayLength(const UnitType &type, const int &digits, const bool &showDiff = false) const;

    //coordinate system specific
    virtual QString getDisplayExpansionOriginX(const UnitType &type, const int &digits) const;
    virtual QString getDisplayExpansionOriginY(const UnitType &type, const int &digits) const;
    virtual QString getDisplayExpansionOriginZ(const UnitType &type, const int &digits) const;
    virtual QString getDisplayXAxisI(const int &digits) const;
    virtual QString getDisplayXAxisJ(const int &digits) const;
    virtual QString getDisplayXAxisK(const int &digits) const;
    virtual QString getDisplayYAxisI(const int &digits) const;
    virtual QString getDisplayYAxisJ(const int &digits) const;
    virtual QString getDisplayYAxisK(const int &digits) const;
    virtual QString getDisplayZAxisI(const int &digits) const;
    virtual QString getDisplayZAxisJ(const int &digits) const;
    virtual QString getDisplayZAxisK(const int &digits) const;

    //trafo param specific
    virtual QString getDisplayStartSystem() const;
    virtual QString getDisplayDestinationSystem() const;
    virtual QString getDisplayTranslationX(const UnitType &type, const int &digits) const;
    virtual QString getDisplayTranslationY(const UnitType &type, const int &digits) const;
    virtual QString getDisplayTranslationZ(const UnitType &type, const int &digits) const;
    virtual QString getDisplayRotationX(const UnitType &type, const int &digits) const;
    virtual QString getDisplayRotationY(const UnitType &type, const int &digits) const;
    virtual QString getDisplayRotationZ(const UnitType &type, const int &digits) const;
    virtual QString getDisplayScaleX(const int &digits) const;
    virtual QString getDisplayScaleY(const int &digits) const;
    virtual QString getDisplayScaleZ(const int &digits) const;
    virtual QString getDisplayIsUsed() const;
    virtual QString getDisplayValidTime() const;
    virtual QString getDisplayIsMovement() const;

signals:

    //#######################################
    //signals to inform about feature changes
    //#######################################

    void featureNameChanged(const int &featureId, const QString &oldName);
    void featureGroupChanged(const int &featureId, const QString &oldGroup);
    void featureCommentChanged(const int &featureId);
    void featureIsUpdatedChanged(const int &featureId);
    void featureIsSolvedChanged(const int &featureId);
    void featureFunctionListChanged(const int &featureId);
    void featureUsedForChanged(const int &featureId);
    void featurePreviouslyNeededChanged(const int &featureId);
    void featureIsActiveChanged(const int &featureId);

protected:

    //######################################
    //update feature id when the job was set
    //######################################

    virtual void setUpFeatureId();

    //##################
    //feature attributes
    //##################

    QString name;
    QString group;
    QString comment;

    bool isSolved; //true of the feature is solved in the current disply system
    bool isUpdated; //helper attribute to indicate if a feature was already recalced during recalculation process

    bool isActiveFeature;

    QList< QPointer<Function> > functionList;

    QList< QPointer<FeatureWrapper> > usedFor; //features which need this feature to recalc
    QList< QPointer<FeatureWrapper> > previouslyNeeded; //features which are needed to recalc this feature

    //######################################
    //this pointer wrapped by FeatureWrapper
    //######################################

    QPointer<FeatureWrapper> selfFeature;

    //#############################################
    //pointer to OiJob that this feature belongs to
    //#############################################

    virtual void setJob(const QPointer<OiJob> &job);

    QPointer<OiJob> job;

};

Q_DECLARE_METATYPE( Feature* )

#endif // FEATURE_H
