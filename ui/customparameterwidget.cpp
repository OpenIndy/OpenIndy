#include "customparameterwidget.h"

CustomParameterWidget::CustomParameterWidget(QWidget *parent) :
    QWidget(parent)
{
    //set up regular expressions
    this->validInt = QRegExp("^-?(0|[1-9]{1}[0-9]{1,9})$");
    this->validDouble = QRegExp("^-?(0|[1-9]{1}[0-9]{0,9})(\\.[0-9]+)?$");
    this->intValidator = new QRegExpValidator(this->validInt);
    this->doubleValidator = new QRegExpValidator(this->validDouble);

    this->myLayout = NULL;

    this->setUpGui();
}

/*!
 * \brief CustomParameterWidget::setIntParameter
 * \param intParameter
 */
void CustomParameterWidget::setIntParameter(QMap<QString, int> intParameter, FunctionConfiguration config){
    this->intParameter = intParameter;
    QMapIterator<QString, int> intIterator(config.intParameter);
    while(intIterator.hasNext()){
        intIterator.next();
        QString intKey = static_cast<QString>(intIterator.key());
        int intValue = static_cast<int>(intIterator.value());
        this->intParameter.insert(intKey, intValue);
    }
    this->setUpGui();
}

/*!
 * \brief CustomParameterWidget::setDoubleParameter
 * \param doubleParameter
 */
void CustomParameterWidget::setDoubleParameter(QMap<QString, double> doubleParameter, FunctionConfiguration config){
    this->doubleParameter = doubleParameter;
    QMapIterator<QString, double> doubleIterator(config.doubleParameter);
    while(doubleIterator.hasNext()){
        doubleIterator.next();
        QString doubleKey = static_cast<QString>(doubleIterator.key());
        double doubleValue = static_cast<double>(doubleIterator.value());
        this->doubleParameter.insert(doubleKey, doubleValue);
    }
    this->setUpGui();
}

/*!
 * \brief CustomParameterWidget::setStringParameter
 * \param stringParameter
 */
void CustomParameterWidget::setStringParameter(QMap<QString, QStringList> stringParameter, FunctionConfiguration config){
    this->stringParameter = stringParameter;
    QMapIterator<QString, QString> stringIterator(config.stringParameter);
    while(stringIterator.hasNext()){
        stringIterator.next();
        QString stringKey = static_cast<QString>(stringIterator.key());
        QString stringValue = static_cast<QString>(stringIterator.value());
        if(this->stringParameter.contains(stringKey)){
            QStringList myValues = static_cast<QStringList>(this->stringParameter.find(stringKey).value());
            myValues.removeOne(stringValue);
            myValues.prepend(stringValue);
            this->stringParameter.insert(stringKey, myValues);
        }else{
            QStringList myValues;
            myValues.append(stringValue);
            this->stringParameter.insert(stringKey, myValues);
        }
    }
    this->setUpGui();
}

/*!
 * \brief CustomParameterWidget::getFunctionConfiguration
 * Get the selected parameters
 * \return
 */
FunctionConfiguration CustomParameterWidget::getFunctionConfiguration(){
    FunctionConfiguration result;
    //set up int parameters
    QMapIterator<QString, QLineEdit*> intIterator(this->intParameterLineEdit);
    while(intIterator.hasNext()){
        intIterator.next();
        QString intKey = static_cast<QString>(intIterator.key());
        QLineEdit *intValue = static_cast<QLineEdit*>(intIterator.value());
        if(intValue->hasAcceptableInput()){ //if line edit contains an integer
            this->intParameter.insert(intKey, intValue->text().toInt());
        }
    }
    //set up double parameters
    QMapIterator<QString, QLineEdit*> doubleIterator(this->doubleParameterLineEdit);
    while(doubleIterator.hasNext()){
        doubleIterator.next();
        QString doubleKey = static_cast<QString>(doubleIterator.key());
        QLineEdit *doubleValue = static_cast<QLineEdit*>(doubleIterator.value());
        if(doubleValue->hasAcceptableInput()){ //if line edit contains a double
            this->doubleParameter.insert(doubleKey, doubleValue->text().toDouble());
        }
    }
    //set up string paramerters
    QMapIterator<QString, QComboBox*> stringIterator(this->stringParameterComboBox);
    QMap<QString, QString> stringParameterSelection;
    while(stringIterator.hasNext()){
        stringIterator.next();
        QString stringKey = static_cast<QString>(stringIterator.key());
        QComboBox *stringValue = static_cast<QComboBox*>(stringIterator.value());
        stringParameterSelection.insert(stringKey, stringValue->currentText());
    }
    //fill result
    result.intParameter = this->intParameter;
    result.doubleParameter = this->doubleParameter;
    result.stringParameter = stringParameterSelection;
    return result;
}

/*!
 * \brief CustomParameterWidget::setUpGui
 * Set up the GUI with all custom parameters
 */
