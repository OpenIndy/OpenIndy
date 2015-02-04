#include "createfeaturedialog.h"
#include "ui_createfeaturedialog.h"

/*!
 * \brief CreateFeature constructor. Initiates the gui.
 * \param parent
 */
CreateFeatureDialog::CreateFeatureDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::CreateFeatureDialog)
{
    ui->setupUi(this);
    initGUI();
}

/*!
 * \brief CreateFeature destructor.
 */
CreateFeatureDialog::~CreateFeatureDialog()
{
    delete ui;
}

/*!
 * \brief CreateFeature::setFeatureType
 * \param typeOfFeature
 */
void CreateFeatureDialog::setFeatureType(Configuration::FeatureTypes typeOfFeature){
    this->typeOfFeature = typeOfFeature;
}

/*!
 * \brief receiveFeatureType enables all gui elements and functions for the selected feature type.
 * Enables or disables some GUI elemts depending from the feature type. The feature type is set by the selected feature in the
 * combobox in the main application or by clicking on a create feature button (e.g. the create point button) in the toolbar of the
 * main application.
 * \param Configuration::FeatureTypes fT
 */
/*void CreateFeature::receiveFeatureType(Configuration::FeatureTypes fT){
    this->typeOfFeature = fT;

    initGUI();

    switch(typeOfFeature){
    case Configuration::eCoordinateSystemFeature:{
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
    case Configuration::eTrafoParamFeature:{
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
    case Configuration::ePlaneFeature:
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
    case Configuration::ePointFeature:
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
    case Configuration::eLineFeature:
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
    case Configuration::eStationFeature:
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
    case Configuration::eSphereFeature:
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
    case Configuration::eCylinderFeature:
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
    case Configuration::eConeFeature:
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
    case Configuration::eEllipsoidFeature:
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
    case Configuration::eHyperboloidFeature:
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
    case Configuration::eParaboloidFeature:
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
    case Configuration::eNurbsFeature:
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
    case Configuration::ePointCloudFeature:
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
    case Configuration::eCircleFeature:
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
}*/

/*!
 * \brief create feature.
 * Saves the values set up in the GUI and emits them to the create feature function in the controller, that crates the feature,
 * adds it to the features list and displays it in the table model.
 */
void CreateFeatureDialog::on_toolButton_create_clicked()
{
    if(ui->spinBox_count->value()>0){

        int count = ui->spinBox_count->value();
        Configuration::FeatureTypes featureType = this->typeOfFeature;
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

        if (typeOfFeature != Configuration::eTrafoParamFeature){

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
/*!
 * \brief closes this window without saving values or creating features.
 */
void CreateFeatureDialog::on_toolButton_cancel_clicked()
{
    this->close();
}

/*!
 * \brief initiates the gui and fills the coordinate system comboboxes with availables coordinate systems.
 */
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

/*!
 * \brief this function is called when the nominal checkboxes gets toggled and generates specific gui elements for each case.
 * \param bool checked
 */
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

/*!
 * \brief calls the measurement configuration window for creating a config.
 * This config will be saved for this feature and all following features. It can be changed via the main application.
 */
void CreateFeatureDialog::on_toolButton_mConfig_clicked()
{
    emit createFeatureMConfig(this->typeOfFeature);
}

/*!
 * \brief CreateFeature::availableGroupsChanged
 * Update group combobox
 * \param myGroups
 */
/*void CreateFeature::availableGroupsChanged(QStringList myGroups){
    this->ui->comboBox_group->clear();
    this->ui->comboBox_group->clearEditText();
    this->ui->comboBox_group->addItems(myGroups);
}*/

/*!
 * \brief CreateFeature::setAvailableFunctions
 * Assign all available create functions to the function combo box and select default value
 * \param functions
 * \param defaultFunction
 */
/*void CreateFeature::setAvailableFunctions(QStringList functions, QString defaultFunction){
    this->ui->comboBox_function->clear();
    this->ui->comboBox_function->addItems(functions);
    this->ui->comboBox_function->setCurrentText(defaultFunction);
}*/

/*!
 * \brief CreateFeature::showEvent
 * \param event
 */
void CreateFeatureDialog::showEvent(QShowEvent *event){

    //put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );
    event->accept();

}

/*!
 * \brief CreateFeature::on_checkBox_Actual_toggled
 * \param checked
 */
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
}
