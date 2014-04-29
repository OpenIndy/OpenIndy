#include "trafoparamdelegate.h"

TrafoParamDelegate::TrafoParamDelegate(QObject * parent)
    : QItemDelegate(parent)
{
}

/*!
 * \brief TrafoParamDelegate::createEditor
 * Create the editor widget
 * \param parent
 * \param option
 * \param index
 */
QWidget* TrafoParamDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    QLineEdit* myEditor = new QLineEdit(parent);
    const TrafoParamProxyModel *myModel = static_cast<const TrafoParamProxyModel*>(index.model());
    if(myModel != NULL && (myModel->mapToSource(index).column() == 1 || myModel->mapToSource(index).column() == 2 || myModel->mapToSource(index).column() == 32)){ //column feature name or comment
        return myEditor;
    }
    return NULL;
}

/*!
 * \brief TrafoParamDelegate::setEditorData
 * Set current input of editor widget
 * \param editor
 * \param index
 */
void TrafoParamDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{
    const TrafoParamProxyModel *myModel = static_cast<const TrafoParamProxyModel*>(index.model());
    if(myModel != NULL && (myModel->mapToSource(index).column() == 1 || myModel->mapToSource(index).column() == 2 || myModel->mapToSource(index).column() == 32)){ //column feature name or comment
         QLineEdit* myEditor = qobject_cast<QLineEdit*>(editor);
         if(myEditor != NULL){
             myEditor->setText(index.data().toString());
         }
    }
}

/*!
 * \brief TrafoParamDelegate::setModelData
 * Commit the edited input of the editor widget back to the model
 * \param editor
 * \param model
 * \param index
 */
void TrafoParamDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{
    const TrafoParamProxyModel *myModel = static_cast<const TrafoParamProxyModel*>(index.model());
    if(myModel != NULL && (myModel->mapToSource(index).column() == 1 || myModel->mapToSource(index).column() == 2 || myModel->mapToSource(index).column() == 32)){ //column feature name or comment
        QLineEdit* myEditor = qobject_cast<QLineEdit*>(editor);
        if(myEditor != NULL){
            model->setData(index, myEditor->text());
        }
    }
}
