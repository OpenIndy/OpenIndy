#include "scalarparameterwidget.h"

ScalarParameterWidget::ScalarParameterWidget(QWidget *parent) :
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
 * \brief ScalarParameterWidget::setIntParameter
 * \param intParameter
 */
void ScalarParameterWidget::setIntParameter(const QMap<QString, int> &intParameter){
    this->intParameter = intParameter;
    this->setUpGui();
}

/*!
 * \brief ScalarParameterWidget::setDoubleParameter
 * \param doubleParameter
 */
void ScalarParameterWidget::setDoubleParameter(const QMap<QString, double> &doubleParameter){
    this->doubleParameter = doubleParameter;
    this->setUpGui();
}

/*!
 * \brief ScalarParameterWidget::setStringParameter
 * \param stringParameter
 * \param defaultSelection
 */
void ScalarParameterWidget::setStringParameter(const QMultiMap<QString, QString> &stringParameter, const QMap<QString, QString> &defaultSelection){
    this->stringParameter.clear();
    foreach(const QString &key, stringParameter.keys()){
        this->stringParameter.insert(key, stringParameter.values(key));
    }
    this->stringParameterSelection = defaultSelection;
    this->setUpGui();
}

/*!
 * \brief ScalarParameterWidget::getIntParameter
 * \return
 */
const QMap<QString, int> &ScalarParameterWidget::getIntParameter(){
    this->getParameterFromGUI();
    return this->intParameter;
}

/*!
 * \brief ScalarParameterWidget::getDoubleParameter
 * \return
 */
const QMap<QString, double> &ScalarParameterWidget::getDoubleParameter(){
    this->getParameterFromGUI();
    return this->doubleParameter;
}

/*!
 * \brief ScalarParameterWidget::getStringParameter
 * \return
 */
const QMap<QString, QString> &ScalarParameterWidget::getStringParameter(){
    this->getParameterFromGUI();
    return this->stringParameterSelection;
}

/*!
 * \brief ScalarParameterWidget::clearAll
 */
void ScalarParameterWidget::clearAll(){

    //clear user defined parameters
    this->intParameter.clear();
    this->doubleParameter.clear();
    this->stringParameter.clear();
    this->stringParameterSelection.clear();

    //clear GUI elements
    this->clearGuiElements();

}

/*!
 * \brief ScalarParameterWidget::setUpGui
 * Set up the GUI with all custom parameters
 */
void ScalarParameterWidget::setUpGui(){

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
        stringComboBox->setCurrentText(this->stringParameterSelection.value(stringKey));
        this->parameterLabel.append(stringLabel);
        this->stringParameterComboBox.insert(stringKey, stringComboBox);
        this->myLayout->insertRow(rowIndex, stringLabel, stringComboBox);
        QObject::connect(stringComboBox, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), this, &ScalarParameterWidget::scalarParametersChanged, Qt::AutoConnection);
        rowIndex++;
    }
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
        QObject::connect(intLineEdit, &QLineEdit::textChanged, this, &ScalarParameterWidget::scalarParametersChanged, Qt::AutoConnection);
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
        QObject::connect(doubleLineEdit, &QLineEdit::textChanged, this, &ScalarParameterWidget::scalarParametersChanged, Qt::AutoConnection);
        rowIndex++;
    }

    //assign layout
    this->setLayout(this->myLayout);

}

/*!
 * \brief ScalarParameterWidget::clearGuiElements
 * Clears all GUI elements in this widget
 */
void ScalarParameterWidget::clearGuiElements(){

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
 * \brief ScalarParameterWidget::getParameterFromGUI
 * Set up user defined parameters from GUI elements
 */
void ScalarParameterWidget::getParameterFromGUI(){

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

    //set up string parameters
    QMapIterator<QString, QComboBox*> stringIterator(this->stringParameterComboBox);
    while(stringIterator.hasNext()){
        stringIterator.next();
        QString stringKey = static_cast<QString>(stringIterator.key());
        QComboBox *stringValue = static_cast<QComboBox*>(stringIterator.value());
        this->stringParameterSelection.insert(stringKey, stringValue->currentText());
    }

}


