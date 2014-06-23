#include "plugininfowidget.h"

PluginInfoWidget::PluginInfoWidget(QWidget *parent) :
    QWidget(parent), myLayout(NULL), label_description(NULL), txt_description(NULL),
    layout_applicableFor(NULL), layout_neededElements(NULL)
{
    this->initGUI();
}

/*!
 * \brief PluginInfoWidget::displayFunction
 * Set up the widget to display information about a given function
 * \param myFunction
 */
void PluginInfoWidget::displayFunction(FunctionPlugin myFunction){

    //delete old GUI elements
    this->reset();

    //fill GUI elements for the selected function
    this->label_description->setText("function description:");
    this->txt_description->setText(myFunction.description);
    this->label_applicableFor->setText("applicable for:");
    this->label_neededElements->setText("needed elements:");

    //enable needed GUI elements
    this->label_description->setVisible(true);
    this->txt_description->setVisible(true);
    this->label_applicableFor->setVisible(true);
    this->label_neededElements->setVisible(true);
    foreach(Configuration::FeatureTypes featureType, myFunction.applicableFor){
        this->applicableFor.at(featureType)->setVisible(true);
    }
    foreach(Configuration::ElementTypes elementType, myFunction.neededElements){
        this->neededElements.at(elementType)->setVisible(true);
    }

}

/*!
 * \brief PluginInfoWidget::displaySensor
 * Set up the widget to display information about a given sensor
 * \param mySensor
 */
void PluginInfoWidget::displaySensor(SensorPlugin mySensor){

    //delete old GUI elements
    this->clearLayout(this->myLayout);

    //fill GUI elements for the selected sensor
    this->label_description->setText("sensor description:");
    this->txt_description->setText(mySensor.description);

    //enable needed GUI elements
    this->label_description->setVisible(true);
    this->txt_description->setVisible(true);

}

/*!
 * \brief PluginInfoWidget::reset
 * Reset GUI elements when a tree item other than sensor or function was selected
 */
void PluginInfoWidget::reset(){

    this->label_description->setVisible(false);
    this->txt_description->setVisible(false);
    this->label_applicableFor->setVisible(false);
    this->label_neededElements->setVisible(false);
    for(int i = 0; i < 19; i++){
        this->applicableFor.at(i)->setVisible(false);
    }
    for(int i = 0; i < 24; i++){
        this->neededElements.at(i)->setVisible(false);
    }

}

/*!
 * \brief PluginInfoWidget::init
 */
void PluginInfoWidget::initGUI(){

    //set up layout
    this->myLayout = new QVBoxLayout();
    this->myLayout->setContentsMargins(0,0,0,0);
    this->setLayout(this->myLayout);

    //set up items
    this->label_description = new QLabel();
    this->txt_description = new QTextBrowser();
    this->label_applicableFor = new QLabel();
    this->label_neededElements = new QLabel();
    this->layout_applicableFor = new QHBoxLayout();
    this->layout_neededElements = new QHBoxLayout();

    //disable by default
    this->label_description->setVisible(false);
    this->txt_description->setVisible(false);
    this->label_applicableFor->setVisible(false);
    this->label_neededElements->setVisible(false);

    QFont bold;
    bold.setBold(true);
    this->label_description->setFont(bold);
    this->label_applicableFor->setFont(bold);
    this->label_neededElements->setFont(bold);

    this->txt_description->setMinimumHeight(70);
    this->txt_description->setMaximumHeight(70);

    QSpacerItem *bottomSpacer = new QSpacerItem(20,20,QSizePolicy::Expanding);
    QSpacerItem *featureSpacer = new QSpacerItem(20,20,QSizePolicy::Expanding);
    QSpacerItem *elementSpacer = new QSpacerItem(20,20,QSizePolicy::Expanding);

    //add items to layout
    this->myLayout->addSpacing(20);
    this->myLayout->addWidget(this->label_description);
    this->myLayout->addWidget(this->txt_description);
    this->myLayout->addWidget(this->label_applicableFor);
    this->myLayout->addLayout(this->layout_applicableFor);
    this->myLayout->addWidget(this->label_neededElements);
    this->myLayout->addLayout(this->layout_neededElements);
    this->myLayout->addSpacerItem(bottomSpacer);

    //streatch GUI elements so the left space is at the bottom
    this->myLayout->setStretch(0, 0);
    this->myLayout->setStretch(1, 0);
    this->myLayout->setStretch(2, 0);
    this->myLayout->setStretch(3, 0);
    this->myLayout->setStretch(4, 0);
    this->myLayout->setStretch(5, 0);
    this->myLayout->setStretch(6, 0);
    this->myLayout->setStretch(7, 1);

    //add labels with icons of all elements / features
    for(int i = 0; i < 19; i++){
        QLabel *myFeature = new QLabel();
        myFeature->setPixmap(Configuration::getFeatureIcon((Configuration::FeatureTypes)i));
        myFeature->setFixedHeight(20);
        myFeature->setFixedWidth(20);
        myFeature->setScaledContents(true);
        myFeature->setVisible(false);
        this->layout_applicableFor->addWidget(myFeature);
        this->applicableFor.append(myFeature);
    }
    for(int i = 0; i < 24; i++){
        QLabel *myElement = new QLabel();
        myElement->setPixmap(Configuration::getElementIcon((Configuration::ElementTypes)i));
        myElement->setFixedHeight(20);
        myElement->setFixedWidth(20);
        myElement->setScaledContents(true);
        myElement->setVisible(false);
        this->layout_neededElements->addWidget(myElement);
        this->neededElements.append(myElement);
    }
    this->layout_applicableFor->addSpacerItem(featureSpacer);
    this->layout_neededElements->addSpacerItem(elementSpacer);

    //stretch applicableFor and neededElements
    for(int i = 0; i < 19; i++){
        this->layout_applicableFor->setStretch(i, 0);
    }
    this->layout_applicableFor->setStretch(19, 1);
    for(int i = 0; i < 24; i++){
        this->layout_neededElements->setStretch(i, 0);
    }
    this->layout_neededElements->setStretch(24, 1);

}

void PluginInfoWidget::clearLayout(QLayout *layout){

}
