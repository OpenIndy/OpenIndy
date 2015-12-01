#ifndef SENSORPARAMETERSDELEGATE_H
#define SENSORPARAMETERSDELEGATE_H

#include <QItemDelegate>
#include <QLineEdit>
#include <QModelIndex>
#include <QAbstractTableModel>

#include "sensorparametersmodel.h"
#include "modelmanager.h"

/*!
 * \brief The SensorParametersDelegate class
 * Delegate to edit data in SensorParametersModel
 */
class SensorParametersDelegate : public QItemDelegate
{
public:
    SensorParametersDelegate(QObject * parent = 0);

    QWidget* createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    void setEditorData(QWidget * editor, const QModelIndex & index) const;
    void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;

    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;

};

#endif // SENSORPARAMETERSDELEGATE_H
