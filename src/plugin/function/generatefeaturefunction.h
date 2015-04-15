#ifndef GENERATEFEATUREFUNCTION_H
#define GENERATEFEATUREFUNCTION_H

#include "function.h"

/*!
 * \brief The GenerateFeatureFunction class
 * Function that uses its input elements to generate one or more new feature
 */
class GenerateFeatureFunction : public Function
{
    Q_OBJECT

public:
    GenerateFeatureFunction(QObject *parent = 0) : Function(parent){}

    virtual ~GenerateFeatureFunction(){}

signals:
    void addFeature(const QPointer<FeatureWrapper> &feature);
    void addFeatures(const QList<QPointer<FeatureWrapper> > &features);
};

#define GenerateFeatureFunction_iidd "de.openIndy.plugin.function.generateFeatureFunction.v001"

#endif // GENERATEFEATUREFUNCTION_H
