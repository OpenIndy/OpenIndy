#ifndef OBSERVATIONTABLEDELEGATE_H
#define OBSERVATIONTABLEDELEGATE_H

#include <QItemDelegate>
#include <QLineEdit>
#include <QModelIndex>
#include <QAbstractTableModel>

#include "observationproxymodel.h"
#include "observationmodel.h"

using namespace oi;

/*!
 * \brief The ObservationTableDelegate class
 */
class ObservationTableDelegate : public QItemDelegate
{
public:
    ObservationTableDelegate(QObject * parent = 0);

    QWidget* createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    void setEditorData(QWidget * editor, const QModelIndex & index) const;
    void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;

};

#endif // OBSERVATIONTABLEDELEGATE_H
