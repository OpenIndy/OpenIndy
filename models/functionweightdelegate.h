#ifndef FUNCTIONWEIGHTDELEGATE_H
#define FUNCTIONWEIGHTDELEGATE_H

#include <QItemDelegate>
#include <QLineEdit>
#include <QCheckBox>
#include <QModelIndex>
#include <QAbstractTableModel>

/*!
 * \brief The FunctionWeightDelegate class
 */
class FunctionWeightDelegate : public QItemDelegate
{
public:
    FunctionWeightDelegate(QObject *parent = 0);

    QWidget* createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    void setEditorData(QWidget * editor, const QModelIndex & index) const;
    void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;

};

#endif // FUNCTIONWEIGHTDELEGATE_H
