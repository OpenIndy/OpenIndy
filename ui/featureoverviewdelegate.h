#ifndef TABLEMODELDELEGATE_H
#define TABLEMODELDELEGATE_H

#include <QItemDelegate>
#include <QLineEdit>

#include "featureoverviewproxymodel.h"

class FeatureOverviewDelegate : public QItemDelegate
{
public:
    FeatureOverviewDelegate(QObject * parent = 0);

    QWidget* createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    void setEditorData(QWidget * editor, const QModelIndex & index) const;
    void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;
};

#endif // TABLEMODELDELEGATE_H
