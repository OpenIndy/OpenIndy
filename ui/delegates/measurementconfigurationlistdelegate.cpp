#include "measurementconfigurationlistdelegate.h"

/*!
 * \brief MeasurementConfigurationListDelegate::MeasurementConfigurationListDelegate
 * \param parent
 */
MeasurementConfigurationListDelegate::MeasurementConfigurationListDelegate(QObject * parent)
    : QItemDelegate(parent)
{
}

/*!
 * \brief MeasurementConfigurationListDelegate::createEditor
 * Create the editor widget
 * \param parent
 * \param option
 * \param index
 */
QWidget* MeasurementConfigurationListDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const{

    return new QLineEdit(parent);

}

/*!
 * \brief MeasurementConfigurationListDelegate::setEditorData
 * Set current input of editor widget
 * \param editor
 * \param index
 */
void MeasurementConfigurationListDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{

    //check model index
    if(!index.isValid()){
        return;
    }

    //get and check editor
    QLineEdit *customEditor = qobject_cast<QLineEdit*>(editor);
    if(customEditor == NULL){
        return;
    }

    //set editor input
    customEditor->setText(index.data().toString());

}

/*!
 * \brief MeasurementConfigurationListDelegate::setModelData
 * Commit the edited input of the editor widget back to the model
 * \param editor
 * \param model
 * \param index
 */
void MeasurementConfigurationListDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{

    //check model index
    if(!index.isValid()){
        return;
    }

    //get and check sensor config proxy model
    const MeasurementConfigurationProxyModel *mConfigProxyModel = static_cast<const MeasurementConfigurationProxyModel*>(index.model());
    if(mConfigProxyModel == NULL){
        return;
    }

    //get and check editor
    QLineEdit *customEditor = qobject_cast<QLineEdit*>(editor);
    if(customEditor == NULL){
        return;
    }

    mConfigProxyModel->sourceModel()->setData(index, customEditor->text());

}
