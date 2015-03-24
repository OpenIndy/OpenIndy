#include "createfeaturedialog.h"
#include "ui_createfeaturedialog.h"

/*!
 * \brief CreateFeature constructor. Initiates the gui.
 * \param parent
 */
CreateFeatureDialog::CreateFeatureDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::CreateFeatureDialog)
{
    this->ui->setupUi(this);
    this->initGUI();
    this->initModels();
}

/*!
 * \brief CreateFeature destructor.
 */
CreateFeatureDialog::~CreateFeatureDialog(){
    delete this->ui;
}

/*!
 * \brief CreateFeatureDialog::setFeatureType
 * \param typeOfFeature
 */
void CreateFeatureDialog::setFeatureType(const FeatureTypes &typeOfFeature){
    this->typeOfFeature = typeOfFeature;
}

/*!
 * \brief CreateFeatureDialog::showEvent
 * \param event
 */
void CreateFeatureDialog::showEvent(QShowEvent *event){
    this->initGUI();
    this->initFunctionsModel();
    event->accept();
}

/*!
 * \brief CreateFeatureDialog::initGUI
 * Set up GUI based on the selected type of feature
 */
void CreateFeatureDialog::initGUI(){

    //general GUI init
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
        this->ui->checkBox_isActual->setVisible(false);
        this->ui->checkBox_isNominal->setVisible(false);
        this->ui->checkBox_isCommon->setVisible(false);
        this->ui->checkBox_isMovement->setVisible(true);

        //set checked state
        this->ui->checkBox_isMovement->setChecked(false);

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
        this->ui->checkBox_isActual->setVisible(false);
        this->ui->checkBox_isNominal->setVisible(false);
        this->ui->checkBox_isCommon->setVisible(false);
        this->ui->checkBox_isMovement->setVisible(false);

    }else{

        //set visibility
        this->ui->label_startSystem->setVisible(false);
        this->ui->comboBox_startSystem->setVisible(false);
        this->ui->label_destinationSystem->setVisible(false);
        this->ui->comboBox_destinationSystem->setVisible(false);
        this->ui->label_function->setVisible(true);
        this->ui->comboBox_function->setVisible(true);
        this->ui->label_nominalSystem->setVisible(true);
        this->ui->comboBox_nominalSystem->setVisible(true);
        this->ui->checkBox_isActual->setVisible(true);
        this->ui->checkBox_isNominal->setVisible(true);
        this->ui->checkBox_isCommon->setVisible(true);
        this->ui->checkBox_isMovement->setVisible(false);

        //set checked state
        this->ui->checkBox_isActual->setChecked(true);
        this->ui->checkBox_isNominal->setChecked(false);
        this->ui->checkBox_isCommon->setChecked(false);

        //set function model based on selected feature type

    }

}

/*!
 * \brief CreateFeatureDialog::initModels
 */
void CreateFeatureDialog::initModels(){

    //this->ui->comboBox_startSystem->setModel(&OiModelManager::getCoordinateSystemsModel());
    //this->ui->comboBox_destinationSystem->setModel(&OiModelManager::getCoordinateSystemsModel());
    //this->ui->comboBox_nominalSystem->setModel(&OiModelManager::getCoordinateSystemsModel());

}

/*!
 * \brief CreateFeatureDialog::setFunctionsModel
 * Sets the model of the function combo box and selects the default function of the specified feature type
 */
void CreateFeatureDialog::initFunctionsModel(){

    switch(this->typeOfFeature){
    case ePointFeature:
        break;
    case eLineFeature:
        break;
    case ePlaneFeature:
        break;
    case eCircleFeature:
        break;
    case eSphereFeature:
        break;
    case ePointCloudFeature:
        break;
    case eConeFeature:
        break;
    case eCylinderFeature:
        break;
    case eEllipsoidFeature:
        break;
    case eParaboloidFeature:
        break;
    case eHyperboloidFeature:
        break;
    case eNurbsFeature:
        break;
    case eScalarEntityAngleFeature:
        break;
    case eScalarEntityDistanceFeature:
        break;
    case eScalarEntityMeasurementSeriesFeature:
        break;
    case eScalarEntityTemperatureFeature:
        break;
    }

}


















