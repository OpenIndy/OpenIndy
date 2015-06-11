#include "createfeaturedialog.h"
#include "ui_createfeaturedialog.h"

/*!
 * \brief CreateFeatureDialog::CreateFeatureDialog
 * \param parent
 */
CreateFeatureDialog::CreateFeatureDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::CreateFeatureDialog)
{
    this->ui->setupUi(this);

    //init GUI elements and assign models
    this->initGUI();
    this->initModels();
}

/*!
 * \brief CreateFeatureDialog::~CreateFeatureDialog
 */
CreateFeatureDialog::~CreateFeatureDialog(){
    delete this->ui;
}

/*!
 * \brief CreateFeatureDialog::setFeatureType
 * \param type
 */
void CreateFeatureDialog::setFeatureType(const FeatureTypes &type){
    this->typeOfFeature = type;
}

/*!
 * \brief CreateFeatureDialog::on_toolButton_ok_clicked
 */
void CreateFeatureDialog::on_toolButton_ok_clicked(){

    //get feature attributes from GUI elements and emit add features signal
    FeatureAttributes attributes;
    this->featureAttributesFromGUI(attributes);
    emit this->addFeatures(attributes);

    //get selected function
    sdb::Function function = this->functionListModel->getFunctionAtIndex(this->ui->comboBox_function->currentIndex());
    if(function.name.compare("") == 0 || function.plugin.name.compare("") == 0){
        this->close();
        return;
    }

    //get and cast source model
    AvailableFunctionsListModel *source_model = dynamic_cast<AvailableFunctionsListModel *>(this->functionListModel->sourceModel());
    if(source_model == NULL){
        this->close();
        return;
    }

    //set default function
    QPair<QString, QString> functionPlugin;
    functionPlugin.first = function.name;
    functionPlugin.second = function.plugin.file_path;
    source_model->setDefaultFunction(this->typeOfFeature, functionPlugin);

    this->close();

}

/*!
 * \brief CreateFeatureDialog::on_toolButton_cancel_clicked
 */
void CreateFeatureDialog::on_toolButton_cancel_clicked(){
    this->close();
}

/*!
 * \brief CreateFeatureDialog::on_checkBox_nominal_toggled
 * \param checked
 */
void CreateFeatureDialog::on_checkBox_nominal_toggled(bool checked){

    //set visible state of nominal system combo box
    if(checked){
        this->ui->label_nominalSystem->setVisible(true);
        this->ui->comboBox_nominalSystem->setVisible(true);
    }else{
        this->ui->label_nominalSystem->setVisible(false);
        this->ui->comboBox_nominalSystem->setVisible(false);
    }

}

/*!
 * \brief CreateFeatureDialog::showEvent
 * \param event
 */
void CreateFeatureDialog::showEvent(QShowEvent *event){

    //put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );

    //init GUI elements based on the current feature type
    this->initGUI();

    //init function models based on the current feature type
    this->initFunctionsModel();

    event->accept();

}

/*!
 * \brief CreateFeatureDialog::initGUI
 * Set up GUI based on the selected type of feature
 */
void CreateFeatureDialog::initGUI(){

    //general defaults
    this->ui->lineEdit_name->setText("");
    this->ui->spinBox_count->setValue(1);

    //GUI inits based on type of feature
    if(this->typeOfFeature == eTrafoParamFeature){

        //set visibility
        this->ui->label_startSystem->setVisible(true);
        this->ui->comboBox_startSystem->setVisible(true);
        this->ui->label_destinationSystem->setVisible(true);
        this->ui->comboBox_destinationSystem->setVisible(true);
        this->ui->label_function->setVisible(false);
        this->ui->comboBox_function->setVisible(false);
        this->ui->label_nominalSystem->setVisible(false);
        this->ui->comboBox_nominalSystem->setVisible(false);
        this->ui->checkBox_actual->setVisible(false);
        this->ui->checkBox_nominal->setVisible(false);
        this->ui->checkBox_common->setVisible(false);
        this->ui->label_entityType->setVisible(false);
        this->ui->comboBox_entityType->setVisible(false);
        this->ui->checkBox_movement->setVisible(true);

        //set checked state
        this->ui->checkBox_movement->setChecked(false);

    }else if(this->typeOfFeature == eStationFeature ||
             this->typeOfFeature == eCoordinateSystemFeature){

        //set visibility
        this->ui->label_startSystem->setVisible(false);
        this->ui->comboBox_startSystem->setVisible(false);
        this->ui->label_destinationSystem->setVisible(false);
        this->ui->comboBox_destinationSystem->setVisible(false);
        this->ui->label_function->setVisible(false);
        this->ui->comboBox_function->setVisible(false);
        this->ui->label_nominalSystem->setVisible(false);
        this->ui->comboBox_nominalSystem->setVisible(false);
        this->ui->checkBox_actual->setVisible(false);
        this->ui->checkBox_nominal->setVisible(false);
        this->ui->checkBox_common->setVisible(false);
        this->ui->checkBox_movement->setVisible(false);
        this->ui->label_entityType->setVisible(false);
        this->ui->comboBox_entityType->setVisible(false);

    }else if(getIsScalarEntity(this->typeOfFeature)){

        //set visibility
        this->ui->label_startSystem->setVisible(false);
        this->ui->comboBox_startSystem->setVisible(false);
        this->ui->label_destinationSystem->setVisible(false);
        this->ui->comboBox_destinationSystem->setVisible(false);
        this->ui->label_function->setVisible(true);
        this->ui->comboBox_function->setVisible(true);
        this->ui->label_nominalSystem->setVisible(false);
        this->ui->comboBox_nominalSystem->setVisible(false);
        this->ui->checkBox_actual->setVisible(true);
        this->ui->checkBox_nominal->setVisible(true);
        this->ui->checkBox_common->setVisible(true);
        this->ui->checkBox_movement->setVisible(false);
        this->ui->label_entityType->setVisible(true);
        this->ui->comboBox_entityType->setVisible(true);

        //set checked state
        this->ui->checkBox_actual->setChecked(true);
        this->ui->checkBox_nominal->setChecked(false);
        this->ui->checkBox_common->setChecked(false);

    }else{ //geometry

        //set visibility
        this->ui->label_startSystem->setVisible(false);
        this->ui->comboBox_startSystem->setVisible(false);
        this->ui->label_destinationSystem->setVisible(false);
        this->ui->comboBox_destinationSystem->setVisible(false);
        this->ui->label_function->setVisible(true);
        this->ui->comboBox_function->setVisible(true);
        this->ui->label_nominalSystem->setVisible(false);
        this->ui->comboBox_nominalSystem->setVisible(false);
        this->ui->checkBox_actual->setVisible(true);
        this->ui->checkBox_nominal->setVisible(true);
        this->ui->checkBox_common->setVisible(true);
        this->ui->checkBox_movement->setVisible(false);
        this->ui->label_entityType->setVisible(false);
        this->ui->comboBox_entityType->setVisible(false);

        //set checked state
        this->ui->checkBox_actual->setChecked(true);
        this->ui->checkBox_nominal->setChecked(false);
        this->ui->checkBox_common->setChecked(false);

    }

}

