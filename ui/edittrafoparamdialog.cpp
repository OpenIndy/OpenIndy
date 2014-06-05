#include "edittrafoparamdialog.h"
#include "ui_edittrafoparamdialog.h"

/*!
 * \brief EditTrafoparamDialog constructor
 * \param parent
 */
EditTrafoparamDialog::EditTrafoparamDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditTrafoparamDialog)
{
    ui->setupUi(this);
    this->selectedTrafoParam = NULL;
}

/*!
 * \brief EditTrafoparamDialog destructor.
 */
EditTrafoparamDialog::~EditTrafoparamDialog()
{
    delete ui;
}

/*!
 * \brief This slot receives the selected transformation parameter feature and initiates the gui elements.
 * \param FeatureWrapper fw
 */
void EditTrafoparamDialog::getSelectedTrafoParam(FeatureWrapper *fw)
{
    if(fw != NULL && fw->getTrafoParam() != NULL){
        this->selectedTrafoParam = fw;
        this->initGUI();
    }
}

void EditTrafoparamDialog::showEvent(QShowEvent *event)
{
    //Put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );
    event->accept();
}

/*!
 * \brief closes the window without saving or changing values.
 */
void EditTrafoparamDialog::on_toolButton_cancel_clicked()
{
    this->close();
}

/*!
 * \brief closes the window and saves the changes to the selected transformation parameter feature.
 */
void EditTrafoparamDialog::on_toolButton_ok_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("Set this parameters for the transformation parameter object.");
    msgBox.setInformativeText("Want to continue?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();

    switch (ret) {
    case QMessageBox::Ok:
        this->getValues();

        this->close();
        break;
    case QMessageBox::Cancel:
        return;
        break;
    default:
        break;
    }
}

/*!
 * \brief Initiates the gui elements and changes the label texts to the specified settings.
 * The UnitConverter class specifies the units for the labels and the multipliers for all the values displayed in this
 * window. Also number of digits are specified by the UnitConverter.
 */
void EditTrafoparamDialog::initGUI()
{
    ui->label_rotationX->setText(QString("rotation x " + UnitConverter::getAngleUnitString()));
    ui->label_rotationY->setText(QString("rotation y " + UnitConverter::getAngleUnitString()));
    ui->label_rotationZ->setText(QString("rotation z " + UnitConverter::getAngleUnitString()));
    ui->label_scaleX->setText(QString("scale x"));
    ui->label_scaleY->setText(QString("scale y"));
    ui->label_scaleZ->setText(QString("scale z"));
    ui->label_translationX->setText(QString("translation x " + UnitConverter::getDistanceUnitString()));
    ui->label_translationY->setText(QString("translation y " + UnitConverter::getDistanceUnitString()));
    ui->label_translationZ->setText(QString("translation z " + UnitConverter::getDistanceUnitString()));

    ui->comboBox_displayedFunction->clear();

    for(int i=0; i<this->selectedTrafoParam->getFeature()->getFunctions().size();i++){
        ui->comboBox_displayedFunction->addItem(this->selectedTrafoParam->getFeature()->getFunctions().at(i)->getMetaData()->name);
    }

    this->setWindowTitle(QString("edit transformation parameter of: " + this->selectedTrafoParam->getFeature()->getFeatureName()));

    ui->lineEdit_translationX->setText(QString::number(this->selectedTrafoParam->getTrafoParam()->getTranslation().getAt(0)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits));
    ui->lineEdit_translationY->setText(QString::number(this->selectedTrafoParam->getTrafoParam()->getTranslation().getAt(1)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits));
    ui->lineEdit_translationZ->setText(QString::number(this->selectedTrafoParam->getTrafoParam()->getTranslation().getAt(2)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits));
    ui->lineEdit_rotationX->setText(QString::number(this->selectedTrafoParam->getTrafoParam()->getRotation().getAt(0)*UnitConverter::getAngleMultiplier(),'f',UnitConverter::angleDigits));
    ui->lineEdit_rotationY->setText(QString::number(this->selectedTrafoParam->getTrafoParam()->getRotation().getAt(1)*UnitConverter::getAngleMultiplier(),'f',UnitConverter::angleDigits));
    ui->lineEdit_rotationZ->setText(QString::number(this->selectedTrafoParam->getTrafoParam()->getRotation().getAt(2)*UnitConverter::getAngleMultiplier(),'f',UnitConverter::angleDigits));
    ui->lineEdit_scaleX->setText(QString::number(this->selectedTrafoParam->getTrafoParam()->getScale().getAt(0),'f',6));
    ui->lineEdit_scaleY->setText(QString::number(this->selectedTrafoParam->getTrafoParam()->getScale().getAt(1),'f',6));
    ui->lineEdit_scaleZ->setText(QString::number(this->selectedTrafoParam->getTrafoParam()->getScale().getAt(2),'f',6));

}

