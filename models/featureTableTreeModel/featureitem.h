#ifndef FEATUREITEM_H
#define FEATUREITEM_H

#include <QVariant>
#include <QList>

#include "oijob.h"
#include "types.h"
#include "util.h"
#include "parameterdisplayconfig.h"
#include "console.h"
#include "measurementconfigmanager.h"

using namespace oi;

class FeatureItem
{
    friend class FeatureItem;

public:
    explicit FeatureItem(const QPointer<FeatureWrapper> &data, FeatureViewState state, FeatureItem *parantItem = 0);
    explicit FeatureItem(FeatureItem *parentItem = 0);
    ~FeatureItem();

    void appendChild(FeatureItem *child);

    void setData(const QPointer<FeatureWrapper> &data, FeatureViewState state);

    FeatureItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column);
    int row() const;
    FeatureItem *parentItem();

    void deleteChildren();

    FeatureViewState getState();
    QPointer<FeatureWrapper> getItemData();

private:

    QList<FeatureItem*> m_childItems;
    FeatureItem *m_parentItem;

    //data
    QPointer<FeatureWrapper> m_itemData;
    FeatureViewState m_state;

    //##############
    //display config
    //##############

    ParameterDisplayConfig parameterDisplayConfig;

    //##############
    //config manager
    //##############

    QPointer<MeasurementConfigManager> measurementConfigManager;

    //#################
    //helper attributes
    //#################

    ActualNominalFilter actualNominalFilter;
};

#endif // FEATUREITEM_H
