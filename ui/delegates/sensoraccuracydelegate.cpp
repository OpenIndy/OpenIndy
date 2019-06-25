#include "sensoraccuracydelegate.h"

/*!
 * \brief SensorAccuracyDelegate::SensorAccuracyDelegate
 * \param parent
 */
SensorAccuracyDelegate::SensorAccuracyDelegate(QObject * parent)
    : QItemDelegate(parent)
{
}

/*!
 * \brief SensorAccuracyDelegate::createEditor
 * Create the editor widget
 * \param parent
 * \param option
 * \param index
 */
QWidget* SensorAccuracyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const{

    //create line edit
    QLineEdit* editor = NULL;

    //check model index
    if(!index.isValid()){
        return editor;
    }

    //get and check model
    const SensorAccuracyModel *model = static_cast<const SensorAccuracyModel*>(index.model());
    if(model == NULL){
        return editor;
    }

    //get row and column indices
    int rowIndex = index.row();
    int columnIndex = index.column();

    //sigma type headers
    if(rowIndex == 0 || rowIndex == 4 || rowIndex == 8 || rowIndex == 12 || rowIndex == 14){
        return editor;
    }

    //create editor
    editor = new QLineEdit(parent);

    return editor;

}

/*!
 * \brief SensorAccuracyDelegate::setEditorData
 * Set current input of editor widget
 * \param editor
 * \param index
 */
void SensorAccuracyDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{

    //check model index
    if(!index.isValid()){
        return;
    }

    //get and check model
    const SensorAccuracyModel *model = static_cast<const SensorAccuracyModel*>(index.model());
    if(model == NULL){
        return;
    }

    //get row and column indices
    int rowIndex = index.row();
    int columnIndex = index.column();

    //sigma type headers
    if(rowIndex == 0 || rowIndex == 4 || rowIndex == 8 || rowIndex == 12 || rowIndex == 14){
        return;
    }

    //set editor default input
    QLineEdit *customEditor = NULL;
    customEditor = qobject_cast<QLineEdit*>(editor);
    if(customEditor != NULL){
        customEditor->setText(index.data().toString());
    }

}

/*!
 * \brief SensorAccuracyDelegate::setModelData
 * Commit the edited input of the editor widget back to the model
 * \param editor
 * \param model
 * \param index
 */
void SensorAccuracyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{

    //check model index
    if(!index.isValid()){
        return;
    }

    //get row and column indices
    int rowIndex = index.row();
    int columnIndex = index.column();

    //sigma type headers
    if(rowIndex == 0 || rowIndex == 4 || rowIndex == 8 || rowIndex == 12 || rowIndex == 14){
        return;
    }

    //commit changes
    QLineEdit *customEditor = qobject_cast<QLineEdit*>(editor);
    if(customEditor != NULL){
        model->setData(index, customEditor->text());
    }

}
