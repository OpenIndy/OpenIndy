#ifndef OBSERVATIONTABLECOLUMNCONFIG_H
#define OBSERVATIONTABLECOLUMNCONFIG_H

#include <QList>

#include "types.h"

using namespace oi;

/*!
 * \brief The ObservationTableColumnConfig class
 * Used to specify which attributes of an observation shall be displayed
 */
class ObservationTableColumnConfig
{
public:
    ObservationTableColumnConfig();

    bool getColumnVisibility(const ObservationDisplayAttributes &column) const;
    void setColumnVisibility(const ObservationDisplayAttributes &column, const bool &isVisible);

    int getColumnPosition(const ObservationDisplayAttributes &column) const;
    void setColumnPosition(const ObservationDisplayAttributes &column, const int &index);

private:
    void init();

    QList<ObservationDisplayAttributes> displayColumns;
};

#endif // OBSERVATIONTABLECOLUMNCONFIG_H
