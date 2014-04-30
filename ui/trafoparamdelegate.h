#ifndef TRAFOPARAMDELEGATE_H
#define TRAFOPARAMDELEGATE_H

#include <QItemDelegate>
#include <QLineEdit>

#include "trafoparamproxymodel.h"

class TrafoParamDelegate : public QItemDelegate
{
public:
    TrafoParamDelegate(QObject * parent = 0);

    QWidget* createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    void setEditorData(QWidget * editor, const QModelIndex & index) const;
    void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;
};

#endif // TRAFOPARAMDELEGATE_H