/*!
 * \brief CreateFeatureDialog::initModels
 */
void CreateFeatureDialog::initModels(){

    //set models for possible start and destination systems of a trafo param
    this->ui->comboBox_startSystem->setModel(&ModelManager::getCoordinateSystemsModel());
    this->ui->comboBox_destinationSystem->setModel(&ModelManager::getCoordinateSystemsModel());

    //set model for possible nominal systems of a nominal geometry
    this->ui->comboBox_nominalSystem->setModel(&ModelManager::getNominalSystemsModel());

    //set model for available functions
    this->functionListModel = ModelManager::getAvailableFunctionsProxyModel();
    this->ui->comboBox_function->setModel(this->functionListModel);

    //set model for available group names
    this->ui->comboBox_group->setModel(&ModelManager::getGroupNamesModel());

    //set model for available scalar entities
    this->ui->comboBox_entityType->setModel(&ModelManager::getScalarEntityTypeNamesModel());

}

/*!
 * \brief CreateFeatureDialog::initFunctionsModel
 * Sets the model of the function combo box and selects the default function of the specified feature type
 */
void CreateFeatureDialog::initFunctionsModel(){

    //set the filter for the function list model
    this->functionListModel->setFilter(this->typeOfFeature);

    //get and cast source model
    AvailableFunctionsListModel *source_model = dynamic_cast<AvailableFunctionsListModel *>(this->functionListModel->sourceModel());
    if(source_model == NULL){
        return;
    }

    //select default function
    this->ui->comboBox_function->setCurrentText(source_model->getDefaultFunction(this->typeOfFeature).first);

}

/*!
 * \brief CreateFeatureDialog::featureAttributesFromGUI
 * \param attributes
 */
void CreateFeatureDialog::featureAttributesFromGUI(FeatureAttributes &attributes){

    //fill general attributes
    attributes.typeOfFeature = this->typeOfFeature;
    attributes.count = this->ui->spinBox_count->value();
    attributes.name = this->ui->lineEdit_name->text();
    attributes.group = this->ui->comboBox_group->currentText();

    //fill feature type specific attributes
    if(this->typeOfFeature == eTrafoParamFeature){

        attributes.startSystem = this->ui->comboBox_startSystem->currentText();
        attributes.destinationSystem = this->ui->comboBox_destinationSystem->currentText();
        attributes.isMovement = this->ui->checkBox_movement->isChecked();

    }else if(getIsScalarEntity(this->typeOfFeature)){

        attributes.typeOfFeature = getFeatureTypeEnum(this->ui->comboBox_entityType->currentText());
        attributes.isActual = this->ui->checkBox_actual->isChecked();
        attributes.isNominal = this->ui->checkBox_nominal->isChecked();
        attributes.isCommon = this->ui->checkBox_common->isChecked();
        attributes.nominalSystem = this->ui->comboBox_nominalSystem->currentText();

    }else if(this->typeOfFeature != eCoordinateSystemFeature
             && this->typeOfFeature != eStationFeature){

        attributes.isActual = this->ui->checkBox_actual->isChecked();
        attributes.isNominal = this->ui->checkBox_nominal->isChecked();
        attributes.isCommon = this->ui->checkBox_common->isChecked();
        attributes.nominalSystem = this->ui->comboBox_nominalSystem->currentText();

    }

    //fill selected function plugin
    sdb::Function function = this->functionListModel->getFunctionAtIndex(this->ui->comboBox_function->currentIndex());
    attributes.functionPlugin.first = function.name;
    attributes.functionPlugin.second = function.plugin.file_path;

}
