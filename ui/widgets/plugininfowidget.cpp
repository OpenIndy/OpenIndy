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
 * \param function
 */
void PluginInfoWidget::displayFunction(const sdb::Function &function){

    //delete old GUI elements
    this->reset();

    //fill GUI elements for the selected function
    this->label_description->setText("function description:");
    this->txt_description->setText(function.description);
    this->label_applicableFor->setText("applicable for:");
    this->label_neededElements->setText("needed elements:");

    //enable needed GUI elements
    this->label_description->setVisible(true);
    this->txt_description->setVisible(true);
    this->label_applicableFor->setVisible(true);
    this->label_neededElements->setVisible(true);
    foreach(const FeatureTypes &featureType, function.applicableFor){
        if(this->applicableFor.contains(featureType)){
            this->applicableFor[featureType]->setVisible(true);
        }
    }
    foreach(const ElementTypes &elementType, function.neededElements){
        if(this->neededElements.contains(elementType)){
            this->neededElements[elementType]->setVisible(true);
        }
    }

}

/*!
 * \brief PluginInfoWidget::displaySensor
 * Set up the widget to display information about a given sensor
 * \param mySensor
 */
void PluginInfoWidget::displaySensor(const sdb::Sensor &sensor){

    //delete old GUI elements
    this->clearLayout(this->myLayout);

    //fill GUI elements for the selected sensor
    this->label_description->setText("sensor description:");
    this->txt_description->setText(sensor.description);

    //enable needed GUI elements
    this->label_description->setVisible(true);
    this->txt_description->setVisible(true);

}

/*!
 * \brief PluginInfoWidget::displaySimulation
 * \param simulation
 */
void PluginInfoWidget::displaySimulation(const sdb::Simulation &simulation){

    //delete old GUI elements
    this->clearLayout(this->myLayout);

    //fill GUI elements for the selected simulation
    this->label_description->setText("simulation description:");
    this->txt_description->setText(simulation.description);

    //enable needed GUI elements
    this->label_description->setVisible(true);
    this->txt_description->setVisible(true);

}

/*!
 * \brief PluginInfoWidget::displayTool
 * \param tool
 */
void PluginInfoWidget::displayTool(const sdb::Tool &tool){

    //delete old GUI elements
    this->clearLayout(this->myLayout);

    //fill GUI elements for the selected tool
    this->label_description->setText("tool description:");
    this->txt_description->setText(tool.description);

    //enable needed GUI elements
    this->label_description->setVisible(true);
    this->txt_description->setVisible(true);

}

/*!
 * \brief PluginInfoWidget::displayNetworkAdjustment
 * \param networkAdjustment
 */
void PluginInfoWidget::displayNetworkAdjustment(const sdb::NetworkAdjustment &networkAdjustment){

    //delete old GUI elements
    this->clearLayout(this->myLayout);

    //fill GUI elements for the selected network adjustment
    this->label_description->setText("network adjustment description:");
    this->txt_description->setText(networkAdjustment.description);

    //enable needed GUI elements
    this->label_description->setVisible(true);
    this->txt_description->setVisible(true);

}

/*!
 * \brief PluginInfoWidget::displayExchange
 * \param exchange
 */
void PluginInfoWidget::displayExchange(const sdb::Exchange &exchange){

    //delete old GUI elements
    this->clearLayout(this->myLayout);

    //fill GUI elements for the selected exchange
    this->label_description->setText("exchange description:");
    this->txt_description->setText(exchange.description);

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
    foreach(const FeatureTypes &featureType, getAvailableFeatureTypes()){
        if(this->applicableFor.contains(featureType)){
            this->applicableFor[featureType]->setVisible(false);
        }
    }
    foreach(const ElementTypes &elementType, getAvailableElementTypes()){
        if(this->neededElements.contains(elementType)){
            this->neededElements[elementType]->setVisible(false);
        }
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

    //stretch GUI elements so the left space is at the bottom
    this->myLayout->setStretch(0, 0);
    this->myLayout->setStretch(1, 0);
    this->myLayout->setStretch(2, 0);
    this->myLayout->setStretch(3, 0);
    this->myLayout->setStretch(4, 0);
    this->myLayout->setStretch(5, 0);
    this->myLayout->setStretch(6, 0);
    this->myLayout->setStretch(7, 1);

    //add labels with icons of all elements / features
    foreach(const FeatureTypes &featureType, getAvailableFeatureTypes()){
        QLabel *feature = new QLabel();
        feature->setPixmap(QPixmap(getFeatureTypeIconPath(featureType)));
        feature->setFixedHeight(30);
        feature->setFixedWidth(30);
        feature->setScaledContents(true);
        feature->setVisible(false);
        this->layout_applicableFor->addWidget(feature);
        this->applicableFor.insert(featureType, feature);
    }
    foreach(const ElementTypes &elementType, getAvailableElementTypes()){
        QLabel *element = new QLabel();
        element->setPixmap(QPixmap(getElementTypeIconPath(elementType)));
        element->setFixedHeight(30);
        element->setFixedWidth(30);
        element->setScaledContents(true);
        element->setVisible(false);
        this->layout_neededElements->addWidget(element);
        this->neededElements.insert(elementType, element);
    }
    this->layout_applicableFor->addSpacerItem(featureSpacer);
    this->layout_neededElements->addSpacerItem(elementSpacer);

    //stretch applicableFor and neededElements
    for(int i = 0; i < this->applicableFor.size(); i++){
        this->layout_applicableFor->setStretch(i, 0);
    }
    this->layout_applicableFor->setStretch(this->applicableFor.size(), 1);
    for(int i = 0; i < this->neededElements.size(); i++){
        this->layout_neededElements->setStretch(i, 0);
    }
    this->layout_neededElements->setStretch(this->neededElements.size(), 1);

}

void PluginInfoWidget::clearLayout(QLayout *layout){

}
