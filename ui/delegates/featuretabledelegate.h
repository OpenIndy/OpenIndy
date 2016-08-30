#ifndef FEATURETABLEDELEGATE_H
#define FEATURETABLEDELEGATE_H

#include <QItemDelegate>
#include <QLineEdit>
#include <QCheckBox>
#include <QModelIndex>
#include <QAbstractTableModel>

#include "featuretableproxymodel.h"
#include "featuretablemodel.h"

using namespace oi;

/*!
 * \brief The FeatureTableDelegate class
 * Delegate to edit data in FeatureTableModel
 */
class FeatureTableDelegate : public QItemDelegate
{
public:
    FeatureTableDelegate(QObject * parent = 0);

    QWidget* createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    void setEditorData(QWidget * editor, const QModelIndex & index) const;
    void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;

    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;

};

#endif // FEATURETABLEDELEGATE_H
