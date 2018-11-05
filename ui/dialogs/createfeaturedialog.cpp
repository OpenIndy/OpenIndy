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
 * \brief CreateFeatureDialog::featureCreated
 * \param created
 * \return
 */
bool CreateFeatureDialog::featureCreated(bool created)
{
    this->created = created;
    return created;
}

/*!
 * \brief CreateFeatureDialog::setCurrentJob
 * \param job
 */
void CreateFeatureDialog::setCurrentJob(const QPointer<OiJob> &job)
{
    this->currentJob = job;
}

/*!
 * \brief CreateFeatureDialog::on_toolButton_ok_clicked
 */
void CreateFeatureDialog::on_toolButton_ok_clicked(){

    this->created = false;

    //get feature attributes from GUI elements and emit add features signal
    FeatureAttributes attributes;
    this->featureAttributesFromGUI(attributes);
    emit this->addFeatures(attributes);

    //only set default function, if an actual feature has been created
    if(!attributes.isActual){
        if(this->created){
            this->close();
        }
        return;
    }

    //get selected function
    sdb::Function function = this->functionListModel->getFunctionAtIndex(this->ui->comboBox_function->currentIndex());
    if(function.name.compare("") == 0 || function.plugin.name.compare("") == 0){
        if(this->created){
            this->close();
        }
        return;
    }

    //get and cast source model
    AvailableFunctionsListModel *source_model = dynamic_cast<AvailableFunctionsListModel *>(this->functionListModel->sourceModel());
    if(source_model == NULL){
        if(this->created){
            this->close();
        }
        return;
    }

    //set default function
    QPair<QString, QString> functionPlugin;
    functionPlugin.first = function.name;
    functionPlugin.second = function.plugin.file_path;
    source_model->setDefaultFunction(this->typeOfFeature, functionPlugin);

    //set default measurement config
    SystemDbManager::setDefaultMeasurementConfig(attributes.mConfig , getFeatureTypeName(this->typeOfFeature));

    if(this->created){
        this->close();
    }
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

    //set visible states of actual/nominal dependent GUI elements
    this->toggleNominalLabels(checked);
}

/*!
 * \brief CreateFeatureDialog::on_checkBox_actual_toggled
 * \param checked
 */
void CreateFeatureDialog::on_checkBox_actual_toggled(bool checked){

    //set visible states of actual/nominal dependent GUI elements
    this->toggleActualLabels(checked);
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

    this->ui->comboBox_entityType->setCurrentText(getFeatureTypeName(this->typeOfFeature));

    this->setDialogName();

    //get default mConfig in combobox
    QString elementConfigName = SystemDbManager::getDefaultMeasurementConfig(getElementTypeName(getElementTypeEnum(this->typeOfFeature)));
    if(!elementConfigName.isEmpty()){
        this->ui->comboBox_mConfig->setCurrentText(elementConfigName);
    }

    event->accept();
}

/*!
 * \brief CreateFeatureDialog::initGUI
 * Set up GUI based on the selected type of feature
 */