/*!
 * \brief Saves the specified values edited in the line edits.
 */
void EditTrafoparamDialog::getValues()
{
    this->selectedTrafoParam->getTrafoParam()->getTranslation().setAt(0,ui->lineEdit_translationX->text().toDouble()/UnitConverter::getDistanceMultiplier());
    this->selectedTrafoParam->getTrafoParam()->getTranslation().setAt(1,ui->lineEdit_translationY->text().toDouble()/UnitConverter::getDistanceMultiplier());
    this->selectedTrafoParam->getTrafoParam()->getTranslation().setAt(2,ui->lineEdit_translationZ->text().toDouble()/UnitConverter::getDistanceMultiplier());
    this->selectedTrafoParam->getTrafoParam()->getRotation().setAt(0,ui->lineEdit_rotationX->text().toDouble()/UnitConverter::getAngleMultiplier());
    this->selectedTrafoParam->getTrafoParam()->getRotation().setAt(1,ui->lineEdit_rotationY->text().toDouble()/UnitConverter::getAngleMultiplier());
    this->selectedTrafoParam->getTrafoParam()->getRotation().setAt(2,ui->lineEdit_rotationZ->text().toDouble()/UnitConverter::getAngleMultiplier());

    if(ui->checkBox_identicalScale->isChecked()){
        this->selectedTrafoParam->getTrafoParam()->getScale().setAt(0,ui->lineEdit_scaleX->text().toDouble());
        this->selectedTrafoParam->getTrafoParam()->getScale().setAt(1,ui->lineEdit_scaleX->text().toDouble());
        this->selectedTrafoParam->getTrafoParam()->getScale().setAt(2,ui->lineEdit_scaleX->text().toDouble());
    }else{
        this->selectedTrafoParam->getTrafoParam()->getScale().setAt(0,ui->lineEdit_scaleX->text().toDouble());
        this->selectedTrafoParam->getTrafoParam()->getScale().setAt(1,ui->lineEdit_scaleY->text().toDouble());
        this->selectedTrafoParam->getTrafoParam()->getScale().setAt(2,ui->lineEdit_scaleZ->text().toDouble());
    }

    this->selectedTrafoParam->getTrafoParam()->generateHomogenMatrix();
}

/*!
 * \brief Specifies 1 or 3 scales and handles the specific saving of the values.
 * \param bool checked
 */
void EditTrafoparamDialog::on_checkBox_identicalScale_toggled(bool checked)
{
    if(checked){
        ui->lineEdit_scaleY->setEnabled(false);
        ui->lineEdit_scaleZ->setEnabled(false);
    }else{
        ui->lineEdit_scaleY->setEnabled(true);
        ui->lineEdit_scaleZ->setEnabled(true);
    }
}

/*!
 * \brief sets the function and matrix model to the view specific for the selected function.
 * \param QString arg1
 */
void EditTrafoparamDialog::on_comboBox_displayedFunction_currentIndexChanged(const QString &arg1)
{
    int currentFunction = ui->comboBox_displayedFunction->currentIndex();

    if(currentFunction != -1){

        ui->textBrowser_statistic->clear();
        QStringList protocol =  this->selectedTrafoParam->getFeature()->getFunctions().at(currentFunction)->getResultProtocol();
        for(int i= 0;i<protocol.size();i++){
            ui->textBrowser_statistic->append(protocol.at(i));
        }

        fModel = new FunctionStatistic();
        fModel->setFunction(NULL);
        ui->tableView_statistic->setModel(fModel);
        fModel->updateModel();

        mModel = new MatrixModel();
        ui->tableView_qxx->setModel(mModel);

        if(this->selectedTrafoParam->getFeature()->getFunctions().size()>0){
            if(this->selectedTrafoParam->getFeature()->getFunctions().at(currentFunction)->getStatistic().isValid){
                mModel->updateModel(this->selectedTrafoParam->getFeature()->getFunctions().at(currentFunction)->getStatistic().qxx);
            }
        }

        if(this->selectedTrafoParam->getFeature()->getFunctions().at(currentFunction)->getFeatureOrder().size()>0){
            fModel->setFunction(this->selectedTrafoParam->getFeature()->getFunctions().at(currentFunction));
            ui->tableView_statistic->setModel(fModel);
            fModel->updateModel();
        }else{
            fModel->setFunction(NULL);
            ui->tableView_statistic->setModel(fModel);
            fModel->updateModel();
        }

    }
}
