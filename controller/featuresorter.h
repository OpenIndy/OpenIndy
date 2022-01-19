#ifndef FEATURESORTER_H
#define FEATURESORTER_H

#include <QPointer>

#include "feature.h"
#include "featurewrapper.h"
#include "oijob.h"

#include "featuretablesortingconfig.h"

using namespace oi;

/*!
 * \brief The FeatureSorter class
 * Sorts features according to sorting settings
 */
class FeatureSorter
{
public:
    FeatureSorter();

    //##########
    //comparison
    //##########

    bool operator()(const int &left, const int &right);
    bool operator()(const QPointer<FeatureWrapper> &left, const QPointer<FeatureWrapper> &right);

    //#############
    //sorting modes
    //#############

    enum SortingMode{
        eSortStandard,                  // by id and put nominal behind actual
        eSortBy_Name_ActNom,            // by "feature name" and put nominal behind actual
        eSortBy_Group_Name_ActNom,      // by "group name", "feature name" and put nominal behind actual
        eSortBy_Id_ActNom               // same as eSortStandard
    };

    //#################################
    //get or set sorting mode an config
    //#################################

    SortingMode getSortingMode() const;
    void setSortingMode(SortingMode mode);

    const FeatureTableSortingConfig getSortingConfig() const;
    void setSortingConfig(const FeatureTableSortingConfig &config);

    //###################################
    //get or set the current OpenIndy job
    //###################################

    const QPointer<OiJob> &getCurrentJob() const;
    void setCurrentJob(const QPointer<OiJob> &job);

    //####################
    //comparison functions
    //####################

    bool lessThan(const int &left, const int &right) const;
    bool lessThan(const QPointer<FeatureWrapper> &left, const QPointer<FeatureWrapper> &right) const;

private:

    //#################
    //helper attributes
    //#################

    QPointer<OiJob> job;

    //sorting mode and config
    SortingMode mode;
    FeatureTableSortingConfig config;

    //##############
    //helper methods
    //##############

    bool sortStandard(QPointer<FeatureWrapper> left, QPointer<FeatureWrapper> right) const;
    bool sortName(QPointer<FeatureWrapper> left, QPointer<FeatureWrapper> right) const;
    bool sortGroup(QPointer<FeatureWrapper> left, QPointer<FeatureWrapper> right) const;
    bool sortId(QPointer<FeatureWrapper> left, QPointer<FeatureWrapper> right) const;

};

#endif // FEATURESORTER_H
