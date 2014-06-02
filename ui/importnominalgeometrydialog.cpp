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

    ui->comboBox_format->addItems(oiDataExchanger::getOutputFormats());
    ui->comboBox_format->addItem("test");

    setUpSupportedElements(oiDataExchanger::getOutputFormats().at(0));

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

void importNominalGeometryDialog::on_pushButton_import_clicked()
{

    QString choosenCoordName = ui->comboBox_coordSys->currentText();


    foreach(CoordinateSystem* c, OiFeatureState::getCoordinateSystems()){
        if (c->getFeatureName() == choosenCoordName){
            ExchangeData.nominalCoordSys = c;
            break;
        }
    }

    QString typeOfElement = ui->comboBox_typeOfElement->currentText();

    ExchangeData.typeOfElement = Configuration::getElementTypeEnum(typeOfElement);

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



void importNominalGeometryDialog::setUpCoordinateSystems(QList<CoordinateSystem*> coordSystems){

    /*ui->comboBox_coordSys->clear();

    availableCoordSys = coordSystems;

    for(int i = 0; i < availableCoordSys.size(); i++){
        ui->comboBox_coordSys->insertItem(i,availableCoordSys.at(i)->name);
    }*/


}

void importNominalGeometryDialog::showEvent(QShowEvent *event)
{
    //Put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );
    event->accept();
}

void importNominalGeometryDialog::on_comboBox_format_currentIndexChanged(const QString &arg1)
{

    setUpSupportedElements(arg1);

}
