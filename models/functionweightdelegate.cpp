#include "functionweightdelegate.h"

/*!
 * \brief FunctionWeightDelegate::FunctionWeightDelegate
 * \param parent
 */
FunctionWeightDelegate::FunctionWeightDelegate(QObject *parent) : QItemDelegate(parent)
{

}

/*!
 * \brief FunctionWeightDelegate::createEditor
 * \param parent
 * \param option
 * \param index
 * \return
 */
QWidget *FunctionWeightDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return NULL;
}

/*!
 * \brief FunctionWeightDelegate::setEditorData
 * \param editor
 * \param index
 */
void FunctionWeightDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    return;
}

void FunctionWeightDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    return;
}
