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
    //QCheckBox* myCheckbox = new QCheckBox(parent);
    QComboBox* myCombobox = new QComboBox(parent);
    QDateTimeEdit* myDateTimeEdit = new QDateTimeEdit(parent);
    const TrafoParamProxyModel *myModel = static_cast<const TrafoParamProxyModel*>(index.model());
    if(myModel != NULL && (myModel->mapToSource(index).column() == 1 || myModel->mapToSource(index).column() == 2 || myModel->mapToSource(index).column() == 32)){ //column feature name or comment
        return myEditor;
    }
    if(myModel != NULL && myModel->mapToSource(index).column() == 33){
        //myCheckbox->setText("use");
        //return myCheckbox;
        myCombobox->addItem("true");
        myCombobox->addItem("false");
        return myCombobox;
    }
    if(myModel != NULL && myModel->mapToSource(index).column() == 34){
        return myDateTimeEdit;
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
    if(myModel != NULL && myModel->mapToSource(index).column() == 33){
        /*QCheckBox* myCheckbox = qobject_cast<QCheckBox*>(editor);
        if(myCheckbox != NULL){
            myCheckbox->setChecked(index.data().toBool());
            myCheckbox->setText("use");
        }*/
        QComboBox* myCombobox = qobject_cast<QComboBox*>(editor);
        if(myCombobox != NULL){
            bool state = index.data().toBool();
            if(state){
                myCombobox->setCurrentText("true");
            }else{
                myCombobox->setCurrentText("false");
            }
        }
    }
    if(myModel != NULL && myModel->mapToSource(index).column() == 34){
        QDateTimeEdit* myDateTimeEdit = qobject_cast<QDateTimeEdit*>(editor);
        myDateTimeEdit->setDateTime(index.data().toDateTime());
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
    if(myModel != NULL && myModel->mapToSource(index).column() == 33){
        /*QCheckBox* myCheckbox = qobject_cast<QCheckBox*>(editor);
        if(myCheckbox != NULL){
            model->setData(index,myCheckbox->isChecked());
        }*/
        QComboBox* myCombobox = qobject_cast<QComboBox*>(editor);
        if(myCombobox != NULL){
            QString state = myCombobox->currentText();
            if(state.compare("true") == 0){
                model->setData(index,true);
            }else{
                model->setData(index,false);
            }
        }
    }
    if(myModel != NULL && myModel->mapToSource(index).column() == 34){
        QDateTimeEdit* myDateTimeEdit = qobject_cast<QDateTimeEdit*>(editor);
        model->setData(index,myDateTimeEdit->dateTime());
    }
}
