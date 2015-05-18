#include "featuretabledelegate.h"

/*!
 * \brief FeatureTableDelegate::FeatureTableDelegate
 * \param parent
 */
FeatureTableDelegate::FeatureTableDelegate(QObject * parent)
    : QItemDelegate(parent)
{
}

/*!
 * \brief FeatureTableDelegate::createEditor
 * Create the editor widget
 * \param parent
 * \param option
 * \param index
 */
QWidget* FeatureTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const{

    //create line edit
    QLineEdit* editor = NULL;

    //check model index
    if(!index.isValid()){
        return editor;
    }

    //get and check models
    const FeatureTableProxyModel *model = static_cast<const FeatureTableProxyModel*>(index.model());
    if(model == NULL){
        return editor;
    }

    //get and check column index
    int column = model->mapToSource(index).column();
    if(column < 0 || getFeatureDisplayAttributes().size() <= column){
        return editor;
    }

    //get and check display attribute
    int attr = getFeatureDisplayAttributes().at(column);
    if(!getIsFeatureDisplayAttribute(attr)){
        return editor;
    }

    //create editor for some columns
    switch((FeatureDisplayAttributes)attr){
    case eFeatureDisplayName:
        editor = new QLineEdit(parent);
        break;
    case eFeatureDisplayComment:
        editor = new QLineEdit(parent);
        break;
    case eFeatureDisplayGroup:
        editor = new QLineEdit(parent);
        break;
    }

    return editor;

}

/*!
 * \brief FeatureTableDelegate::setEditorData
 * Set current input of editor widget
 * \param editor
 * \param index
 */
void FeatureTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{

    //check model index
    if(!index.isValid()){
        return;
    }

    //get and check models
    const FeatureTableProxyModel *model = static_cast<const FeatureTableProxyModel*>(index.model());
    if(model == NULL){
        return;
    }

    //get and check column index
    int column = model->mapToSource(index).column();
    if(column < 0 || getFeatureDisplayAttributes().size() <= column){
        return;
    }

    //get and check display attribute
    int attr = getFeatureDisplayAttributes().at(column);
    if(!getIsFeatureDisplayAttribute(attr)){
        return;
    }

    //set editor default input for some columns
    QLineEdit *customEditor = NULL;
    switch((FeatureDisplayAttributes)attr){
    case eFeatureDisplayName:
        customEditor = qobject_cast<QLineEdit*>(editor);
        if(customEditor != NULL){
            customEditor->setText(index.data().toString());
        }
        break;
    case eFeatureDisplayComment:
        customEditor = qobject_cast<QLineEdit*>(editor);
        if(customEditor != NULL){
            customEditor->setText(index.data().toString());
        }
        break;
    case eFeatureDisplayGroup:
        customEditor = qobject_cast<QLineEdit*>(editor);
        if(customEditor != NULL){
            customEditor->setText(index.data().toString());
        }
        break;
    }

}

/*!
 * \brief FeatureTableDelegate::setModelData
 * Commit the edited input of the editor widget back to the model
 * \param editor
 * \param model
 * \param index
 */
void FeatureTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{

    //check model index
    if(!index.isValid()){
        return;
    }

    //get and check models
    const FeatureTableProxyModel *featureTableProxyModel = static_cast<const FeatureTableProxyModel*>(index.model());
    if(featureTableProxyModel == NULL){
        return;
    }

    //get and check column index
    int column = featureTableProxyModel->mapToSource(index).column();
    if(column < 0 || getFeatureDisplayAttributes().size() <= column){
        return;
    }

    //get and check display attribute
    int attr = getFeatureDisplayAttributes().at(column);
    if(!getIsFeatureDisplayAttribute(attr)){
        return;
    }

    //set editor default input for some columns
    QLineEdit *customEditor = NULL;
    switch((FeatureDisplayAttributes)attr){
    case eFeatureDisplayName:
        customEditor = qobject_cast<QLineEdit*>(editor);
        if(customEditor != NULL){
            featureTableProxyModel->sourceModel()->setData(featureTableProxyModel->mapToSource(index), customEditor->text());
        }
        break;
    case eFeatureDisplayComment:
        customEditor = qobject_cast<QLineEdit*>(editor);
        if(customEditor != NULL){
            featureTableProxyModel->sourceModel()->setData(featureTableProxyModel->mapToSource(index), customEditor->text());
        }
        break;
    case eFeatureDisplayGroup:
        customEditor = qobject_cast<QLineEdit*>(editor);
        if(customEditor != NULL){
            featureTableProxyModel->sourceModel()->setData(featureTableProxyModel->mapToSource(index), customEditor->text());
        }
        break;
    }

}
