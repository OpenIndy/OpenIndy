#ifndef SENSORCONFIGURATIONDELEGATE_H
#define SENSORCONFIGURATIONDELEGATE_H

#include <QItemDelegate>
#include <QLineEdit>
#include <QModelIndex>
#include <QAbstractTableModel>

#include "sensorconfigurationmodel.h"
#include "sensorconfigurationproxymodel.h"

using namespace oi;

/*!
 * \brief The SensorConfigurationListDelegate class
 * Delegate to edit data in SensorConfigurationModel
 */
class SensorConfigurationListDelegate : public QItemDelegate
{
public:
    SensorConfigurationListDelegate(QObject * parent = 0);

    QWidget* createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    void setEditorData(QWidget * editor, const QModelIndex & index) const;
    void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;

};

#endif // SENSORCONFIGURATIONDELEGATE_H
