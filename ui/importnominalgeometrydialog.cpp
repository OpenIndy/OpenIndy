#include "importnominalgeometrydialog.h"
#include "ui_importnominalgeometrydialog.h"

importNominalGeometryDialog::importNominalGeometryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::importNominalGeometryDialog)
{
    ui->setupUi(this);

    initGUI();
}

importNominalGeometryDialog::~importNominalGeometryDialog()
{
    delete ui;
}

void importNominalGeometryDialog::initGUI(){

    ui->comboBox_angleUnit->clear();
    ui->comboBox_distUnit->clear();
    ui->comboBox_tempUnit->clear();
    ui->comboBox_angleUnit->setVisible(true);
    ui->comboBox_distUnit->setVisible(true);
    ui->comboBox_tempUnit->setVisible(true);
    ui->comboBox_format->addItems(oiDataExchanger::getOutputFormats());

    setUpSupportedElements(oiDataExchanger::getOutputFormats().at(0));
    setUpSupportedUnits(oiDataExchanger::getOutputFormats().at(0));
    setUpDescription();

}

void importNominalGeometryDialog::setUpSupportedElements(QString format){

    ui->comboBox_typeOfElement->clear();

    QList<Configuration::ElementTypes> proxyElementTypes = oiDataExchanger::getSupportedElements(format);

    QStringList supportedElements;

    for (int i = 0; i < proxyElementTypes.size();i++){
        supportedElements.append(Configuration::getElementTypeString( proxyElementTypes.at(i)));
    }

    ui->comboBox_typeOfElement->addItems(supportedElements);

}

void importNominalGeometryDialog::setUpSupportedUnits(QString format)
{
    ui->comboBox_angleUnit->clear();
    ui->comboBox_distUnit->clear();
    ui->comboBox_tempUnit->clear();

    QList<UnitConverter::unitType> angleUnit = oiDataExchanger::getAngleUnits(format);
    QList<UnitConverter::unitType> distUnit = oiDataExchanger::getDistanceUnits(format);
    QList<UnitConverter::unitType> tempUnit = oiDataExchanger::getTemperatureUnits(format);

    if(angleUnit.size()>0){
        QStringList a;
        for(int i = 0 ;i<angleUnit.size();i++){
            a.append(UnitConverter::getUnitString(angleUnit.at(i)));
        }
        ui->comboBox_angleUnit->addItems(a);
    }else{
        ui->comboBox_angleUnit->setVisible(false);
    }

    if(distUnit.size()>0){
        QStringList d;
        for(int i = 0 ;i<distUnit.size();i++){
            d.append(UnitConverter::getUnitString(distUnit.at(i)));
        }
        ui->comboBox_distUnit->addItems(d);
    }else{
        ui->comboBox_distUnit->setVisible(false);
    }


    if(tempUnit.size()>0){
        QStringList t;
        for(int i = 0 ;i<tempUnit.size();i++){
            t.append(UnitConverter::getUnitString(tempUnit.at(i)));
        }
        ui->comboBox_tempUnit->addItems(t);
    }else{
        ui->comboBox_tempUnit->setVisible(false);
    }


}

void importNominalGeometryDialog::setUpDescription()
{
    QString typeOfElement = ui->comboBox_typeOfElement->currentText();
    QString format = ui->comboBox_format->currentText();

    QStringList description = oiDataExchanger::getElementDescription(format,Configuration::getElementTypeEnum(typeOfElement));

    QStringListModel *model = new QStringListModel;
    model->setStringList(description);
    ui->listView_description->setModel(model);
}

void importNominalGeometryDialog::on_pushButton_import_clicked()
{
    if(ExchangeData.device == NULL){

        QMessageBox msgBox;
        msgBox.setText("No file selected!");
        msgBox.setInformativeText("Please chose a file.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();

        switch (ret) {
        case QMessageBox::Ok:
            return;
            break;
        default:
            return;
            break;
        }
    }

    QString choosenCoordName = ui->comboBox_coordSys->currentText();


    foreach(CoordinateSystem* c, OiFeatureState::getCoordinateSystems()){
        if (c->getFeatureName() == choosenCoordName){
            ExchangeData.nominalCoordSys = c;
            break;
        }
    }

    QString typeOfElement = ui->comboBox_typeOfElement->currentText();

    ExchangeData.typeOfElement = Configuration::getElementTypeEnum(typeOfElement);

    ExchangeData.unit.clear();

    if(ui->comboBox_distUnit->isVisible()){
        QString distUnit = ui->comboBox_distUnit->currentText();
        ExchangeData.unit.insert(UnitConverter::eMetric,UnitConverter::getUnitType(distUnit));
    }else if(ui->comboBox_angleUnit->isVisible()){
        QString angleUnit = ui->comboBox_angleUnit->currentText();
        ExchangeData.unit.insert(UnitConverter::eAngular,UnitConverter::getUnitType(angleUnit));
    }else if(ui->comboBox_tempUnit->isVisible()){
        QString tempUnit = ui->comboBox_tempUnit->currentText();
        ExchangeData.unit.insert(UnitConverter::eTemperature,UnitConverter::getUnitType(tempUnit));
    }

    QString outputFormat = ui->comboBox_format->currentText();

    oiDataExchanger::importData(ExchangeData,outputFormat);
    //QList<FeatureWrapper*> geom;

    emit sendFeature(ExchangeData.features);
}

void importNominalGeometryDialog::on_toolButton_open_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.*)"));


    ExchangeData.device = new QFile(path);

    ui->lineEdit_path->setText(path);
}



void importNominalGeometryDialog::setUpCoordinateSystems(){

    ui->comboBox_coordSys->clear();

    QList<CoordinateSystem*> availableCoordSys = OiFeatureState::getCoordinateSystems();

    for(int i = 0; i < availableCoordSys.size(); i++){
        ui->comboBox_coordSys->insertItem(i,availableCoordSys.at(i)->getFeatureName());
    }


}

void importNominalGeometryDialog::showEvent(QShowEvent *event)
{
    //Put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );
    setUpCoordinateSystems();
    event->accept();
}

void importNominalGeometryDialog::on_comboBox_format_currentIndexChanged(const QString &arg1)
{

    setUpSupportedElements(arg1);
    setUpSupportedUnits(arg1);

}

void importNominalGeometryDialog::on_comboBox_typeOfElement_currentIndexChanged(const QString &arg1)
{
    this->setUpDescription();
}
