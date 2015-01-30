#ifndef FEATURE_H
#define FEATURE_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QString>
#include <QtXml>

#include "element.h"
#include "configuration.h"
#include "unitconverter.h"

class FeatureWrapper;
class Function;

/*!
 * \brief a feature describes all geometries or coordinate systems
 */
class Feature : public Element
{
    Q_OBJECT
public:
    explicit Feature(QObject *parent = 0);
    virtual ~Feature();

    QString getFeatureName() const;
    bool setFeatureName(QString name);

    QString getGroupName() const;
    void setGroupName(QString group);

    QString getComment() const;
    void setComment(QString comment);

    bool getIsUpdated() const;
    void setIsUpdated(bool isUpdated);

    bool getIsSolved() const;
    void setIsSolved(bool isSolved);

    Configuration::eColor getDisplayColor() const;
    void setDisplayColor(Configuration::eColor displayColor);

    bool getIsDrawn() const;
    void setIsDrawn(bool isDrawn);

    const QList<Function *> getFunctions() const;
    void addFunction(Function *f);
    bool removeFunction(int index);

    bool getIsActiveFeature() const;
    void setActiveFeatureState(bool isActiveFeature);

    QList<FeatureWrapper*> usedFor; //features which need this feature to recalc
    QList<FeatureWrapper*> previouslyNeeded; //features which are needed to recalc this feature

    virtual void recalc() = 0;

    virtual QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    virtual bool fromOpenIndyXML(QDomElement &xmlElem);

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
    virtual QString getDisplayExpansionOriginZ() const;

signals:
    void featureNameChanged(int featureId, QString oldName);
    void featureGroupChanged(int featureId);
    void featureCommentChanged(int featureId);
    void featureIsUpdatedChanged(int featureId);
    void featureIsSolvedChanged(int featureId);
    void featureFunctionListChanged(int featureId);
    void featureUsedForChanged(int featureId);
    void featurePreviouslyNeededChanged(int featureId);
    void featureDisplayColorChanged(int featureId);
    void featureIsDrawnChanged(int featureId);
    void featureIsActiveChanged(int featureId);
    void featureAboutToBeDeleted(int featureId);

protected:
    QString name;
    QString group;
    QString comment;
    bool isUpdated;
    bool isSolved;
    bool isActiveFeature;
    QList<Function*> functionList;

    Configuration::eColor displayColor;
    bool isDrawn;

};

#endif // FEATURE_H
