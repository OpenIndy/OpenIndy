#include "trafoparampropertiesdialog.h"
#include "ui_trafoparampropertiesdialog.h"

/*!
 * \brief TrafoParamPropertiesDialog::TrafoParamPropertiesDialog
 * \param parent
 */
TrafoParamPropertiesDialog::TrafoParamPropertiesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TrafoParamPropertiesDialog)
{
    ui->setupUi(this);
}

/*!
 * \brief TrafoParamPropertiesDialog::~TrafoParamPropertiesDialog
 */
TrafoParamPropertiesDialog::~TrafoParamPropertiesDialog(){
    delete ui;
}

/*!
 * \brief TrafoParamPropertiesDialog::getId
 * \return
 */
const int &TrafoParamPropertiesDialog::getId() const{
    return this->id;
}

/*!
 * \brief TrafoParamPropertiesDialog::setId
 * \param id
 */
void TrafoParamPropertiesDialog::setId(const int &id){
    this->id = id;
}

/*!
 * \brief TrafoParamPropertiesDialog::getName
 * \return
 */
const QString &TrafoParamPropertiesDialog::getName() const{
    return this->name;
}

/*!
 * \brief TrafoParamPropertiesDialog::setName
 * \param name
 */
void TrafoParamPropertiesDialog::setName(const QString &name){
    this->name = name;
}

/*!
 * \brief TrafoParamPropertiesDialog::setCurrentTrafoParam
 * \param id
 * \param name
 */
void TrafoParamPropertiesDialog::setCurrentTrafoParam(const int &id, const QString &name){
    this->id = id;
    this->name = name;
}

/*!
 * \brief TrafoParamPropertiesDialog::getUnknownTrafoParamParameters
 * \return
 */
const QMap<TrafoParamParameters, QString> &TrafoParamPropertiesDialog::getUnknownTrafoParamParameters() const{
    return this->parameters;
}

/*!
 * \brief TrafoParamPropertiesDialog::setUnknownTrafoParamParameters
 * \param parameters
 */
void TrafoParamPropertiesDialog::setUnknownTrafoParamParameters(const QMap<TrafoParamParameters, QString> &parameters){
    this->parameters = parameters;
}

/*!
 * \brief TrafoParamPropertiesDialog::on_toolButton_cancel_clicked
 */
void TrafoParamPropertiesDialog::on_toolButton_cancel_clicked(){
    this->close();
}

/*!
 * \brief TrafoParamPropertiesDialog::on_toolButton_ok_clicked
 */
