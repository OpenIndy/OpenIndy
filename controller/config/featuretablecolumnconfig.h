#ifndef FEATURETABLECOLUMNCONFIG_H
#define FEATURETABLECOLUMNCONFIG_H

#include <QList>

#include "types.h"

using namespace oi;

/*!
 * \brief The FeatureTableColumnConfig class
 * Used to specify which attributes of a feature shall be displayed (e.g. feature table view)
 */
class FeatureTableColumnConfig
{
public:
    FeatureTableColumnConfig();
    FeatureTableColumnConfig(QList<FeatureDisplayAttributes>);

    bool getColumnVisibility(const FeatureDisplayAttributes &column) const;
    void setColumnVisibility(const FeatureDisplayAttributes &column, const bool &isVisible);

    int getColumnPosition(const FeatureDisplayAttributes &column) const;
    void setColumnPosition(const FeatureDisplayAttributes &column, const int &index);

    FeatureDisplayAttributes getDisplayAttributeAt(const int &column) const;

private:
    void init();

    QList<FeatureDisplayAttributes> displayColumns;
};

#endif // FEATURETABLECOLUMNCONFIG_H
