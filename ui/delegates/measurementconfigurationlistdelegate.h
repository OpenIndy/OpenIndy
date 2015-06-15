#ifndef MEASUREMENTCONFIGURATIONDELEGATE_H
#define MEASUREMENTCONFIGURATIONDELEGATE_H

#include <QItemDelegate>
#include <QLineEdit>
#include <QModelIndex>
#include <QAbstractTableModel>

#include "measurementconfigurationmodel.h"
#include "measurementconfigurationproxymodel.h"

using namespace oi;

/*!
 * \brief The MeasurementConfigurationListDelegate class
 * Delegate to edit data in MeasurementConfigurationModel
 */
class MeasurementConfigurationListDelegate : public QItemDelegate
{
public:
    MeasurementConfigurationListDelegate(QObject * parent = 0);

    QWidget* createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    void setEditorData(QWidget * editor, const QModelIndex & index) const;
    void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;

};

#endif // MEASUREMENTCONFIGURATIONDELEGATE_H
