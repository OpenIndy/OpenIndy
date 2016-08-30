#include "trafoparamtabledelegate.h"

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

    //create editor
    QWidget *editor = NULL;

    //check model index
    if(!index.isValid()){
        return editor;
    }

    //get and check models
    const TrafoParamTableProxyModel *model = static_cast<const TrafoParamTableProxyModel*>(index.model());
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
    if(!getIsTrafoParamDisplayAttribute(attr)){
        return editor;
    }

    //create editor for some columns
    switch((TrafoParamDisplayAttributes)attr){
    case eTrafoParamDisplayName:
        editor = new QLineEdit(parent);
        break;
    case eTrafoParamDisplayComment:
        editor = new QLineEdit(parent);
        break;
    case eTrafoParamDisplayGroup:
        editor = new QLineEdit(parent);
        break;
    /*case eTrafoParamDisplayIsUsed: {
        QComboBox *comboEdit = new QComboBox(parent);
        comboEdit->addItem("true");
        comboEdit->addItem("false");
        editor = comboEdit;
        break;
    }case eTrafoParamDisplayIsDatumTransformation:{
        QComboBox *comboEdit = new QComboBox(parent);
        comboEdit->addItem("true");
        comboEdit->addItem("false");
        editor = comboEdit;
        break;
    }*/}

    return editor;

}

/*!
 * \brief TrafoParamDelegate::setEditorData
 * Set current input of editor widget
 * \param editor
 * \param index
 */
void TrafoParamDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{

    //check model index
    if(!index.isValid()){
        return;
    }

    //get and check models
    const TrafoParamTableProxyModel *model = static_cast<const TrafoParamTableProxyModel*>(index.model());
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
    if(!getIsTrafoParamDisplayAttribute(attr)){
        return;
    }

    //create editor for some columns
    switch((TrafoParamDisplayAttributes)attr){
    case eTrafoParamDisplayName:{
        QLineEdit* myEditor = qobject_cast<QLineEdit*>(editor);
        myEditor->setText(index.data().toString());
        break;
    }case eTrafoParamDisplayComment:{
        QLineEdit* myEditor = qobject_cast<QLineEdit*>(editor);
        myEditor->setText(index.data().toString());
        break;
    }case eTrafoParamDisplayGroup:{
        QLineEdit* myEditor = qobject_cast<QLineEdit*>(editor);
        myEditor->setText(index.data().toString());
        break;
    }/*case eTrafoParamDisplayIsUsed:{
        QComboBox* myEditor = qobject_cast<QComboBox*>(editor);
        myEditor->setCurrentText(index.data().toBool()?"true":"false");
        break;
    }case eTrafoParamDisplayIsDatumTransformation:{
        QComboBox* myEditor = qobject_cast<QComboBox*>(editor);
        myEditor->setCurrentText(index.data().toBool()?"true":"false");
        break;
    }*/
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

    //check model index
    if(!index.isValid()){
        return;
    }

    //get and check models
    const TrafoParamTableProxyModel *trafoModel = static_cast<const TrafoParamTableProxyModel*>(index.model());
    if(trafoModel == NULL){
        return;
    }

    //get and check column index
    int column = trafoModel->mapToSource(index).column();
    if(column < 0 || getFeatureDisplayAttributes().size() <= column){
        return;
    }

    //get and check display attribute
    int attr = getFeatureDisplayAttributes().at(column);
    if(!getIsTrafoParamDisplayAttribute(attr)){
        return;
    }

    //create editor for some columns
    switch((TrafoParamDisplayAttributes)attr){
    case eTrafoParamDisplayName:{
        QLineEdit* myEditor = qobject_cast<QLineEdit*>(editor);
        trafoModel->sourceModel()->setData(trafoModel->mapToSource(index), myEditor->text());
        break;
    }case eTrafoParamDisplayComment:{
        QLineEdit* myEditor = qobject_cast<QLineEdit*>(editor);
        trafoModel->sourceModel()->setData(trafoModel->mapToSource(index), myEditor->text());
        break;
    }case eTrafoParamDisplayGroup:{
        QLineEdit* myEditor = qobject_cast<QLineEdit*>(editor);
        trafoModel->sourceModel()->setData(trafoModel->mapToSource(index), myEditor->text());
        break;
    }/*case eTrafoParamDisplayIsUsed:{
        QComboBox* myEditor = qobject_cast<QComboBox*>(editor);
        trafoModel->sourceModel()->setData(trafoModel->mapToSource(index), (myEditor->currentText().compare("true") == 0));
        break;
    }case eTrafoParamDisplayIsDatumTransformation:{
        QComboBox* myEditor = qobject_cast<QComboBox*>(editor);
        trafoModel->sourceModel()->setData(trafoModel->mapToSource(index), (myEditor->currentText().compare("true") == 0));
        break;
    }*/
    }

}