void TrafoParamPropertiesDialog::on_toolButton_ok_clicked(){

    QMessageBox msgBox;
    msgBox.setText("Editing transformation parameters may result in invalid data!!");
    msgBox.setInformativeText("Do you want to continue?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

    if(ret == QMessageBox::Yes){

        //get a map of unknown parameters
        QMap<TrafoParamParameters, double> parameters;
        this->getParametersFromGUI(parameters);

        emit this->trafoParamParametersChanged(this->id, parameters);
    }

    this->close();
}

/*!
 * \brief TrafoParamPropertiesDialog::showEvent
 * \param event
 */
void TrafoParamPropertiesDialog::showEvent(QShowEvent *event){

    //put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );

    //update GUI with respect to the current feature type
    this->initGUI();

    event->accept();

}

/*!
 * \brief TrafoParamPropertiesDialog::initGUI
 */
void TrafoParamPropertiesDialog::initGUI(){

    //get a list of all available unknown parameters
    QList<TrafoParamParameters> availableUnknownParameters = getAvailableTrafoParamParameters();

    //setdefault inputs
    foreach(const TrafoParamParameters &param, availableUnknownParameters){
        this->setParametersToGUI(param, this->parameters.value(param, "0.0"));
    }

}

/*!
 * \brief TrafoParamPropertiesDialog::initModels
 */
void TrafoParamPropertiesDialog::initModels(){

}

/*!
 * \brief TrafoParamPropertiesDialog::setParametersToGUI
 * \param param
 * \param value
 */
void TrafoParamPropertiesDialog::setParametersToGUI(const TrafoParamParameters &param, const QString &value){

    switch(param){
    case eUnknownTX:
        this->ui->lineEdit_tx->setText(value);
        break;
    case eUnknownTY:
        this->ui->lineEdit_ty->setText(value);
        break;
    case eUnknownTZ:
        this->ui->lineEdit_tz->setText(value);
        break;
    case eUnknownRX:
        this->ui->lineEdit_rx->setText(value);
        break;
    case eUnknownRY:
        this->ui->lineEdit_ry->setText(value);
        break;
    case eUnknownRZ:
        this->ui->lineEdit_rz->setText(value);
        break;
    case eUnknownSX:
        this->ui->lineEdit_sx->setText(value);
        break;
    case eUnknownSY:
        this->ui->lineEdit_sy->setText(value);
        break;
    case eUnknownSZ:
        this->ui->lineEdit_sz->setText(value);
        break;
    }

}

/*!
 * \brief TrafoParamPropertiesDialog::getParametersFromGUI
 * \param parameters
 */
void TrafoParamPropertiesDialog::getParametersFromGUI(QMap<TrafoParamParameters, double> &parameters){


    //get a list of all unknown parameters
    QList<TrafoParamParameters> unknownParameters = getAvailableTrafoParamParameters();

    //get parameter display config
    const ParameterDisplayConfig &dConfig = ModelManager::getParameterDisplayConfig();

    foreach(const TrafoParamParameters &param, unknownParameters){

        //get values from GUI elements (only use values that differ from the initial input)
        switch(param){
        case eUnknownTX:
            if(this->parameters.contains(param) && this->parameters.value(param).compare(this->ui->lineEdit_tx->text()) == 0){
                break;
            }
            parameters.insert(param, convertToDefault(this->ui->lineEdit_tx->text().toDouble(), dConfig.getDisplayUnit(eMetric)));
            break;
        case eUnknownTY:
            if(this->parameters.contains(param) && this->parameters.value(param).compare(this->ui->lineEdit_ty->text()) == 0){
                break;
            }
            parameters.insert(param, convertToDefault(this->ui->lineEdit_ty->text().toDouble(), dConfig.getDisplayUnit(eMetric)));
            break;
        case eUnknownTZ:
            if(this->parameters.contains(param) && this->parameters.value(param).compare(this->ui->lineEdit_tz->text()) == 0){
                break;
            }
            parameters.insert(param, convertToDefault(this->ui->lineEdit_tz->text().toDouble(), dConfig.getDisplayUnit(eMetric)));
            break;
        case eUnknownRX:
            if(this->parameters.contains(param) && this->parameters.value(param).compare(this->ui->lineEdit_rx->text()) == 0){
                break;
            }
            parameters.insert(param, convertToDefault(this->ui->lineEdit_rx->text().toDouble(), dConfig.getDisplayUnit(eAngular)));
            break;
        case eUnknownRY:
            if(this->parameters.contains(param) && this->parameters.value(param).compare(this->ui->lineEdit_ry->text()) == 0){
                break;
            }
            parameters.insert(param, convertToDefault(this->ui->lineEdit_ry->text().toDouble(), dConfig.getDisplayUnit(eAngular)));
            break;
        case eUnknownRZ:
            if(this->parameters.contains(param) && this->parameters.value(param).compare(this->ui->lineEdit_rz->text()) == 0){
                break;
            }
            parameters.insert(param, convertToDefault(this->ui->lineEdit_rz->text().toDouble(), dConfig.getDisplayUnit(eAngular)));
            break;
        case eUnknownSX:
            if(this->parameters.contains(param) && this->parameters.value(param).compare(this->ui->lineEdit_sx->text()) == 0){
                break;
            }
            parameters.insert(param, this->ui->lineEdit_sx->text().toDouble());
            break;
        case eUnknownSY:
            if(this->parameters.contains(param) && this->parameters.value(param).compare(this->ui->lineEdit_sy->text()) == 0){
                break;
            }
            parameters.insert(param, this->ui->lineEdit_sy->text().toDouble());
            break;
        case eUnknownSZ:
            if(this->parameters.contains(param) && this->parameters.value(param).compare(this->ui->lineEdit_sz->text()) == 0){
                break;
            }
            parameters.insert(param, this->ui->lineEdit_sz->text().toDouble());
            break;
        }

    }

}