/*

void CreateFeatureDialog::setFeatureType(FeatureTypes typeOfFeature){
    this->typeOfFeature = typeOfFeature;
}


/*void CreateFeature::receiveFeatureType(FeatureTypes fT){
    this->typeOfFeature = fT;

    initGUI();

    switch(typeOfFeature){
    case eCoordinateSystemFeature:{
        ui->toolButton_mConfig->setVisible(false);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        ui->checkBox_Nominal->setEnabled(false);
        ui->checkBox_Actual->setChecked(true);
        ui->checkBox_Actual->setEnabled(false);
        ui->checkBox_common->setEnabled(false);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        this->setWindowTitle("create coordinatesystem");
        break;
    }
    case eTrafoParamFeature:{
        ui->toolButton_mConfig->setVisible(false);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(true);
        ui->label_startSystem->setVisible(true);
        ui->comboBox_destinationSystem->setVisible(true);
        ui->comboBox_startSystem->setVisible(true);
        ui->checkBox_Nominal->setEnabled(false);
        ui->checkBox_Actual->setChecked(true);
        ui->checkBox_Actual->setEnabled(false);
        ui->checkBox_common->setEnabled(false);
        ui->checkBox_movement->setVisible(true);
        ui->checkBox_movement->setEnabled(true);
        ui->checkBox_movement->setChecked(false);
        this->setWindowTitle("create transformation parameter");
        break;
    }
    case ePlaneFeature:
        ui->toolButton_mConfig->setVisible(true);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create plane");
        ui->checkBox_Nominal->setEnabled(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    case ePointFeature:
        ui->toolButton_mConfig->setVisible(true);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create point");
        ui->checkBox_Nominal->setEnabled(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    case eLineFeature:
        ui->toolButton_mConfig->setVisible(true);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create line");
        ui->checkBox_Nominal->setEnabled(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    case eStationFeature:
        ui->toolButton_mConfig->setVisible(true);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create station");
        ui->checkBox_Nominal->setChecked(false);
        ui->checkBox_Nominal->setEnabled(false);
        ui->checkBox_Actual->setChecked(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    case eSphereFeature:
        ui->toolButton_mConfig->setVisible(true);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create sphere");
        ui->checkBox_Nominal->setEnabled(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    case eCylinderFeature:
        ui->toolButton_mConfig->setVisible(true);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create cylinder");
        ui->checkBox_Nominal->setEnabled(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    case eConeFeature:
        ui->toolButton_mConfig->setVisible(true);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create cone");
        ui->checkBox_Nominal->setEnabled(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    case eEllipsoidFeature:
        ui->toolButton_mConfig->setVisible(true);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create ellipsoid");
        ui->checkBox_Nominal->setEnabled(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    case eHyperboloidFeature:
        ui->toolButton_mConfig->setVisible(true);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create hyperboloid");
        ui->checkBox_Nominal->setEnabled(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    case eParaboloidFeature:
        ui->toolButton_mConfig->setVisible(true);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create paraboloid");
        ui->checkBox_Nominal->setEnabled(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    case eNurbsFeature:
        ui->toolButton_mConfig->setVisible(true);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create nurbs");
        ui->checkBox_Nominal->setEnabled(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    case ePointCloudFeature:
        ui->toolButton_mConfig->setVisible(true);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create pointcloud");
        ui->checkBox_Nominal->setEnabled(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    case eCircleFeature:
        ui->toolButton_mConfig->setVisible(true);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create circle");
        ui->checkBox_Nominal->setEnabled(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    default:{
        ui->toolButton_mConfig->setVisible(false);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create feature");
        ui->checkBox_Nominal->setEnabled(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    }
    }

    initGUI();
}


void CreateFeatureDialog::on_toolButton_create_clicked()
{
    if(ui->spinBox_count->value()>0){

        int count = ui->spinBox_count->value();
        FeatureTypes featureType = this->typeOfFeature;
        QString name = ui->lineEdit_name->text();
        bool isActual = ui->checkBox_Actual->isChecked();
        bool isNominal = ui->checkBox_Nominal->isChecked();
        QString group = this->ui->comboBox_group->currentText();
        bool isCommon = ui->checkBox_common->isChecked();
        CoordinateSystem *nominalSystem = NULL;
        QString function = this->ui->comboBox_function->currentText();
        QString plugin = "";
        bool isMovement = ui->checkBox_movement->isChecked();

        QStringList split1 = function.split('[');
        if(split1.size() >= 2){
            QStringList split2 = split1.at(split1.size()-1).split(']');
            if(split2.size() == 2){

                //get function name and plugin name
                function = "";
                for(int i = 0; i < split1.size()-1; i++){
                    function.append(split1.at(i));
                }
                function.truncate(function.size()-1);
                plugin = split2.at(0);

            }
        }

        if(name == ""){
            return;
        }

        if (typeOfFeature != eTrafoParamFeature){

            if(isNominal){

                for(int k=0; k<OiFeatureState::getFeatureCount();k++){
                    if(OiFeatureState::getFeatures().at(k)->getCoordinateSystem() != NULL &&
                            ui->comboBox_nominalSystem->currentText() == OiFeatureState::getFeatures().at(k)->getCoordinateSystem()->getFeatureName()){
                        nominalSystem = OiFeatureState::getFeatures().at(k)->getCoordinateSystem();
                    }
                }
            }

            FeatureAttributes featureAttributes;
            featureAttributes.count = count;
            featureAttributes.featureType = featureType;
            featureAttributes.name = name;
            featureAttributes.group = group;
            featureAttributes.function = function;
            featureAttributes.plugin = plugin;
            featureAttributes.actual = isActual;
            featureAttributes.nominal = isNominal;
            featureAttributes.common = isCommon;
            featureAttributes.nominalSystem = nominalSystem;

            emit createFeature(featureAttributes);

        }else{
            CoordinateSystem *from;
            CoordinateSystem *to;
            //if(ui->comboBox_destinationSystem->currentText() != ui->comboBox_startSystem->currentText()){
            //allow multiple parameter for same direction
            //also parameter from system to itself (neccessary for temperature compensation) !
                for(int i=0;i<OiFeatureState::getFeatureCount();i++){
                    if(OiFeatureState::getFeatures().at(i)->getCoordinateSystem() != NULL &&
                            ui->comboBox_destinationSystem->currentText() == OiFeatureState::getFeatures().at(i)->getCoordinateSystem()->getFeatureName()){
                        to = OiFeatureState::getFeatures().at(i)->getCoordinateSystem();
                    }
                    if(OiFeatureState::getFeatures().at(i)->getStation() != NULL &&
                            ui->comboBox_destinationSystem->currentText() == OiFeatureState::getFeatures().at(i)->getStation()->getFeatureName()){
                        to = OiFeatureState::getFeatures().at(i)->getStation()->coordSys;
                    }
                    if(OiFeatureState::getFeatures().at(i)->getCoordinateSystem() != NULL &&
                            ui->comboBox_startSystem->currentText() == OiFeatureState::getFeatures().at(i)->getCoordinateSystem()->getFeatureName()){
                        from = OiFeatureState::getFeatures().at(i)->getCoordinateSystem();
                    }
                    if(OiFeatureState::getFeatures().at(i)->getStation() != NULL &&
                            ui->comboBox_startSystem->currentText() == OiFeatureState::getFeatures().at(i)->getStation()->getFeatureName()){
                        from = OiFeatureState::getFeatures().at(i)->getStation()->coordSys;
                    }
                }
                if(isMovement){
                    from = to;
                }
                FeatureAttributes featureAttributes;
                featureAttributes.count = count;
                featureAttributes.featureType = featureType;
                featureAttributes.name = name;
                featureAttributes.group = group;
                featureAttributes.function = function;
                featureAttributes.plugin = plugin;
                featureAttributes.actual = isActual;
                featureAttributes.nominal = isNominal;
                featureAttributes.common = isCommon;
                featureAttributes.nominalSystem = nominalSystem;
                featureAttributes.startSystem = from;
                featureAttributes.destSystem = to;
                featureAttributes.isMovement = isMovement;

                emit createFeature(featureAttributes);
                emit trafoParamCreated();
            //}else{
            //    return;
            //}
        }

        this->close();
    }

}


void CreateFeatureDialog::on_toolButton_cancel_clicked()
{
    this->close();
}


void CreateFeatureDialog::initGUI(){

    ui->comboBox_destinationSystem->clear();
    ui->comboBox_startSystem->clear();
    ui->comboBox_nominalSystem->clear();
    ui->checkBox_Nominal->setChecked(false);
    ui->checkBox_Actual->setChecked(true);
    ui->checkBox_common->setChecked(false);
    ui->lineEdit_name->setText("");
    ui->spinBox_count->setValue(1);

    if(OiFeatureState::getFeatures().size() !=0){
        for(int i=0; i<OiFeatureState::getFeatures().size();i++){
            if(OiFeatureState::getFeatures().at(i)->getCoordinateSystem() != NULL){
                ui->comboBox_destinationSystem->addItem(OiFeatureState::getFeatures().at(i)->getCoordinateSystem()->getFeatureName());
                ui->comboBox_startSystem->addItem(OiFeatureState::getFeatures().at(i)->getCoordinateSystem()->getFeatureName());
                ui->comboBox_nominalSystem->addItem(OiFeatureState::getFeatures().at(i)->getCoordinateSystem()->getFeatureName());
            }
            if(OiFeatureState::getFeatures().at(i)->getStation() != NULL){
                ui->comboBox_destinationSystem->addItem(OiFeatureState::getFeatures().at(i)->getStation()->getFeatureName());
                ui->comboBox_startSystem->addItem(OiFeatureState::getFeatures().at(i)->getStation()->getFeatureName());

            }
        }
    }
}


void CreateFeatureDialog::on_checkBox_Nominal_toggled(bool checked)
{
    if(checked){
        ui->label_nominalSystem->setVisible(true);
        ui->comboBox_nominalSystem->setVisible(true);
    }else{
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
    }
}


void CreateFeatureDialog::on_toolButton_mConfig_clicked()
{
    emit createFeatureMConfig(this->typeOfFeature);
}


void CreateFeature::availableGroupsChanged(QStringList myGroups){
    this->ui->comboBox_group->clear();
    this->ui->comboBox_group->clearEditText();
    this->ui->comboBox_group->addItems(myGroups);
}


void CreateFeature::setAvailableFunctions(QStringList functions, QString defaultFunction){
    this->ui->comboBox_function->clear();
    this->ui->comboBox_function->addItems(functions);
    this->ui->comboBox_function->setCurrentText(defaultFunction);
}


void CreateFeatureDialog::showEvent(QShowEvent *event){

    //put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );
    event->accept();

}


void CreateFeatureDialog::on_checkBox_Actual_toggled(bool checked)
{
    if(checked){
        this->ui->comboBox_function->setEnabled(true);
    }else{
        this->ui->comboBox_function->setEnabled(false);
    }
}

void CreateFeatureDialog::on_checkBox_movement_toggled(bool checked)
{
    if(checked){
        ui->comboBox_startSystem->setEnabled(false);
        ui->comboBox_startSystem->clear();
        ui->comboBox_destinationSystem->clear();

        foreach (Station *s, OiFeatureState::getStations()) {
            ui->comboBox_destinationSystem->addItem(s->getFeatureName());
            ui->comboBox_startSystem->addItem(s->getFeatureName());
        }

    }else{
        ui->comboBox_startSystem->setEnabled(true);
        ui->comboBox_startSystem->clear();
        ui->comboBox_destinationSystem->clear();

        foreach (CoordinateSystem *cs, OiFeatureState::getCoordinateSystems()) {
            ui->comboBox_destinationSystem->addItem(cs->getFeatureName());
            ui->comboBox_startSystem->addItem(cs->getFeatureName());
        }

        foreach (Station *s, OiFeatureState::getStations()) {
            ui->comboBox_destinationSystem->addItem(s->getFeatureName());
            ui->comboBox_startSystem->addItem(s->getFeatureName());
        }
    }
}

void CreateFeatureDialog::on_comboBox_destinationSystem_currentIndexChanged(const QString &arg1)
{
    if(ui->checkBox_movement->isChecked()){
        ui->comboBox_startSystem->setCurrentText(arg1);
    }
}*/