void CreateFeatureDialog::initGUI(){

    //general defaults
    this->ui->lineEdit_name->setText("");
    this->ui->comboBox_group->setCurrentIndex(0);
    this->ui->spinBox_count->setValue(1);
    this->ui->checkBox_actual->setChecked(true);
    this->ui->checkBox_common->setChecked(false);
    this->ui->checkBox_nominal->setChecked(false);

    this->ui->lineEdit_name->setFocus();

    if(!this->currentJob.isNull()){
        //set default feature name depending on existing features
        int featureCount = this->currentJob->getFeatureCount();
        QString featureName = "";
        if(featureCount < 9){
            featureName.append("Feature").append("0").append(QString::number(featureCount+1));
        }else{
            featureName.append("Feature").append(QString::number(featureCount+1));
        }
        this->ui->lineEdit_name->setText(featureName);
    }else{
        this->ui->lineEdit_name->setText("");
    }

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
        this->ui->label_mConfig->setVisible(false);
        this->ui->comboBox_mConfig->setVisible(false);

    }else if(this->typeOfFeature == eStationFeature ||
             this->typeOfFeature == eCoordinateSystemFeature){

        if(!this->currentJob.isNull()){
            //predefined Stationname depending on existing stations
            if(this->typeOfFeature == eStationFeature){
                int count = this->currentJob->getStationsList().size();
                QString stationName = "";
                if(count < 9){
                    stationName.append("STATION").append("0").append(QString::number(count+1));
                }else{
                    stationName.append("STATION").append(QString::number(count+1));
                }
                this->ui->lineEdit_name->setText(stationName);
            }
        }else{
            this->ui->lineEdit_name->setText("");
        }

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
        this->ui->label_entityType->setVisible(false);
        this->ui->comboBox_entityType->setVisible(false);
        this->ui->label_mConfig->setVisible(false);
        this->ui->comboBox_mConfig->setVisible(false);

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
        this->ui->label_entityType->setVisible(true);
        this->ui->comboBox_entityType->setVisible(true);
        this->ui->label_mConfig->setVisible(true);
        this->ui->comboBox_mConfig->setVisible(true);

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
        this->ui->label_entityType->setVisible(false);
        this->ui->comboBox_entityType->setVisible(false);
        this->ui->label_mConfig->setVisible(true);
        this->ui->comboBox_mConfig->setVisible(true);

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

    //set combobox size
    int sizeSystem =oi::getDropDownMenuSize(ModelManager::getCoordinateSystemsModel().stringList(), this->ui->comboBox_startSystem->width());
    this->ui->comboBox_startSystem->view()->setMinimumWidth(sizeSystem);
    this->ui->comboBox_destinationSystem->view()->setMinimumWidth(sizeSystem);

    //set model for possible nominal systems of a nominal geometry
    this->ui->comboBox_nominalSystem->setModel(&ModelManager::getNominalSystemsModel());
    int sizeNominal = oi::getDropDownMenuSize(ModelManager::getNominalSystemsModel().stringList(), this->ui->comboBox_nominalSystem->width());
    this->ui->comboBox_nominalSystem->view()->setMinimumWidth(sizeNominal);

    //set model for available functions
    this->functionListModel = ModelManager::getAvailableFunctionsProxyModel();
    this->ui->comboBox_function->setModel(this->functionListModel);

    //set model for available group names
    this->ui->comboBox_group->setModel(&ModelManager::getGroupNamesModel());

    //set model for available scalar entities
    this->ui->comboBox_entityType->setModel(&ModelManager::getScalarEntityTypeNamesModel());

    //set model for available measurement configs
    this->ui->comboBox_mConfig->setModel(&ModelManager::getMeasurementConfigurationModel());

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
 * \brief CreateFeatureDialog::toggleActualLabels
 * \param toggle
 */
void CreateFeatureDialog::toggleActualLabels(bool toggle)
{
    this->ui->label_function->setVisible(toggle);
    this->ui->comboBox_function->setVisible(toggle);
    this->ui->label_mConfig->setVisible(toggle);
    this->ui->comboBox_mConfig->setVisible(toggle);
}

/*!
 * \brief CreateFeatureDialog::toggleNominalLabels
 * \param toggle
 */
void CreateFeatureDialog::toggleNominalLabels(bool toggle)
{
    this->ui->label_nominalSystem->setVisible(toggle);
    this->ui->comboBox_nominalSystem->setVisible(toggle);
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

    }else if(getIsScalarEntity(this->typeOfFeature)){

        attributes.typeOfFeature = getFeatureTypeEnum(this->ui->comboBox_entityType->currentText());
        attributes.isActual = this->ui->checkBox_actual->isChecked();
        attributes.isNominal = this->ui->checkBox_nominal->isChecked();
        attributes.isCommon = this->ui->checkBox_common->isChecked();
        attributes.nominalSystem = this->ui->comboBox_nominalSystem->currentText();
        attributes.mConfig = this->ui->comboBox_mConfig->currentText();

    }else if(this->typeOfFeature != eCoordinateSystemFeature
             && this->typeOfFeature != eStationFeature){

        attributes.isActual = this->ui->checkBox_actual->isChecked();
        attributes.isNominal = this->ui->checkBox_nominal->isChecked();
        attributes.isCommon = this->ui->checkBox_common->isChecked();
        attributes.nominalSystem = this->ui->comboBox_nominalSystem->currentText();
        attributes.mConfig = this->ui->comboBox_mConfig->currentText();

    }

    //fill selected function plugin
    if(attributes.isActual){
        sdb::Function function = this->functionListModel->getFunctionAtIndex(this->ui->comboBox_function->currentIndex());
        attributes.functionPlugin.first = function.name;
        attributes.functionPlugin.second = function.plugin.file_path;
    }
}

void CreateFeatureDialog::setDialogName()
{

    QString title = "create " + getFeatureTypeName(typeOfFeature);
    this->setWindowTitle(title);
}

/*!
 * \brief CreateFeatureDialog::on_comboBox_entityType_currentIndexChanged check if the type of the scalar entity changed
 * \param arg1
 */
void CreateFeatureDialog::on_comboBox_entityType_currentIndexChanged(const QString &arg1)
{
    this-> typeOfFeature = getFeatureTypeEnum(arg1);
    this->initModels();
    this->initFunctionsModel();
}
