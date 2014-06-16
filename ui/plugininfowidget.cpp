#include "plugininfowidget.h"

PluginInfoWidget::PluginInfoWidget(QWidget *parent) :
    QWidget(parent), myLayout(NULL), label_description(NULL), txt_description(NULL),
    layout_applicableFor(NULL), layout_neededElements(NULL)
{
    //set up layout
    this->myLayout = new QVBoxLayout();
    this->myLayout->setContentsMargins(0,0,0,0);
    this->setLayout(this->myLayout);

    //set up items
    this->label_description = new QLabel();
    this->txt_description = new QTextBrowser();
}

/*!
 * \brief PluginInfoWidget::displayFunction
 * Set up the widget to display information about a given function
 * \param myFunction
 */
void PluginInfoWidget::displayFunction(FunctionPlugin myFunction){

    //delete old GUI elements
    this->clearLayout(this->myLayout);

    //set up GUI for the selected function
    this->label_description->setText("function description:");
    this->txt_description->setText(myFunction.description);

    //add Gui elements
    this->myLayout->addSpacing(20);
    this->myLayout->addWidget(this->label_description);
    this->myLayout->addWidget(this->txt_description);

}

/*!
 * \brief PluginInfoWidget::displaySensor
 * Set up the widget to display information about a given sensor
 * \param mySensor
 */
void PluginInfoWidget::displaySensor(SensorPlugin mySensor){
    //delete old GUI elements
    this->clearLayout(this->myLayout);
}

void PluginInfoWidget::setUpGui(){
    /*//delete old GUI elements
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
    this->setLayout(this->myLayout);*/
}

void PluginInfoWidget::clearLayout(QLayout *layout){

    /*QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if(item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        if(item->widget()) {
            delete item->widget();
        }
        if(item->spacerItem()){
            delete item->spacerItem();
        }
    }*/

    /*//get iterator for maps with GUI elements
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
    this->stringParameterComboBox.clear();*/
}
