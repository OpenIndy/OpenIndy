#include "featureoverviewdelegate.h"

FeatureOverviewDelegate::FeatureOverviewDelegate(QObject * parent)
    : QItemDelegate(parent)
{
}

/*!
 * \brief FeatureOverviewDelegate::createEditor
 * Create the editor widget
 * \param parent
 * \param option
 * \param index
 */
QWidget* FeatureOverviewDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    QLineEdit* myEditor = new QLineEdit(parent);
    const FeatureOverviewProxyModel *myModel = static_cast<const FeatureOverviewProxyModel*>(index.model());
    const TableModel *myTblModel = static_cast<const TableModel*>(index.model());
    if(myModel != NULL && (myModel->mapToSource(index).column() == 3 || myModel->mapToSource(index).column() == 2 || myModel->mapToSource(index).column() == 12)){ //column feature name or comment
        return myEditor;

    }else if(myTblModel != NULL && myModel->mapToSource(index).column() == 22 || myModel->mapToSource(index).column() == 23 || myModel->mapToSource(index).column() == 24){
        //QModelIndex idx = createIndex(myModel->mapToSource(index).row(), 0, NULL);
        QModelIndex idx = myTblModel->index(myModel->mapToSource(index).row(), 0);
        QVariant q = myTblModel->data(idx,Qt::DisplayRole);
        QString fType = q.toString();

        if(Configuration::getFeatureTypeEnum(fType) == Configuration::eCoordinateSystemFeature){
            return myEditor;
        }

    }
    return NULL;
}

/*!
 * \brief FeatureOverviewDelegate::setEditorData
 * Set current input of editor widget
 * \param editor
 * \param index
 */
void FeatureOverviewDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{
    const FeatureOverviewProxyModel *myModel = static_cast<const FeatureOverviewProxyModel*>(index.model());
    if(myModel != NULL && (myModel->mapToSource(index).column() == 3 || myModel->mapToSource(index).column() == 2 || myModel->mapToSource(index).column() == 12)){ //column feature name or comment
         QLineEdit* myEditor = qobject_cast<QLineEdit*>(editor);
         if(myEditor != NULL){
             myEditor->setText(index.data().toString());
         }
    }else if(myModel != NULL && (myModel->mapToSource(index).column() == 22 || myModel->mapToSource(index).column() == 23 || myModel->mapToSource(index).column() == 24)){
        QLineEdit* myEditor = qobject_cast<QLineEdit*>(editor);
        if(myEditor != NULL){
            myEditor->setText(index.data().toString());
        }
    }
}

/*!
 * \brief FeatureOverviewDelegate::setModelData
 * Commit the edited input of the editor widget back to the model
 * \param editor
 * \param model
 * \param index
 */
void FeatureOverviewDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{
    const FeatureOverviewProxyModel *myModel = static_cast<const FeatureOverviewProxyModel*>(index.model());
    if(myModel != NULL && (myModel->mapToSource(index).column() == 3 || myModel->mapToSource(index).column() == 2 || myModel->mapToSource(index).column() == 12)){ //column feature name or comment
        QLineEdit* myEditor = qobject_cast<QLineEdit*>(editor);
        if(myEditor != NULL){
            model->setData(index, myEditor->text());
            return;
        }
    }else if(myModel != NULL && (myModel->mapToSource(index).column() == 22 || myModel->mapToSource(index).column() == 23 || myModel->mapToSource(index).column() == 24)){

        QLineEdit* myEditor = qobject_cast<QLineEdit*>(editor);
        if(myEditor != NULL){
            model->setData(index, myEditor->text());
            return;
        }
    }
}
