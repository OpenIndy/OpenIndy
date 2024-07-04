#include "movesensordialog.h"
#include "ui_movesensordialog.h"

/*!
 * \brief MoveSensorDialog::MoveSensorDialog
 * \param parent
 */
MoveSensorDialog::MoveSensorDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::MoveSensorDialog)
{
    ui->setupUi(this);

    //set up regular expressions
    this->validDouble = QRegExp("^-?(0|[1-9]{1}[0-9]{0,9})(\\.[0-9]+)?$");
    this->doubleValidator = new QRegExpValidator(this->validDouble);

    this->initModels();
    this->initGUI();
}

/*!
 * \brief MoveSensorDialog::~MoveSensorDialog
 */
MoveSensorDialog::~MoveSensorDialog(){
    delete ui;
}

/*!
 * \brief MoveSensorDialog::initGUI
 */
void MoveSensorDialog::initGUI(){

    //assign validators
    this->ui->lineEdit_azimuth->setValidator(this->doubleValidator);
    this->ui->lineEdit_zenith->setValidator(this->doubleValidator);
    this->ui->lineEdit_distance->setValidator(this->doubleValidator);
    this->ui->lineEdit_x->setValidator(this->doubleValidator);
    this->ui->lineEdit_y->setValidator(this->doubleValidator);
    this->ui->lineEdit_z->setValidator(this->doubleValidator);
}

/*!
 * \brief MoveSensorDialog::initModels
 */
void MoveSensorDialog::initModels(){

    //set reading types model
    this->ui->comboBox_typeOfReading->setModel(&ModelManager::getReadingTypeNamesModel());

}

void MoveSensorDialog::showEvent(QShowEvent *event){

    if(this->ui->comboBox_typeOfReading->currentIndex() < 0) {
        this->ui->comboBox_typeOfReading->setCurrentText(getReadingTypeName(eCartesianReading));
    }

    event->accept();

}
/*!
 * \brief MoveSensorDialog::on_pushButton_move_clicked
 */
void MoveSensorDialog::on_pushButton_move_clicked(){

    //set up reading from GUI elements
    ReadingTypes readingType = getReadingTypeEnum(this->ui->comboBox_typeOfReading->currentText());
    switch(readingType){
    case eDistanceReading:{
        break;
    }case eCartesianReading:{

        ReadingCartesian rCartesian;
        rCartesian.xyz.setAt(0, this->ui->lineEdit_x->text().toDouble());
        rCartesian.xyz.setAt(1, this->ui->lineEdit_y->text().toDouble());
        rCartesian.xyz.setAt(2, this->ui->lineEdit_z->text().toDouble());

        Reading reading(rCartesian);

        emit this->moveSensor(reading);

        break;

    }case ePolarReading:{

        ReadingPolar rPolar;
        rPolar.azimuth = this->ui->lineEdit_azimuth->text().toDouble();
        rPolar.zenith = this->ui->lineEdit_zenith->text().toDouble();
        rPolar.distance = this->ui->lineEdit_distance->text().toDouble();

        Reading reading(rPolar);

        emit this->moveSensor(reading);

        break;

    }case eDirectionReading:{
        break;
    }case eTemperatureReading:{
        break;
    }case eLevelReading:{
        break;
    }case eUndefinedReading:{
        break;
    }
    }

    this->close();

}

/*!
 * \brief MoveSensorDialog::on_comboBox_typeOfReading_currentIndexChanged
 * Depending on the selected reading type set visibility of GUI elements
 * \param arg1
 */
void MoveSensorDialog::on_comboBox_typeOfReading_currentIndexChanged(const QString &arg1){

    //get selected reading type
    ReadingTypes readingType = getReadingTypeEnum(arg1);

    //set visibility depending on reading type
    switch(readingType){
    case eDistanceReading:

        //set visibility
        this->ui->lineEdit_azimuth->setVisible(false);
        this->ui->lineEdit_zenith->setVisible(false);
        this->ui->lineEdit_distance->setVisible(true);
        this->ui->lineEdit_x->setVisible(false);
        this->ui->lineEdit_y->setVisible(false);
        this->ui->lineEdit_z->setVisible(false);
        this->ui->label_azimuth->setVisible(false);
        this->ui->label_zenith->setVisible(false);
        this->ui->label_distance->setVisible(true);
        this->ui->label_x->setVisible(false);
        this->ui->label_y->setVisible(false);
        this->ui->label_z->setVisible(false);

        this->ui->pushButton_move->setEnabled(true);

        break;

    case eCartesianReading:

        //set visibility
        this->ui->lineEdit_azimuth->setVisible(false);
        this->ui->lineEdit_zenith->setVisible(false);
        this->ui->lineEdit_distance->setVisible(false);
        this->ui->lineEdit_x->setVisible(true);
        this->ui->lineEdit_y->setVisible(true);
        this->ui->lineEdit_z->setVisible(true);
        this->ui->label_azimuth->setVisible(false);
        this->ui->label_zenith->setVisible(false);
        this->ui->label_distance->setVisible(false);
        this->ui->label_x->setVisible(true);
        this->ui->label_y->setVisible(true);
        this->ui->label_z->setVisible(true);

        this->ui->pushButton_move->setEnabled(true);

        break;

    case ePolarReading:

        //set visibility
        this->ui->lineEdit_azimuth->setVisible(true);
        this->ui->lineEdit_zenith->setVisible(true);
        this->ui->lineEdit_distance->setVisible(true);
        this->ui->lineEdit_x->setVisible(false);
        this->ui->lineEdit_y->setVisible(false);
        this->ui->lineEdit_z->setVisible(false);
        this->ui->label_azimuth->setVisible(true);
        this->ui->label_zenith->setVisible(true);
        this->ui->label_distance->setVisible(true);
        this->ui->label_x->setVisible(false);
        this->ui->label_y->setVisible(false);
        this->ui->label_z->setVisible(false);

        this->ui->pushButton_move->setEnabled(true);

        break;

    case eDirectionReading:     // not implemented, fall through
    case eTemperatureReading:   // not implemented, fall through
    case eLevelReading:         // not implemented, fall through
    case eCartesianReading6D:   // not implemented, fall through
    case eUndefinedReading:     // fall through
    default:                    // disable all
        //set visibility
        this->ui->lineEdit_azimuth->setVisible(false);
        this->ui->lineEdit_zenith->setVisible(false);
        this->ui->lineEdit_distance->setVisible(false);
        this->ui->lineEdit_x->setVisible(false);
        this->ui->lineEdit_y->setVisible(false);
        this->ui->lineEdit_z->setVisible(false);
        this->ui->label_azimuth->setVisible(false);
        this->ui->label_zenith->setVisible(false);
        this->ui->label_distance->setVisible(false);
        this->ui->label_x->setVisible(false);
        this->ui->label_y->setVisible(false);
        this->ui->label_z->setVisible(false);

        this->ui->pushButton_move->setEnabled(false);

        break;

    }

}
