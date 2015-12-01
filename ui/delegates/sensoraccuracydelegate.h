#ifndef SENSORACCURACYDELEGATE_H
#define SENSORACCURACYDELEGATE_H

#include <QItemDelegate>
#include <QLineEdit>
#include <QModelIndex>
#include <QAbstractTableModel>

#include "sensoraccuracymodel.h"

/*!
 * \brief The SensorAccuracyDelegate class
 * Delegate to edit data in SensorAccuracyModel
 */
class SensorAccuracyDelegate : public QItemDelegate
{
public:
    SensorAccuracyDelegate(QObject * parent = 0);

    QWidget* createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    void setEditorData(QWidget * editor, const QModelIndex & index) const;
    void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;

    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;

};

#endif // SENSORACCURACYDELEGATE_H
