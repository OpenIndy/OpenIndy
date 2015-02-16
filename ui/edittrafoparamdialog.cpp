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
}

/*!
 * \brief EditTrafoparamDialog destructor.
 */
EditTrafoparamDialog::~EditTrafoparamDialog()
{
    delete ui;
}

void EditTrafoparamDialog::showEvent(QShowEvent *event)
{
    //Put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );
    this->initGUI();
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
    /*ui->label_rotationX->setText(QString("rotation x " + OiUnitConverter::getAngleUnitString()));
    ui->label_rotationY->setText(QString("rotation y " + OiUnitConverter::getAngleUnitString()));
    ui->label_rotationZ->setText(QString("rotation z " + OiUnitConverter::getAngleUnitString()));
    ui->label_scaleX->setText(QString("scale x"));
    ui->label_scaleY->setText(QString("scale y"));
    ui->label_scaleZ->setText(QString("scale z"));
    ui->label_translationX->setText(QString("translation x " + OiUnitConverter::getDistanceUnitString()));
    ui->label_translationY->setText(QString("translation y " + OiUnitConverter::getDistanceUnitString()));
    ui->label_translationZ->setText(QString("translation z " + OiUnitConverter::getDistanceUnitString()));

    ui->comboBox_displayedFunction->clear();

    for(int i=0; i<OiFeatureState::getActiveFeature()->getFeature()->getFunctions().size();i++){
        ui->comboBox_displayedFunction->addItem(OiFeatureState::getActiveFeature()->getFeature()->getFunctions().at(i)->getMetaData()->name);
    }

    this->setWindowTitle(QString("edit transformation parameter of: " + OiFeatureState::getActiveFeature()->getFeature()->getFeatureName()));

    ui->lineEdit_translationX->setText(QString::number(OiFeatureState::getActiveFeature()->getTrafoParam()->getTranslation().getAt(0)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits));
    ui->lineEdit_translationY->setText(QString::number(OiFeatureState::getActiveFeature()->getTrafoParam()->getTranslation().getAt(1)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits));
    ui->lineEdit_translationZ->setText(QString::number(OiFeatureState::getActiveFeature()->getTrafoParam()->getTranslation().getAt(2)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits));
    ui->lineEdit_rotationX->setText(QString::number(OiFeatureState::getActiveFeature()->getTrafoParam()->getRotation().getAt(0)*OiUnitConverter::getAngleMultiplier(),'f',OiUnitConverter::angleDigits));
    ui->lineEdit_rotationY->setText(QString::number(OiFeatureState::getActiveFeature()->getTrafoParam()->getRotation().getAt(1)*OiUnitConverter::getAngleMultiplier(),'f',OiUnitConverter::angleDigits));
    ui->lineEdit_rotationZ->setText(QString::number(OiFeatureState::getActiveFeature()->getTrafoParam()->getRotation().getAt(2)*OiUnitConverter::getAngleMultiplier(),'f',OiUnitConverter::angleDigits));
    ui->lineEdit_scaleX->setText(QString::number(OiFeatureState::getActiveFeature()->getTrafoParam()->getScale().getAt(0),'f',6));
    ui->lineEdit_scaleY->setText(QString::number(OiFeatureState::getActiveFeature()->getTrafoParam()->getScale().getAt(1),'f',6));
    ui->lineEdit_scaleZ->setText(QString::number(OiFeatureState::getActiveFeature()->getTrafoParam()->getScale().getAt(2),'f',6));
*/
}

/*!
 * \brief Saves the specified values edited in the line edits.
 */
