#ifndef READINGTABLECOLUMNCONFIG_H
#define READINGTABLECOLUMNCONFIG_H

#include <QList>

#include "types.h"

/*!
 * \brief The ReadingTableColumnConfig class
 * Used to specify which attributes of a reading shall be displayed
 */
class ReadingTableColumnConfig
{
public:
    ReadingTableColumnConfig();

    bool getColumnVisibility(const ReadingDisplayAttributes &column) const;
    void setColumnVisibility(const ReadingDisplayAttributes &column, const bool &isVisible);

    int getColumnPosition(const ReadingDisplayAttributes &column) const;
    void setColumnPosition(const ReadingDisplayAttributes &column, const int &index);

private:
    void init();

    QList<ReadingDisplayAttributes> displayColumns;
};

#endif // READINGTABLECOLUMNCONFIG_H
