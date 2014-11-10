#include "observationdelegate.h"

ObservationDelegate::ObservationDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

/*!
 * \brief createEditor adds the gui element to the view.
 * \param parent
 * \param option
 * \param index
 * \return
 */
QWidget *ObservationDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QCheckBox* cBox;
    cBox = new QCheckBox(parent);

    const ObservationProxyModel *myModel = static_cast<const ObservationProxyModel*>(index.model());
    if(myModel != NULL && (myModel->mapToSource(index).column() == 11)){
        return cBox;
    }
    return NULL;
}

/*!
 * \brief setEditorData sets current input of editor widget
 * \param editor
 * \param index
 */
void ObservationDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    const ObservationProxyModel *myModel = static_cast<const ObservationProxyModel*>(index.model());

    if(myModel != NULL && (myModel->mapToSource(index).column() == 11)){
        QCheckBox* cBox = qobject_cast<QCheckBox*>(editor);
        if(cBox != NULL){
            cBox->setChecked(index.data().toBool());
        }
    }
}

/*!
 * \brief setModelData
 * Commit the edited input of the editor widget back to the model
 * \param editor
 * \param model
 * \param index
 */
void ObservationDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    const ObservationProxyModel *myModel = static_cast<const ObservationProxyModel*>(index.model());
    if(myModel != NULL && (myModel->mapToSource(index).column() == 11)){
        QCheckBox* cBox = qobject_cast<QCheckBox*>(editor);
        if(cBox != NULL){
            model->setData(index, cBox->isChecked());
            return;
        }
    }
    return;
}
