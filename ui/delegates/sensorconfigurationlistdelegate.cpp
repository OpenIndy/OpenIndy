#include "sensorconfigurationlistdelegate.h"

/*!
 * \brief SensorConfigurationListDelegate::SensorConfigurationListDelegate
 * \param parent
 */
SensorConfigurationListDelegate::SensorConfigurationListDelegate(QObject * parent)
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
QWidget* SensorConfigurationListDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const{

    return new QLineEdit(parent);

}

/*!
 * \brief SensorConfigurationListDelegate::setEditorData
 * Set current input of editor widget
 * \param editor
 * \param index
 */
void SensorConfigurationListDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{

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
 * \brief SensorConfigurationListDelegate::setModelData
 * Commit the edited input of the editor widget back to the model
 * \param editor
 * \param model
 * \param index
 */
void SensorConfigurationListDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{

    //check model index
    if(!index.isValid()){
        return;
    }

    //get and check sensor config proxy model
    const SensorConfigurationProxyModel *sensorConfigProxyModel = static_cast<const SensorConfigurationProxyModel*>(index.model());
    if(sensorConfigProxyModel == NULL){
        return;
    }

    //get and check editor
    QLineEdit *customEditor = qobject_cast<QLineEdit*>(editor);
    if(customEditor == NULL){
        return;
    }

    sensorConfigProxyModel->sourceModel()->setData(index, customEditor->text());

}
