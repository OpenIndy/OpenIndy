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
    /*QLineEdit* myEditor = new QLineEdit(parent);
    //QCheckBox* myCheckbox = new QCheckBox(parent);
    QComboBox* myCombobox = new QComboBox(parent);
    QDateTimeEdit* myDateTimeEdit = new QDateTimeEdit(parent);
    QComboBox* myDatumTrafoCB = new QComboBox(parent);
    const TrafoParamProxyModel *myModel = static_cast<const TrafoParamProxyModel*>(index.model());
    if(myModel != NULL && (myModel->mapToSource(index).column() == 3 || myModel->mapToSource(index).column() == 2 || myModel->mapToSource(index).column() == 12)){ //column feature name or comment
        return myEditor;
    }
    if(myModel != NULL && myModel->mapToSource(index).column() == 25){
        //myCheckbox->setText("use");
        //return myCheckbox;
        myCombobox->addItem("true");
        myCombobox->addItem("false");
        return myCombobox;
    }
    if(myModel != NULL && myModel->mapToSource(index).column() == 38){
        return myDateTimeEdit;
    }
    if(myModel != NULL && myModel->mapToSource(index).column() == 26){
        myDatumTrafoCB->addItem("true");
        myDatumTrafoCB->addItem("false");
        return myDatumTrafoCB;
    }*/
    return NULL;
}

/*!
 * \brief TrafoParamDelegate::setEditorData
 * Set current input of editor widget
 * \param editor
 * \param index
 */
void TrafoParamDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{
    /*const TrafoParamProxyModel *myModel = static_cast<const TrafoParamProxyModel*>(index.model());
    if(myModel != NULL && (myModel->mapToSource(index).column() == 3 || myModel->mapToSource(index).column() == 2 || myModel->mapToSource(index).column() == 12)){ //column feature name or comment
         QLineEdit* myEditor = qobject_cast<QLineEdit*>(editor);
         if(myEditor != NULL){
             myEditor->setText(index.data().toString());
         }
    }
    if(myModel != NULL && myModel->mapToSource(index).column() == 25){

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
    if(myModel != NULL && myModel->mapToSource(index).column() == 38){
        QDateTimeEdit* myDateTimeEdit = qobject_cast<QDateTimeEdit*>(editor);
        if(myDateTimeEdit != NULL){
            myDateTimeEdit->setDateTime(index.data().toDateTime());
        }
    }
    if(myModel != NULL && myModel->mapToSource(index).column() == 26){
        QComboBox* myDatumTrafoCB = qobject_cast<QComboBox*>(editor);
        if(myDatumTrafoCB != NULL){
            bool state = index.data().toBool();
            if(state){
                myDatumTrafoCB->setCurrentText("true");
            }else{
                myDatumTrafoCB->setCurrentText("false");
            }
        }
    }*/
}

/*!
 * \brief TrafoParamDelegate::setModelData
 * Commit the edited input of the editor widget back to the model
 * \param editor
 * \param model
 * \param index
 */
void TrafoParamDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{
    /*const TrafoParamProxyModel *myModel = static_cast<const TrafoParamProxyModel*>(index.model());
    if(myModel != NULL && (myModel->mapToSource(index).column() == 3 || myModel->mapToSource(index).column() == 2 || myModel->mapToSource(index).column() == 12)){ //column feature name or comment
        QLineEdit* myEditor = qobject_cast<QLineEdit*>(editor);
        if(myEditor != NULL){
            model->setData(index, myEditor->text());
            return;
        }
    }
    if(myModel != NULL && myModel->mapToSource(index).column() == 25){

        QComboBox* myCombobox = qobject_cast<QComboBox*>(editor);
        if(myCombobox != NULL){
            QString state = myCombobox->currentText();
            if(state.compare("true") == 0){
                model->setData(index,true);
            }else{
                model->setData(index,false);
            }
            return;
        }
    }
    if(myModel != NULL && myModel->mapToSource(index).column() == 38){
        QDateTimeEdit* myDateTimeEdit = qobject_cast<QDateTimeEdit*>(editor);
        if(myDateTimeEdit != NULL){
            model->setData(index,myDateTimeEdit->dateTime());
            return;
        }
    }
    if(myModel != NULL && myModel->mapToSource(index).column() == 26){
        QComboBox* myDatumTrafoCB = qobject_cast<QComboBox*>(editor);
        if(myDatumTrafoCB != NULL){
            QString state = myDatumTrafoCB->currentText();
            if(state.compare("true") == 0){
                model->setData(index,true);
            }else{
                model->setData(index,false);
            }
            return;
        }
    }
    return;*/
}
