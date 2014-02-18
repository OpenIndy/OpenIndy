#ifndef FEATURE_H
#define FEATURE_H

#include <QList>
#include <QMap>
#include <QString>

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

public:
    virtual ~Feature();

    QString name;
    QString group;
    bool isUpdated;
    bool isSolved;
    QList<Function*> functionList;
    QList<FeatureWrapper*> usedFor; //features which need this feature to recalc
    QList<FeatureWrapper*> previouslyNeeded; //features which are needed to recalc this feature
    Configuration::eColor displayColor;

    bool isDrawn;

    void addFunction(Function *f);

    virtual void recalc() = 0;

    virtual QString getDisplayX() const{
        return "-/-";
    }
    virtual QString getDisplayY() const{
        return "-/-";
    }
    virtual QString getDisplayZ() const{
        return "-/-";
    }
    virtual QString getDisplayI() const{
        return "-/-";
    }
    virtual QString getDisplayJ() const{
        return "-/-";
    }
    virtual QString getDisplayK() const{
        return "-/-";
    }
    virtual QString getDisplayRadius() const{
        return "-/-";
    }
    Configuration::eColor getDisplayColor() const{
        return displayColor;
    }
    virtual QString getDisplayIsCommon() const{
        return "-/-";
    }
    virtual QString getDisplayIsNominal() const{
        return "-/-";
    }
    void setColor(Configuration::eColor c){
        this->displayColor = c;
    }
    virtual QString getDisplayObs() const{
        return "-/-";
    }
    virtual QString getDisplaySolved() const{
        return "-/-";
    }
    virtual QString getDisplayMConfig() const{
        return "-/-";
    }
    virtual QString getDisplayStdDev() const{
        return "-/-";
    }
    virtual QString getDisplayScalarDistanceValue() const{
        return "-/-";
    }
    virtual QString getDisplayScalarAngleValue() const{
        return "-/-";
    }
    virtual QString getDisplayStartSystem() const{
        return "-/-";
    }
    virtual QString getDisplayDestinationSystem() const{
        return "-/-";
    }
    virtual QString getDisplayTranslationX() const{
        return "-/-";
    }
    virtual QString getDisplayTranslationY() const{
        return "-/-";
    }
    virtual QString getDisplayTranslationZ() const{
        return "-/-";
    }
    virtual QString getDisplayScaleX() const{
        return "-/-";
    }
    virtual QString getDisplayScaleY() const{
        return "-/-";
    }
    virtual QString getDisplayScaleZ() const{
        return "-/-";
    }
    virtual QString getDisplayRotationX() const{
        return "-/-";
    }
    virtual QString getDisplayRotationY() const{
        return "-/-";
    }
    virtual QString getDisplayRotationZ() const{
        return "-/-";
    }
    virtual QString getDisplayScalarTemperatureValue() const{
        return "-/-";
    }
    virtual QString getDisplayScalarMeasurementSeriesValue() const{
        return "-/-";
    }


};

#endif // FEATURE_H
