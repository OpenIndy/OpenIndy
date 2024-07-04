#ifndef FEATURESORTER_H
#define FEATURESORTER_H

#include <QPointer>

#include "feature.h"
#include "featurewrapper.h"
#include "oijob.h"

#include "featuretablesortingconfig.h"

#if defined(OI_MAIN_LIB)
#  define OI_MAIN_EXPORT Q_DECL_EXPORT
#else
#  define OI_MAIN_EXPORT Q_DECL_IMPORT
#endif

using namespace oi;

/*!
 * \brief The FeatureSorter class
 * Sorts features according to sorting settings
 */
class OI_MAIN_EXPORT FeatureSorter
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
        eSortStandard,                  // sort by: id, isNominal
        eSortBy_Name_ActNom,            // sort by: featureName, isNominal
        eSortBy_Group_Name_ActNom,      // sort by: groupName, featureName, isNominal
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
    bool sortBy_Name_ActNom(QPointer<FeatureWrapper> left, QPointer<FeatureWrapper> right) const;
    bool sortBy_Group_Name(QPointer<FeatureWrapper> left, QPointer<FeatureWrapper> right) const;

};

#endif // FEATURESORTER_H
