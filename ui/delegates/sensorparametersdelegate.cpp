#include "sensorparametersdelegate.h"

/*!
 * \brief SensorParametersDelegate::SensorParametersDelegate
 * \param parent
 */
SensorParametersDelegate::SensorParametersDelegate(QObject * parent)
    : QItemDelegate(parent)
{
}

/*!
 * \brief SensorParametersDelegate::createEditor
 * Create the editor widget
 * \param parent
 * \param option
 * \param index
 */
QWidget* SensorParametersDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const{

    QWidget *editor = NULL;

    //check model index
    if(!index.isValid()){
        return editor;
    }

    //get and check model
    const SensorParametersModel *model = static_cast<const SensorParametersModel*>(index.model());
    if(model == NULL){
        return editor;
    }

    //get sensor config
    const SensorConfiguration &sConfig = model->getSensorConfiguration();

    //get row and column indices
    int rowIndex = index.row();
    int columnIndex = index.column();

    //create editor
    if(rowIndex < sConfig.getStringParameter().uniqueKeys().size()){

        //create editor with model
        editor = new QComboBox(parent);
        QStringListModel *model = new QStringListModel(sConfig.getAvailableStringParameter().values(sConfig.getStringParameter().uniqueKeys().at(rowIndex)), editor);
        static_cast<QComboBox*>(editor)->setModel(model);

    }else if(rowIndex < sConfig.getDoubleParameter().size() + sConfig.getStringParameter().uniqueKeys().size()){
        rowIndex = rowIndex - sConfig.getStringParameter().uniqueKeys().size();
        editor = new QLineEdit(parent);
    }else{
        rowIndex = rowIndex - sConfig.getStringParameter().uniqueKeys().size()
                - sConfig.getDoubleParameter().size();
        editor = new QLineEdit(parent);
    }

    return editor;

}

/*!
 * \brief SensorParametersDelegate::setEditorData
 * Set current input of editor widget
 * \param editor
 * \param index
 */
void SensorParametersDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{

    //check model index
    if(!index.isValid()){
        return;
    }

    //get and check model
    const SensorParametersModel *model = static_cast<const SensorParametersModel*>(index.model());
    if(model == NULL){
        return;
    }

    //get sensor config
    const SensorConfiguration &sConfig = model->getSensorConfiguration();

    //get row and column indices
    int rowIndex = index.row();
    int columnIndex = index.column();

    //set editor default input
    if(rowIndex < sConfig.getStringParameter().uniqueKeys().size()){

        QComboBox *customEditor = NULL;
        customEditor = qobject_cast<QComboBox*>(editor);
        if(customEditor != NULL){
            customEditor->setCurrentText(index.data().toString());
        }

    }else if(rowIndex < sConfig.getDoubleParameter().size() + sConfig.getStringParameter().uniqueKeys().size()){

        rowIndex = rowIndex - sConfig.getStringParameter().uniqueKeys().size();
        QLineEdit *customEditor = NULL;
        customEditor = qobject_cast<QLineEdit*>(editor);
        if(customEditor != NULL){
            customEditor->setText(index.data().toString());
        }

    }else{

        rowIndex = rowIndex - sConfig.getStringParameter().uniqueKeys().size()
                - sConfig.getDoubleParameter().size();
        QLineEdit *customEditor = NULL;
        customEditor = qobject_cast<QLineEdit*>(editor);
        if(customEditor != NULL){
            customEditor->setText(index.data().toString());
        }

    }

}

/*!
 * \brief SensorParametersDelegate::setModelData
 * Commit the edited input of the editor widget back to the model
 * \param editor
 * \param model
 * \param index
 */
void SensorParametersDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{

    //check model index
    if(!index.isValid()){
        return;
    }

    //get and check model
    const SensorParametersModel *sensorParamModel = static_cast<const SensorParametersModel*>(index.model());
    if(sensorParamModel == NULL){
        return;
    }

    //get sensor config
    const SensorConfiguration &sConfig = sensorParamModel->getSensorConfiguration();

    //get row and column indices
    int rowIndex = index.row();
    int columnIndex = index.column();

    //commit changes
    if(rowIndex < sConfig.getStringParameter().uniqueKeys().size()){

        QComboBox *customEditor = qobject_cast<QComboBox*>(editor);
        if(customEditor != NULL){
            model->setData(index, customEditor->currentText());
        }

    }else if(rowIndex < sConfig.getDoubleParameter().size() + sConfig.getStringParameter().uniqueKeys().size()){

        rowIndex = rowIndex - sConfig.getStringParameter().uniqueKeys().size();
        QLineEdit *customEditor = qobject_cast<QLineEdit*>(editor);
        if(customEditor != NULL){
            model->setData(index, customEditor->text());
        }

    }else{

        rowIndex = rowIndex - sConfig.getStringParameter().uniqueKeys().size()
                - sConfig.getDoubleParameter().size();
        QLineEdit *customEditor = qobject_cast<QLineEdit*>(editor);
        if(customEditor != NULL){
            model->setData(index, customEditor->text());
        }

    }

}

/*!
 * \brief SensorParametersDelegate::sizeHint
 * \param option
 * \param index
 * \return
 */
QSize SensorParametersDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const{

    //check model index
    if(!index.isValid()){
        return QSize(5, 22);
    }

    //get and check models
    const SensorParametersModel *model = static_cast<const SensorParametersModel*>(index.model());
    if(model == NULL){
        return QSize(5, 22);
    }

    QString input = model->data(index).toString();

    QFontMetrics metrics(option.font);

    int width = metrics.width(input) + option.decorationSize.width();

    return QSize(width, 22);

}