/*!
 * \brief CreateFeatureDialog::on_toolButton_cancel_clicked
 */
void CreateFeatureDialog::on_toolButton_cancel_clicked(){
    this->close();
}

/*!
 * \brief CreateFeatureDialog::on_toolButton_create_clicked
 */
void CreateFeatureDialog::on_toolButton_create_clicked(){
    FeatureAttributes attributes;
    this->featureAttributesFromGUI(attributes);
    emit this->addFeatures(attributes);
    this->close();
}

/*!
 * \brief CreateFeatureDialog::on_toolButton_mConfig_clicked
 */
void CreateFeatureDialog::on_toolButton_mConfig_clicked(){

}

/*!
 * \brief CreateFeatureDialog::featureAttributesFromGUI
 * \param attributes
 */
void CreateFeatureDialog::featureAttributesFromGUI(FeatureAttributes &attributes){

    //fill general attributes
    //attributes.typeOfFeature = this->typeOfFeature;
    attributes.count = this->ui->spinBox_count->value();
    attributes.name = this->ui->lineEdit_name->text();
    attributes.group = this->ui->comboBox_group->currentText();

    //fill feature type specific attributes
    if(this->typeOfFeature == eTrafoParamFeature){

        attributes.startSystem = this->ui->comboBox_startSystem->currentText();
        attributes.destinationSystem = this->ui->comboBox_destinationSystem->currentText();
        attributes.isMovement = this->ui->checkBox_isMovement->isChecked();

    }else if(this->typeOfFeature != eCoordinateSystemFeature
             && this->typeOfFeature != eStationFeature){

        attributes.isActual = this->ui->checkBox_isActual->isChecked();
        attributes.isNominal = this->ui->checkBox_isNominal->isChecked();
        attributes.isCommon = this->ui->checkBox_isCommon->isChecked();
        attributes.nominalSystem = this->ui->comboBox_nominalSystem->currentText();


        /*
        attributes.functionPlugin.first = this->ui->comboBox_function
        attributes.functionPlugin.second = this->ui->checkBox_isActual->isChecked();
        */
        //TODO: get function and plugin from model

    }

}
