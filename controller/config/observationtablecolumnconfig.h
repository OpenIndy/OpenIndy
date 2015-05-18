#ifndef OBSERVATIONTABLECOLUMNCONFIG_H
#define OBSERVATIONTABLECOLUMNCONFIG_H

#include <QList>

#include "types.h"

/*!
 * \brief The ObservationTableColumnConfig class
 * Used to specify which attributes of an observation shall be displayed
 */
class ObservationTableColumnConfig
{
public:
    ObservationTableColumnConfig();

    bool getColumnVisibility(const ObservationDisplayattributes &column) const;
    void setColumnVisibility(const ObservationDisplayattributes &column, const bool &isVisible);

    int getColumnPosition(const ObservationDisplayattributes &column) const;
    void setColumnPosition(const ObservationDisplayattributes &column, const int &index);

private:
    void init();

    QList<ObservationDisplayattributes> displayColumns;
};

#endif // OBSERVATIONTABLECOLUMNCONFIG_H
