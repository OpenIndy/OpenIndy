#ifndef TRAFOPARAMTABLECOLUMNCONFIG_H
#define TRAFOPARAMTABLECOLUMNCONFIG_H

#include <QList>

#include "types.h"

using namespace oi;

/*!
 * \brief The TrafoParamTableColumnConfig class
 * Used to specify which attributes of a feature shall be displayed (e.g. trafo param table view)
 */
class TrafoParamTableColumnConfig
{
public:
    TrafoParamTableColumnConfig(bool initBundle = false);

    bool getColumnVisibility(const TrafoParamDisplayAttributes &column) const;
    void setColumnVisibility(const TrafoParamDisplayAttributes &column, const bool &isVisible);

    int getColumnPosition(const TrafoParamDisplayAttributes &column) const;
    void setColumnPosition(const TrafoParamDisplayAttributes &column, const int &index);

    TrafoParamDisplayAttributes getDisplayAttributeAt(const int &column) const;

private:
    void init();
    void initBundle();

    QList<TrafoParamDisplayAttributes> displayColumns;
};

#endif // TRAFOPARAMTABLECOLUMNCONFIG_H