void EditTrafoparamDialog::getValues()
{/*
    double tx, ty, tz, rx, ry, rz, sx, sy, sz;
    tx = ui->lineEdit_translationX->text().toDouble()/OiUnitConverter::getDistanceMultiplier();
    ty = ui->lineEdit_translationY->text().toDouble()/OiUnitConverter::getDistanceMultiplier();
    tz = ui->lineEdit_translationZ->text().toDouble()/OiUnitConverter::getDistanceMultiplier();
    rx = ui->lineEdit_rotationX->text().toDouble()/OiUnitConverter::getAngleMultiplier();
    ry = ui->lineEdit_rotationY->text().toDouble()/OiUnitConverter::getAngleMultiplier();
    rz = ui->lineEdit_rotationZ->text().toDouble()/OiUnitConverter::getAngleMultiplier();

    if(ui->checkBox_identicalScale->isChecked()){
        sx = ui->lineEdit_scaleX->text().toDouble();
        sy = ui->lineEdit_scaleX->text().toDouble();
        sz = ui->lineEdit_scaleX->text().toDouble();
    }else{
        sx = ui->lineEdit_scaleX->text().toDouble();
        sy = ui->lineEdit_scaleY->text().toDouble();
        sz = ui->lineEdit_scaleZ->text().toDouble();
    }

    OiMat r = this->getRotationMatrix(rx,ry,rz);
    OiMat t = this->getTranslationMatrix(tx,ty,tz);
    OiMat s = this->getScaleMatrix(sx,sy,sz);

    OiFeatureState::getActiveFeature()->getTrafoParam()->setHomogenMatrix(r, t, s);*/
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
        QStringList protocol =  OiJob::getActiveFeature()->getFeature()->getFunctions().at(currentFunction)->getResultProtocol();
        for(int i= 0;i<protocol.size();i++){
            ui->textBrowser_statistic->append(protocol.at(i));
        }

        fModel = new FunctionStatistic();
        fModel->setFunction(NULL);
        ui->tableView_statistic->setModel(fModel);
        fModel->updateModel();

        mModel = new MatrixModel();
        ui->tableView_qxx->setModel(mModel);

        if(OiJob::getActiveFeature()->getFeature()->getFunctions().size()>0){
            if(OiJob::getActiveFeature()->getFeature()->getFunctions().at(currentFunction)->getStatistic().isValid){
                mModel->updateModel(OiJob::getActiveFeature()->getFeature()->getFunctions().at(currentFunction)->getStatistic().qxx);
            }
        }

        if(OiJob::getActiveFeature()->getFeature()->getFunctions().at(currentFunction)->getFeatureOrder().size()>0){
            fModel->setFunction(OiJob::getActiveFeature()->getFeature()->getFunctions().at(currentFunction));
            ui->tableView_statistic->setModel(fModel);
            fModel->updateModel();
        }else{
            fModel->setFunction(NULL);
            ui->tableView_statistic->setModel(fModel);
            fModel->updateModel();
        }

    }
}

OiMat EditTrafoparamDialog::getTranslationMatrix(double tx, double ty, double tz)
{
    OiMat tmpTranslation(4,4);

    tmpTranslation.setAt(0,0,1.0);
    tmpTranslation.setAt(0,1,0.0);
    tmpTranslation.setAt(0,2,0.0);
    tmpTranslation.setAt(0,3,tx);
    tmpTranslation.setAt(1,0,0.0);
    tmpTranslation.setAt(1,1,1.0);
    tmpTranslation.setAt(1,2,0.0);
    tmpTranslation.setAt(1,3,ty);
    tmpTranslation.setAt(2,0,0.0);
    tmpTranslation.setAt(2,1,0.0);
    tmpTranslation.setAt(2,2,1.0);
    tmpTranslation.setAt(2,3,tz);
    tmpTranslation.setAt(3,0,0.0);
    tmpTranslation.setAt(3,1,0.0);
    tmpTranslation.setAt(3,2,0.0);
    tmpTranslation.setAt(3,3,1.0);

    return tmpTranslation;
}

OiMat EditTrafoparamDialog::getScaleMatrix(double sx, double sy, double sz)
{
    OiMat tmpScale(4,4);

    tmpScale.setAt(0,0,sx);
    tmpScale.setAt(0,1,0.0);
    tmpScale.setAt(0,2,0.0);
    tmpScale.setAt(0,3,0.0);
    tmpScale.setAt(1,0,0.0);
    tmpScale.setAt(1,1,sy);
    tmpScale.setAt(1,2,0.0);
    tmpScale.setAt(1,3,0.0);
    tmpScale.setAt(2,0,0.0);
    tmpScale.setAt(2,1,0.0);
    tmpScale.setAt(2,2,sz);
    tmpScale.setAt(2,3,0.0);
    tmpScale.setAt(3,0,0.0);
    tmpScale.setAt(3,1,0.0);
    tmpScale.setAt(3,2,0.0);
    tmpScale.setAt(3,3,1.0);

    return tmpScale;
}

OiMat EditTrafoparamDialog::getRotationMatrix(double rx, double ry, double rz)
{
    OiMat tmpRotation(4,4);

    tmpRotation.setAt(0,0,qCos(ry)*qCos(rz));
    tmpRotation.setAt(0,1,qCos(rx)*qSin(rz)+qSin(rx)*qSin(ry)*qCos(rz));
    tmpRotation.setAt(0,2,qSin(rx)*qSin(rz)-qCos(rx)*qSin(ry)*qCos(rz));
    tmpRotation.setAt(0,3,0.0);
    tmpRotation.setAt(1,0,-qCos(ry)*qSin(rz));
    tmpRotation.setAt(1,1,qCos(rx)*qCos(rz)-qSin(rx)*qSin(ry)*qSin(rz));
    tmpRotation.setAt(1,2,qSin(rx)*qCos(rz)+qCos(rx)*qSin(ry)*qSin(rz));
    tmpRotation.setAt(1,3,0.0);
    tmpRotation.setAt(2,0,qSin(ry));
    tmpRotation.setAt(2,1,-qSin(rx)*qCos(ry));
    tmpRotation.setAt(2,2,qCos(rx)*qCos(ry));
    tmpRotation.setAt(2,3,0.0);
    tmpRotation.setAt(3,0,0.0);
    tmpRotation.setAt(3,1,0.0);
    tmpRotation.setAt(3,2,0.0);
    tmpRotation.setAt(3,3,1.0);

    return tmpRotation;
}
