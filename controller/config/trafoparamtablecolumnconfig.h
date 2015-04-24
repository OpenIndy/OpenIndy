#ifndef TRAFOPARAMTABLECOLUMNCONFIG_H
#define TRAFOPARAMTABLECOLUMNCONFIG_H

#include <QList>

#include "types.h"

/*!
 * \brief The TrafoParamTableColumnConfig class
 * Used to specify which attributes of a feature shall be displayed (e.g. trafo param table view)
 */
class TrafoParamTableColumnConfig
{
public:
    TrafoParamTableColumnConfig();

    bool getColumnVisibility(const TrafoParamDisplayAttributes &column) const;
    void setColumnVisibility(const TrafoParamDisplayAttributes &column, const bool &isVisible);

    int getColumnPosition(const TrafoParamDisplayAttributes &column) const;
    void setColumnPosition(const TrafoParamDisplayAttributes &column, const int &index);

private:
    void init();

    QList<TrafoParamDisplayAttributes> displayColumns;
};

#endif // TRAFOPARAMTABLECOLUMNCONFIG_H
