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
    Q_OBJECT

public:
    explicit Feature(QObject *parent = 0);

    Feature(const Feature &copy, QObject *parent = 0);

    Feature &operator=(const Feature &copy);

    virtual ~Feature();

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

    //Configuration::eColor getDisplayColor() const;
    //void setDisplayColor(Configuration::eColor displayColor);

    const bool &getIsDrawn() const;
    void setIsDrawn(const bool &isDrawn);

    const QList<QPointer<Function> > &getFunctions() const;
    void addFunction(const QPointer<Function> &function);
    bool removeFunction(const int &index);

    const bool &getIsActiveFeature() const;
    void setActiveFeatureState(const bool &isActiveFeature);

    //###########################
    //reexecute the function list
    //###########################

    virtual void recalc();

    //#################
    //save and load XML
    //#################

    virtual QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    virtual bool fromOpenIndyXML(QDomElement &xmlElem);

/*
    virtual QString getDisplayX(bool showDiff) const;
    virtual QString getDisplayY(bool showDiff) const;
    virtual QString getDisplayZ(bool showDiff) const;
    virtual QString getDisplayI(bool showDiff) const;
    virtual QString getDisplayJ(bool showDiff) const;
    virtual QString getDisplayK(bool showDiff) const;
    virtual QString getDisplayRadius(bool showDiff) const;
    virtual QString getDisplayIsCommon() const;
    virtual QString getDisplayIsNominal() const;
    virtual QString getDisplayObs() const;
    virtual QString getDisplaySolved() const;
    virtual QString getDisplayMConfig() const;
    virtual QString getDisplayStdDev() const;
    virtual QString getDisplayScalarDistanceValue() const;
    virtual QString getDisplayScalarAngleValue() const;
    virtual QString getDisplayStartSystem() const;
    virtual QString getDisplayDestinationSystem() const;
    virtual QString getDisplayTranslationX() const;
    virtual QString getDisplayTranslationY() const;
    virtual QString getDisplayTranslationZ() const;
    virtual QString getDisplayScaleX() const;
    virtual QString getDisplayScaleY() const;
    virtual QString getDisplayScaleZ() const;
    virtual QString getDisplayRotationX() const;
    virtual QString getDisplayRotationY() const;
    virtual QString getDisplayRotationZ() const;
    virtual QString getDisplayScalarTemperatureValue() const;
    virtual QString getDisplayScalarMeasurementSeriesValue() const;
    virtual QString getDisplayUse() const;
    virtual QString getDisplayTime() const;
    virtual QString getDisplayExpansionOriginX() const;
    virtual QString getDisplayExpansionOriginY() const;
    virtual QString getDisplayExpansionOriginZ() const;*/

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
    void featureDisplayColorChanged(const int &featureId);
    void featureIsDrawnChanged(const int &featureId);
    void featureIsActiveChanged(const int &featureId);
    void featureAboutToBeDeleted(const int &featureId);

protected:

    //##################
    //feature attributes
    //##################

    QString name;
    QString group;
    QString comment;
    bool isUpdated;
    bool isSolved;
    bool isActiveFeature;
    QList< QPointer<Function> > functionList;

    QList< QPointer<FeatureWrapper> > usedFor; //features which need this feature to recalc
    QList< QPointer<FeatureWrapper> > previouslyNeeded; //features which are needed to recalc this feature

    //Configuration::eColor displayColor;
    bool isDrawn;

};

#endif // FEATURE_H
