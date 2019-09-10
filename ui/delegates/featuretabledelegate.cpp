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
    case eFeatureDisplayX:
        if(activeFeatureIsNominal(model)){
            editor = new QLineEdit(parent);
        }
        break;
    case eFeatureDisplayY:
        if(activeFeatureIsNominal(model)){
            editor = new QLineEdit(parent);
        }
        break;
    case eFeatureDisplayZ:
        if(activeFeatureIsNominal(model)){
            editor = new QLineEdit(parent);
        }
        break;
    case eFeatureDisplayPrimaryI:
        if(activeFeatureIsNominal(model)){
            editor = new QLineEdit(parent);
        }
        break;
    case eFeatureDisplayPrimaryJ:
        if(activeFeatureIsNominal(model)){
            editor = new QLineEdit(parent);
        }
        break;
    case eFeatureDisplayPrimaryK:
        if(activeFeatureIsNominal(model)){
            editor = new QLineEdit(parent);
        }
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
    QLineEdit *customLineEdit = NULL;
    switch((FeatureDisplayAttributes)attr){
    case eFeatureDisplayName:
        customLineEdit = qobject_cast<QLineEdit*>(editor);
        if(customLineEdit != NULL){
            customLineEdit->setText(index.data().toString());
        }
        break;
    case eFeatureDisplayComment:
        customLineEdit = qobject_cast<QLineEdit*>(editor);
        if(customLineEdit != NULL){
            customLineEdit->setText(index.data().toString());
        }
        break;
    case eFeatureDisplayGroup:
        customLineEdit = qobject_cast<QLineEdit*>(editor);
        if(customLineEdit != NULL){
            customLineEdit->setText(index.data().toString());
        }
        break;
    case eFeatureDisplayX:
        if(activeFeatureIsNominal(model)){
            customLineEdit = qobject_cast<QLineEdit*> (editor);
            if(customLineEdit != NULL){
                customLineEdit->setText(index.data().toString());
            }
        }
        break;
    case eFeatureDisplayY:
        if(activeFeatureIsNominal(model)){
            customLineEdit = qobject_cast<QLineEdit*> (editor);
            if(customLineEdit != NULL){
                customLineEdit->setText(index.data().toString());
            }
        }
        break;
    case eFeatureDisplayZ:
        if(activeFeatureIsNominal(model)){
            customLineEdit = qobject_cast<QLineEdit*> (editor);
            if(customLineEdit != NULL){
                customLineEdit->setText(index.data().toString());
            }
        }
        break;
    case eFeatureDisplayPrimaryI:
        if(activeFeatureIsNominal(model)){
            customLineEdit = qobject_cast<QLineEdit*> (editor);
            if(customLineEdit != NULL){
                customLineEdit->setText(index.data().toString());
            }
        }
        break;
    case eFeatureDisplayPrimaryJ:
        if(activeFeatureIsNominal(model)){
            customLineEdit = qobject_cast<QLineEdit*> (editor);
            if(customLineEdit != NULL){
                customLineEdit->setText(index.data().toString());
            }
        }
        break;
    case eFeatureDisplayPrimaryK:
        if(activeFeatureIsNominal(model)){
            customLineEdit = qobject_cast<QLineEdit*> (editor);
            if(customLineEdit != NULL){
                customLineEdit->setText(index.data().toString());
            }
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
    QLineEdit *customLineEdit = NULL;
    switch((FeatureDisplayAttributes)attr){
    case eFeatureDisplayName:
        customLineEdit = qobject_cast<QLineEdit*>(editor);
        if(customLineEdit != NULL){
            featureTableProxyModel->sourceModel()->setData(featureTableProxyModel->mapToSource(index), customLineEdit->text());
        }
        break;
    case eFeatureDisplayComment:
        customLineEdit = qobject_cast<QLineEdit*>(editor);
        if(customLineEdit != NULL){
            featureTableProxyModel->sourceModel()->setData(featureTableProxyModel->mapToSource(index), customLineEdit->text());
        }
        break;
    case eFeatureDisplayGroup:
        customLineEdit = qobject_cast<QLineEdit*>(editor);
        if(customLineEdit != NULL){
            featureTableProxyModel->sourceModel()->setData(featureTableProxyModel->mapToSource(index), customLineEdit->text());
        }
        break;
    case eFeatureDisplayX:
        customLineEdit = qobject_cast<QLineEdit*>(editor);
        if(activeFeatureIsNominal(featureTableProxyModel)){
            if(customLineEdit != NULL){
                featureTableProxyModel->sourceModel()->setData(featureTableProxyModel->mapToSource(index), customLineEdit->text().toDouble());
            }
        }
        break;
    case eFeatureDisplayY:
        customLineEdit = qobject_cast<QLineEdit*>(editor);
        if(activeFeatureIsNominal(featureTableProxyModel)){
            if(customLineEdit != NULL){
                featureTableProxyModel->sourceModel()->setData(featureTableProxyModel->mapToSource(index), customLineEdit->text().toDouble());
            }
        }
        break;
    case eFeatureDisplayZ:
        customLineEdit = qobject_cast<QLineEdit*>(editor);
        if(activeFeatureIsNominal(featureTableProxyModel)){
            if(customLineEdit != NULL){
                featureTableProxyModel->sourceModel()->setData(featureTableProxyModel->mapToSource(index), customLineEdit->text().toDouble());
            }
        }
        break;
    case eFeatureDisplayPrimaryI:
        customLineEdit = qobject_cast<QLineEdit*>(editor);
        if(activeFeatureIsNominal(featureTableProxyModel)){
            if(customLineEdit != NULL){
                featureTableProxyModel->sourceModel()->setData(featureTableProxyModel->mapToSource(index), customLineEdit->text().toDouble());
            }
        }
        break;
    case eFeatureDisplayPrimaryJ:
        customLineEdit = qobject_cast<QLineEdit*>(editor);
        if(activeFeatureIsNominal(featureTableProxyModel)){
            if(customLineEdit != NULL){
                featureTableProxyModel->sourceModel()->setData(featureTableProxyModel->mapToSource(index), customLineEdit->text().toDouble());
            }
        }
        break;
    case eFeatureDisplayPrimaryK:
        customLineEdit = qobject_cast<QLineEdit*>(editor);
        if(activeFeatureIsNominal(featureTableProxyModel)){
            if(customLineEdit != NULL){
                featureTableProxyModel->sourceModel()->setData(featureTableProxyModel->mapToSource(index), customLineEdit->text().toDouble());
            }
        }
        break;
    }

}

/*!
 * \brief FeatureTableDelegate::activeFeatureIsNominal
 * returns if the active feature is a nominal feature
 * \return
 */
bool FeatureTableDelegate::activeFeatureIsNominal(const FeatureTableProxyModel *featureTablePM) const
{
    if(featureTablePM->getCurrentJob().isNull()){
        return false;
    }
    if(!featureTablePM->getCurrentJob()->getActiveFeature().isNull()){
        if(!featureTablePM->getCurrentJob()->getActiveFeature()->getGeometry().isNull()){
            if(featureTablePM->getCurrentJob()->getActiveFeature()->getGeometry()->getIsNominal()){
                return true;
            }
        }
    }
    return false;
}