void CustomParameterWidget::setUpGui(){
    //delete old GUI elements
    this->clearGuiElements();
    //get iterators for parameter maps
    QMapIterator<QString, int> intIterator(this->intParameter);
    QMapIterator<QString, double> doubleIterator(this->doubleParameter);
    QMapIterator<QString, QStringList> stringIterator(this->stringParameter);
    //set up layout
    if(this->myLayout != NULL){
        delete this->myLayout;
    }
    this->myLayout = new QFormLayout();
    //create GUI elements for each parameter
    QFont boldFont;
    boldFont.setBold(true);
    int rowIndex = 0;
    while(intIterator.hasNext()){
        intIterator.next();
        QString intKey = static_cast<QString>(intIterator.key());
        int intValue = static_cast<int>(intIterator.value());
        QLabel *intLabel = new QLabel(intKey);
        intLabel->setMinimumWidth(300);
        intLabel->setWordWrap(true);
        intLabel->setFont(boldFont);
        QLineEdit *intLineEdit = new QLineEdit(QString::number(intValue));
        intLineEdit->setValidator(this->intValidator);
        this->parameterLabel.append(intLabel);
        this->intParameterLineEdit.insert(intKey, intLineEdit);
        this->myLayout->insertRow(rowIndex, intLabel, intLineEdit);
        connect(intLineEdit, SIGNAL(textChanged(QString)), this, SLOT(emitFunctionConfigurationChanged()));
        rowIndex++;
    }
    while(doubleIterator.hasNext()){
        doubleIterator.next();
        QString doubleKey = static_cast<QString>(doubleIterator.key());
        double doubleValue = static_cast<double>(doubleIterator.value());
        QLabel *doubleLabel = new QLabel(doubleKey);
        doubleLabel->setMinimumWidth(300);
        doubleLabel->setWordWrap(true);
        doubleLabel->setFont(boldFont);
        QLineEdit *doubleLineEdit = new QLineEdit(QString::number(doubleValue));
        doubleLineEdit->setValidator(this->doubleValidator);
        this->parameterLabel.append(doubleLabel);
        this->doubleParameterLineEdit.insert(doubleKey, doubleLineEdit);
        this->myLayout->insertRow(rowIndex, doubleLabel, doubleLineEdit);
        connect(doubleLineEdit, SIGNAL(textChanged(QString)), this, SLOT(emitFunctionConfigurationChanged()));
        rowIndex++;
    }
    while(stringIterator.hasNext()){
        stringIterator.next();
        QString stringKey = static_cast<QString>(stringIterator.key());
        QStringList stringValue = static_cast<QStringList>(stringIterator.value());
        QLabel *stringLabel = new QLabel(stringKey);
        stringLabel->setMinimumWidth(300);
        stringLabel->setWordWrap(true);
        stringLabel->setFont(boldFont);
        QComboBox *stringComboBox = new QComboBox();
        stringComboBox->addItems(stringValue);
        this->parameterLabel.append(stringLabel);
        this->stringParameterComboBox.insert(stringKey, stringComboBox);
        this->myLayout->insertRow(rowIndex, stringLabel, stringComboBox);
        connect(stringComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(emitFunctionConfigurationChanged(QString)));
        rowIndex++;
    }
    this->setLayout(this->myLayout);
}

/*!
 * \brief CustomParameterWidget::clearGuiElements
 * Clears all GUI elements in this widget
 */
void CustomParameterWidget::clearGuiElements(){
    //get iterator for maps with GUI elements
    QMapIterator<QString, QLineEdit*> intIterator(this->intParameterLineEdit);
    QMapIterator<QString, QLineEdit*> doubleIterator(this->doubleParameterLineEdit);
    QMapIterator<QString, QComboBox*> stringIterator(this->stringParameterComboBox);
    //delete GUI elements
    while(intIterator.hasNext()){
        delete static_cast<QLineEdit*>(intIterator.next().value());
    }
    while(doubleIterator.hasNext()){
        delete static_cast<QLineEdit*>(doubleIterator.next().value());
    }
    while(stringIterator.hasNext()){
        delete static_cast<QComboBox*>(stringIterator.next().value());
    }
    foreach(QLabel *label, this->parameterLabel){
        delete label;
    }
    //clear containers
    this->parameterLabel.clear();
    this->intParameterLineEdit.clear();
    this->doubleParameterLineEdit.clear();
    this->stringParameterComboBox.clear();
}

/*!
 * \brief CustomParameterWidget::clearAll
 */
void CustomParameterWidget::clearAll(){
    this->intParameter.clear();
    this->doubleParameter.clear();
    this->stringParameter.clear();
    this->clearGuiElements();
}

/*!
 * \brief CustomParameterWidget::emitFunctionConfigurationChanged
 */
void CustomParameterWidget::emitFunctionConfigurationChanged(){
    emit this->functionConfigurationChanged();
}

/*!
 * \brief CustomParameterWidget::emitFunctionConfigurationChanged
 * \param value
 */
void CustomParameterWidget::emitFunctionConfigurationChanged(QString value){
    emit this->functionConfigurationChanged();
}
