#include "nominalpropertiesdialog.h"
#include "ui_nominalpropertiesdialog.h"

/*
NominalDataDialog::NominalDataDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::NominalDataDialog)
{
    ui->setupUi(this);
}

NominalDataDialog::~NominalDataDialog()
{
    delete ui;
}

void NominalDataDialog::on_toolButton_ok_clicked()
{
    NominalAttributeExchange nominalValue;
    //nominalValue.nomX = ui->lineEdit_nominalX->text().toDouble()/OiUnitConverter::getDistanceMultiplier();
    //nominalValue.nomY = ui->lineEdit_nominalY->text().toDouble()/OiUnitConverter::getDistanceMultiplier();
    //nominalValue.nomZ = ui->lineEdit_nominalZ->text().toDouble()/OiUnitConverter::getDistanceMultiplier();
    nominalValue.nomI = ui->lineEdit_nominalI->text().toDouble();
    nominalValue.nomJ = ui->lineEdit_nominalJ->text().toDouble();
    nominalValue.nomK = ui->lineEdit_nominalK->text().toDouble();
    //nominalValue.nomR = ui->lineEdit_nominalR->text().toDouble()/OiUnitConverter::getDistanceMultiplier();
    //nominalValue.nomSDE = ui->lineEdit_nominalSDE->text().toDouble()/OiUnitConverter::getDistanceMultiplier();
    //nominalValue.nomSAE = ui->lineEdit_nominalSAE->text().toDouble()/OiUnitConverter::getAngleMultiplier();
    //nominalValue.nomSTE = OiUnitConverter::getReverseTemperature(ui->lineEdit_nominalTemperature->text().toDouble());
    nominalValue.nomSMSE = ui->lineEdit_nominalMeasurementSeries->text().toDouble();

    emit sendNominalValues(nominalValue);

    this->close();
}

void NominalDataDialog::on_toolButton_cancel_clicked()
{
    this->close();
}

void NominalDataDialog::setLabelText()
{
    ui->label_nominalI->setText(QString("nominal I value"));
    ui->label_nominalJ->setText(QString("nominal J value"));
    ui->label_nominalK->setText(QString("nominal K value"));
    ui->label_nominalMeasurementSeries->setText(QString("nominal measurement series"));
    //ui->label_nominalR->setText(QString("nominal radius value " + OiUnitConverter::getDistanceUnitString()));
    //ui->label_nominalSAE->setText(QString("nominal scalar entity angle value " + OiUnitConverter::getAngleUnitString()));
    //ui->label_nominalSDE->setText(QString("nominal scalar distance value " + OiUnitConverter::getDistanceUnitString()));
    //ui->label_nominalTemperature->setText(QString("nominal scalar entity temperature value " + OiUnitConverter::getTemperatureUnitString()));
    //ui->label_nominalX->setText(QString("nominal X value " + OiUnitConverter::getDistanceUnitString()));
    //ui->label_nominalY->setText(QString("nominal Y value " + OiUnitConverter::getDistanceUnitString()));
    //ui->label_nominalZ->setText(QString("nominal Z value " + OiUnitConverter::getDistanceUnitString()));
}

void NominalDataDialog::showEvent(QShowEvent *event)
{
    //Put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );

    ui->label_nominalI->setEnabled(false);
    ui->label_nominalJ->setEnabled(false);
    ui->label_nominalJ->setEnabled(false);
    ui->label_nominalX->setEnabled(false);
    ui->label_nominalY->setEnabled(false);
    ui->label_nominalZ->setEnabled(false);
    ui->label_nominalR->setEnabled(false);
    ui->lineEdit_nominalI->setEnabled(false);
    ui->lineEdit_nominalJ->setEnabled(false);
    ui->lineEdit_nominalJ->setEnabled(false);
    ui->lineEdit_nominalX->setEnabled(false);
    ui->lineEdit_nominalY->setEnabled(false);
    ui->lineEdit_nominalZ->setEnabled(false);
    ui->lineEdit_nominalR->setEnabled(false);
    ui->label_nominalSAE->setEnabled(false);
    ui->label_nominalSDE->setEnabled(false);
    ui->lineEdit_nominalSAE->setEnabled(false);
    ui->lineEdit_nominalSDE->setEnabled(false);
    ui->label_nominalMeasurementSeries->setEnabled(false);
    ui->lineEdit_nominalMeasurementSeries->setEnabled(false);
    ui->lineEdit_nominalTemperature->setEnabled(false);
    ui->label_nominalTemperature->setEnabled(false);

    this->setLabelText();

    this->setWindowTitle(QString("nominal data of " + OiJob::getActiveFeature()->getFeature()->getFeatureName()));

    switch (OiJob::getActiveFeature()->getTypeOfFeature()) {
    case Configuration::ePointFeature:
        ui->label_nominalI->setEnabled(false);
        ui->label_nominalJ->setEnabled(false);
        ui->label_nominalK->setEnabled(false);
        ui->label_nominalX->setEnabled(true);
        ui->label_nominalY->setEnabled(true);
        ui->label_nominalZ->setEnabled(true);
        ui->label_nominalR->setEnabled(false);
        ui->lineEdit_nominalI->setEnabled(false);
        ui->lineEdit_nominalJ->setEnabled(false);
        ui->lineEdit_nominalK->setEnabled(false);
        ui->lineEdit_nominalX->setEnabled(true);
        ui->lineEdit_nominalY->setEnabled(true);
        ui->lineEdit_nominalZ->setEnabled(true);
        ui->lineEdit_nominalR->setEnabled(false);
        ui->label_nominalSAE->setEnabled(false);
        ui->label_nominalSDE->setEnabled(false);
        ui->lineEdit_nominalSAE->setEnabled(false);
        ui->lineEdit_nominalSDE->setEnabled(false);
        ui->label_nominalMeasurementSeries->setEnabled(false);
        ui->lineEdit_nominalMeasurementSeries->setEnabled(false);
        ui->lineEdit_nominalTemperature->setEnabled(false);
        ui->label_nominalTemperature->setEnabled(false);
        break;
    case Configuration::ePlaneFeature:
        ui->label_nominalI->setEnabled(true);
        ui->label_nominalJ->setEnabled(true);
        ui->label_nominalK->setEnabled(true);
        ui->label_nominalX->setEnabled(true);
        ui->label_nominalY->setEnabled(true);
        ui->label_nominalZ->setEnabled(true);
        ui->label_nominalR->setEnabled(false);
        ui->lineEdit_nominalI->setEnabled(true);
        ui->lineEdit_nominalJ->setEnabled(true);
        ui->lineEdit_nominalK->setEnabled(true);
        ui->lineEdit_nominalX->setEnabled(true);
        ui->lineEdit_nominalY->setEnabled(true);
        ui->lineEdit_nominalZ->setEnabled(true);
        ui->lineEdit_nominalR->setEnabled(false);
        ui->label_nominalSAE->setEnabled(false);
        ui->label_nominalSDE->setEnabled(false);
        ui->lineEdit_nominalSAE->setEnabled(false);
        ui->lineEdit_nominalSDE->setEnabled(false);
        ui->label_nominalMeasurementSeries->setEnabled(false);
        ui->lineEdit_nominalMeasurementSeries->setEnabled(false);
        ui->lineEdit_nominalTemperature->setEnabled(false);
        ui->label_nominalTemperature->setEnabled(false);
        break;
    case Configuration::eLineFeature:
        ui->label_nominalI->setEnabled(true);
        ui->label_nominalJ->setEnabled(true);
        ui->label_nominalK->setEnabled(true);
        ui->label_nominalX->setEnabled(true);
        ui->label_nominalY->setEnabled(true);
        ui->label_nominalZ->setEnabled(true);
        ui->label_nominalR->setEnabled(false);
        ui->lineEdit_nominalI->setEnabled(true);
        ui->lineEdit_nominalJ->setEnabled(true);
        ui->lineEdit_nominalK->setEnabled(true);
        ui->lineEdit_nominalX->setEnabled(true);
        ui->lineEdit_nominalY->setEnabled(true);
        ui->lineEdit_nominalZ->setEnabled(true);
        ui->lineEdit_nominalR->setEnabled(false);
        ui->label_nominalSAE->setEnabled(false);
        ui->label_nominalSDE->setEnabled(false);
        ui->lineEdit_nominalSAE->setEnabled(false);
        ui->lineEdit_nominalSDE->setEnabled(false);
        ui->label_nominalMeasurementSeries->setEnabled(false);
        ui->lineEdit_nominalMeasurementSeries->setEnabled(false);
        ui->lineEdit_nominalTemperature->setEnabled(false);
        ui->label_nominalTemperature->setEnabled(false);
        break;
    case Configuration::eSphereFeature:
        ui->label_nominalI->setEnabled(false);
        ui->label_nominalJ->setEnabled(false);
        ui->label_nominalK->setEnabled(false);
        ui->label_nominalX->setEnabled(true);
        ui->label_nominalY->setEnabled(true);
        ui->label_nominalZ->setEnabled(true);
        ui->label_nominalR->setEnabled(true);
        ui->lineEdit_nominalI->setEnabled(false);
        ui->lineEdit_nominalJ->setEnabled(false);
        ui->lineEdit_nominalK->setEnabled(false);
        ui->lineEdit_nominalX->setEnabled(true);
        ui->lineEdit_nominalY->setEnabled(true);
        ui->lineEdit_nominalZ->setEnabled(true);
        ui->lineEdit_nominalR->setEnabled(true);
        ui->label_nominalSAE->setEnabled(false);
        ui->label_nominalSDE->setEnabled(false);
        ui->lineEdit_nominalSAE->setEnabled(false);
        ui->lineEdit_nominalSDE->setEnabled(false);
        ui->label_nominalMeasurementSeries->setEnabled(false);
        ui->lineEdit_nominalMeasurementSeries->setEnabled(false);
        ui->lineEdit_nominalTemperature->setEnabled(false);
        ui->label_nominalTemperature->setEnabled(false);
        break;
    case Configuration::eScalarEntityAngleFeature:
        ui->label_nominalI->setEnabled(false);
        ui->label_nominalJ->setEnabled(false);
        ui->label_nominalK->setEnabled(false);
        ui->label_nominalX->setEnabled(false);
        ui->label_nominalY->setEnabled(false);
        ui->label_nominalZ->setEnabled(false);
        ui->label_nominalR->setEnabled(false);
        ui->lineEdit_nominalI->setEnabled(false);
        ui->lineEdit_nominalJ->setEnabled(false);
        ui->lineEdit_nominalK->setEnabled(false);
        ui->lineEdit_nominalX->setEnabled(false);
        ui->lineEdit_nominalY->setEnabled(false);
        ui->lineEdit_nominalZ->setEnabled(false);
        ui->lineEdit_nominalR->setEnabled(false);
        ui->label_nominalSAE->setEnabled(true);
        ui->label_nominalSDE->setEnabled(false);
        ui->lineEdit_nominalSAE->setEnabled(true);
        ui->lineEdit_nominalSDE->setEnabled(false);
        ui->label_nominalMeasurementSeries->setEnabled(false);
        ui->lineEdit_nominalMeasurementSeries->setEnabled(false);
        ui->lineEdit_nominalTemperature->setEnabled(false);
        ui->label_nominalTemperature->setEnabled(false);
        break;
    case Configuration::eScalarEntityDistanceFeature:
        ui->label_nominalI->setEnabled(false);
        ui->label_nominalJ->setEnabled(false);
        ui->label_nominalK->setEnabled(false);
        ui->label_nominalX->setEnabled(false);
        ui->label_nominalY->setEnabled(false);
        ui->label_nominalZ->setEnabled(false);
        ui->label_nominalR->setEnabled(false);
        ui->lineEdit_nominalI->setEnabled(false);
        ui->lineEdit_nominalJ->setEnabled(false);
        ui->lineEdit_nominalK->setEnabled(false);
        ui->lineEdit_nominalX->setEnabled(false);
        ui->lineEdit_nominalY->setEnabled(false);
        ui->lineEdit_nominalZ->setEnabled(false);
        ui->lineEdit_nominalR->setEnabled(false);
        ui->label_nominalSAE->setEnabled(false);
        ui->label_nominalSDE->setEnabled(true);
        ui->lineEdit_nominalSAE->setEnabled(false);
        ui->lineEdit_nominalSDE->setEnabled(true);
        ui->label_nominalMeasurementSeries->setEnabled(false);
        ui->lineEdit_nominalMeasurementSeries->setEnabled(false);
        ui->lineEdit_nominalTemperature->setEnabled(false);
        ui->label_nominalTemperature->setEnabled(false);
        break;
    case Configuration::eScalarEntityTemperatureFeature:
        ui->label_nominalI->setEnabled(false);
        ui->label_nominalJ->setEnabled(false);
        ui->label_nominalK->setEnabled(false);
        ui->label_nominalX->setEnabled(false);
        ui->label_nominalY->setEnabled(false);
        ui->label_nominalZ->setEnabled(false);
        ui->label_nominalR->setEnabled(false);
        ui->lineEdit_nominalI->setEnabled(false);
        ui->lineEdit_nominalJ->setEnabled(false);
        ui->lineEdit_nominalK->setEnabled(false);
        ui->lineEdit_nominalX->setEnabled(false);
        ui->lineEdit_nominalY->setEnabled(false);
        ui->lineEdit_nominalZ->setEnabled(false);
        ui->lineEdit_nominalR->setEnabled(false);
        ui->label_nominalSAE->setEnabled(false);
        ui->label_nominalSDE->setEnabled(false);
        ui->lineEdit_nominalSAE->setEnabled(false);
        ui->lineEdit_nominalSDE->setEnabled(false);
        ui->label_nominalMeasurementSeries->setEnabled(false);
        ui->lineEdit_nominalMeasurementSeries->setEnabled(false);
        ui->lineEdit_nominalTemperature->setEnabled(true);
        ui->label_nominalTemperature->setEnabled(true);
        break;
    case Configuration::eScalarEntityMeasurementSeriesFeature:
        ui->label_nominalI->setEnabled(false);
        ui->label_nominalJ->setEnabled(false);
        ui->label_nominalK->setEnabled(false);
        ui->label_nominalX->setEnabled(false);
        ui->label_nominalY->setEnabled(false);
        ui->label_nominalZ->setEnabled(false);
        ui->label_nominalR->setEnabled(false);
        ui->lineEdit_nominalI->setEnabled(false);
        ui->lineEdit_nominalJ->setEnabled(false);
        ui->lineEdit_nominalK->setEnabled(false);
        ui->lineEdit_nominalX->setEnabled(false);
        ui->lineEdit_nominalY->setEnabled(false);
        ui->lineEdit_nominalZ->setEnabled(false);
        ui->lineEdit_nominalR->setEnabled(false);
        ui->label_nominalSAE->setEnabled(false);
        ui->label_nominalSDE->setEnabled(false);
        ui->lineEdit_nominalSAE->setEnabled(false);
        ui->lineEdit_nominalSDE->setEnabled(false);
        ui->label_nominalMeasurementSeries->setEnabled(true);
        ui->lineEdit_nominalMeasurementSeries->setEnabled(true);
        ui->lineEdit_nominalTemperature->setEnabled(false);
        ui->label_nominalTemperature->setEnabled(false);
        break;
    default:
        ui->label_nominalI->setEnabled(false);
        ui->label_nominalJ->setEnabled(false);
        ui->label_nominalK->setEnabled(false);
        ui->label_nominalX->setEnabled(false);
        ui->label_nominalY->setEnabled(false);
        ui->label_nominalZ->setEnabled(false);
        ui->label_nominalR->setEnabled(false);
        ui->lineEdit_nominalI->setEnabled(false);
        ui->lineEdit_nominalJ->setEnabled(false);
        ui->lineEdit_nominalK->setEnabled(false);
        ui->lineEdit_nominalX->setEnabled(false);
        ui->lineEdit_nominalY->setEnabled(false);
        ui->lineEdit_nominalZ->setEnabled(false);
        ui->lineEdit_nominalR->setEnabled(false);
        ui->label_nominalSAE->setEnabled(false);
        ui->label_nominalSDE->setEnabled(false);
        ui->lineEdit_nominalSAE->setEnabled(false);
        ui->lineEdit_nominalSDE->setEnabled(false);
        ui->label_nominalMeasurementSeries->setEnabled(false);
        ui->lineEdit_nominalMeasurementSeries->setEnabled(false);
        ui->lineEdit_nominalTemperature->setEnabled(false);
        ui->label_nominalTemperature->setEnabled(false);
        break;
    }

    fillGUI();

    event->accept();
}

void NominalDataDialog::fillGUI(){

    switch (OiJob::getActiveFeature()->getTypeOfFeature()) {
    case Configuration::ePointFeature:
        //ui->lineEdit_nominalX->setText(QString::number(OiFeatureState::getActiveFeature()->getPoint()->xyz.getAt(0)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits));
        //ui->lineEdit_nominalY->setText(QString::number(OiFeatureState::getActiveFeature()->getPoint()->xyz.getAt(1)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits));
        //ui->lineEdit_nominalZ->setText(QString::number(OiFeatureState::getActiveFeature()->getPoint()->xyz.getAt(2)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits));
        ui->lineEdit_nominalI->setText("");
        ui->lineEdit_nominalJ->setText("");
        ui->lineEdit_nominalK->setText("");
        ui->lineEdit_nominalMeasurementSeries->setText("");
        ui->lineEdit_nominalR->setText("");
        ui->lineEdit_nominalSAE->setText("");
        ui->lineEdit_nominalSDE->setText("");
        ui->lineEdit_nominalTemperature->setText("");
        break;
    case Configuration::ePlaneFeature:
        ui->lineEdit_nominalX->setText(QString::number(OiJob::getActiveFeature()->getPlane()->xyz.getAt(0),'f',6));
        ui->lineEdit_nominalY->setText(QString::number(OiJob::getActiveFeature()->getPlane()->xyz.getAt(1),'f',6));
        ui->lineEdit_nominalZ->setText(QString::number(OiJob::getActiveFeature()->getPlane()->xyz.getAt(2),'f',6));
        ui->lineEdit_nominalI->setText(QString::number(OiJob::getActiveFeature()->getPlane()->ijk.getAt(0),'f',6));
        ui->lineEdit_nominalJ->setText(QString::number(OiJob::getActiveFeature()->getPlane()->ijk.getAt(1),'f',6));
        ui->lineEdit_nominalK->setText(QString::number(OiJob::getActiveFeature()->getPlane()->ijk.getAt(2),'f',6));
        ui->lineEdit_nominalMeasurementSeries->setText("");
        ui->lineEdit_nominalR->setText("");
        ui->lineEdit_nominalSAE->setText("");
        ui->lineEdit_nominalSDE->setText("");
        ui->lineEdit_nominalTemperature->setText("");
        break;
    case Configuration::eLineFeature:
        ui->lineEdit_nominalX->setText(QString::number(OiJob::getActiveFeature()->getLine()->xyz.getAt(0),'f',6));
        ui->lineEdit_nominalY->setText(QString::number(OiJob::getActiveFeature()->getLine()->xyz.getAt(1),'f',6));
        ui->lineEdit_nominalZ->setText(QString::number(OiJob::getActiveFeature()->getLine()->xyz.getAt(2),'f',6));
        ui->lineEdit_nominalI->setText(QString::number(OiJob::getActiveFeature()->getLine()->ijk.getAt(0),'f',6));
        ui->lineEdit_nominalJ->setText(QString::number(OiJob::getActiveFeature()->getLine()->ijk.getAt(1),'f',6));
        ui->lineEdit_nominalK->setText(QString::number(OiJob::getActiveFeature()->getLine()->ijk.getAt(2),'f',6));
        ui->lineEdit_nominalMeasurementSeries->setText("");
        ui->lineEdit_nominalR->setText("");
        ui->lineEdit_nominalSAE->setText("");
        ui->lineEdit_nominalSDE->setText("");
        ui->lineEdit_nominalTemperature->setText("");
        break;
    case Configuration::eSphereFeature:
        ui->lineEdit_nominalX->setText(QString::number(OiJob::getActiveFeature()->getSphere()->xyz.getAt(0),'f',6));
        ui->lineEdit_nominalY->setText(QString::number(OiJob::getActiveFeature()->getSphere()->xyz.getAt(1),'f',6));
        ui->lineEdit_nominalZ->setText(QString::number(OiJob::getActiveFeature()->getSphere()->xyz.getAt(2),'f',6));
        ui->lineEdit_nominalR->setText(QString::number(OiJob::getActiveFeature()->getSphere()->radius,'f',6));
        ui->lineEdit_nominalMeasurementSeries->setText("");
        ui->lineEdit_nominalSAE->setText("");
        ui->lineEdit_nominalSDE->setText("");
        ui->lineEdit_nominalTemperature->setText("");
        ui->lineEdit_nominalI->setText("");
        ui->lineEdit_nominalJ->setText("");
        ui->lineEdit_nominalK->setText("");
        break;
    case Configuration::eScalarEntityAngleFeature:
        //ui->lineEdit_nominalSAE->setText(QString::number(OiFeatureState::getActiveFeature()->getScalarEntityAngle()->getAngle()*OiUnitConverter::getAngleMultiplier(),'f',6));
        ui->lineEdit_nominalI->setText("");
        ui->lineEdit_nominalJ->setText("");
        ui->lineEdit_nominalK->setText("");
        ui->lineEdit_nominalMeasurementSeries->setText("");
        ui->lineEdit_nominalR->setText("");
        ui->lineEdit_nominalSDE->setText("");
        ui->lineEdit_nominalTemperature->setText("");
        ui->lineEdit_nominalX->setText("");
        ui->lineEdit_nominalY->setText("");
        ui->lineEdit_nominalZ->setText("");
        break;
    case Configuration::eScalarEntityDistanceFeature:
        //ui->lineEdit_nominalSDE->setText(QString::number(OiFeatureState::getActiveFeature()->getScalarEntityDistance()->getDistance()*OiUnitConverter::getDistanceMultiplier(),'f',6));
        ui->lineEdit_nominalI->setText("");
        ui->lineEdit_nominalJ->setText("");
        ui->lineEdit_nominalK->setText("");
        ui->lineEdit_nominalMeasurementSeries->setText("");
        ui->lineEdit_nominalR->setText("");
        ui->lineEdit_nominalSAE->setText("");
        ui->lineEdit_nominalTemperature->setText("");
        ui->lineEdit_nominalX->setText("");
        ui->lineEdit_nominalY->setText("");
        ui->lineEdit_nominalZ->setText("");
        break;
    case Configuration::eScalarEntityTemperatureFeature:
        ui->lineEdit_nominalI->setText("");
        ui->lineEdit_nominalJ->setText("");
        ui->lineEdit_nominalK->setText("");
        ui->lineEdit_nominalMeasurementSeries->setText("");
        ui->lineEdit_nominalR->setText("");
        ui->lineEdit_nominalSAE->setText("");
        //ui->lineEdit_nominalTemperature->setText(QString::number(OiUnitConverter::getTemperature(OiFeatureState::getActiveFeature()->getScalarEntityTemperature()->getTemperature()),'f',OiUnitConverter::temperatureDigits));
        ui->lineEdit_nominalX->setText("");
        ui->lineEdit_nominalY->setText("");
        ui->lineEdit_nominalZ->setText("");
        ui->lineEdit_nominalSDE->setText("");
        break;
    case Configuration::eScalarEntityMeasurementSeriesFeature:
        ui->lineEdit_nominalI->setText("");
        ui->lineEdit_nominalJ->setText("");
        ui->lineEdit_nominalK->setText("");
        ui->lineEdit_nominalMeasurementSeries->setText(QString::number(OiJob::getActiveFeature()->getScalarEntityMeasurementSeries()->getSeriesValue(),'f',6));
        ui->lineEdit_nominalR->setText("");
        ui->lineEdit_nominalSAE->setText("");
        ui->lineEdit_nominalTemperature->setText("");
        ui->lineEdit_nominalX->setText("");
        ui->lineEdit_nominalY->setText("");
        ui->lineEdit_nominalZ->setText("");
        ui->lineEdit_nominalSDE->setText("");
        break;
    default:
        break;
    }
}
*/

/*!
 * \brief NominalPropertiesDialog::NominalPropertiesDialog
 * \param parent
 */
NominalPropertiesDialog::NominalPropertiesDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::NominalPropertiesDialog)
{
    this->ui->setupUi(this);

    //init GUI elements and assign models
    this->initGUI();
    this->initModels();
}

/*!
 * \brief NominalPropertiesDialog::~NominalPropertiesDialog
 */
NominalPropertiesDialog::~NominalPropertiesDialog(){
    delete this->ui;
}

/*!
 * \brief NominalPropertiesDialog::showEvent
 * \param event
 */
void NominalPropertiesDialog::showEvent(QShowEvent *event){

    //put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );

    event->accept();

}

/*!
 * \brief NominalPropertiesDialog::initGUI
 */
void NominalPropertiesDialog::initGUI(){

}

/*!
 * \brief NominalPropertiesDialog::initModels
 */
void NominalPropertiesDialog::initModels(){

}
