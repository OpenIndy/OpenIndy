#include "watchwindowdialog.h"

#include "ui_watchwindowdialog.h"

/*!
 * \brief WatchWindowDialog::WatchWindowDialog
 * \param parent
 */
WatchWindowDialog::WatchWindowDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::WatchWindowDialog)
{
    ui->setupUi(this);

    //initialize dynamic GUI elements
    this->initGUI();
    this->initModels();

    //set up default settings
    this->getDefaultSettings();

    Qt::WindowFlags flags = Qt::Window | Qt::WindowSystemMenuHint
                                | Qt::WindowMinimizeButtonHint
                                | Qt::WindowMaximizeButtonHint
                                | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);
}

/*!
 * \brief WatchWindowDialog::~WatchWindowDialog
 */
WatchWindowDialog::~WatchWindowDialog(){
    delete this->ui;
}

/*!
 * \brief WatchWindowDialog::getCurrentJob
 * \return
 */
const QPointer<OiJob> &WatchWindowDialog::getCurrentJob() const{
    return this->currentJob;
}

/*!
 * \brief WatchWindowDialog::setCurrentJob
 * \param job
 */
void WatchWindowDialog::setCurrentJob(const QPointer<OiJob> &job){
    if(!job.isNull()){
        this->currentJob = job;
        this->connectJob();
    }
}

void WatchWindowDialog::keyPressEvent(QKeyEvent *e)
{

}

/*!
 * \brief WatchWindowDialog::on_spinBox_decimalDigits_valueChanged
 * \param arg1
 */
void WatchWindowDialog::on_spinBox_decimalDigits_valueChanged(int arg1){
    this->settings.digits = this->ui->spinBox_decimalDigits->value();
}

/*!
 * \brief WatchWindowDialog::on_comboBox_readingTypes_currentIndexChanged
 * \param arg1
 */
void WatchWindowDialog::on_comboBox_readingTypes_currentIndexChanged(const QString &arg1){
    this->settings.readingType = getReadingTypeEnum(this->ui->comboBox_readingTypes->currentText());
    if(this->settings.readingType == ePolarReading){
        this->ui->comboBox_polarMode->setVisible(true);
        this->ui->label_polarMode->setVisible(true);
    }else{
        this->ui->comboBox_polarMode->setVisible(false);
        this->ui->label_polarMode->setVisible(false);
    }
}

/*!
 * \brief WatchWindowDialog::on_comboBox_polarMode_currentIndexChanged
 * \param arg1
 */
void WatchWindowDialog::on_comboBox_polarMode_currentIndexChanged(const QString &arg1){
    if(this->ui->comboBox_polarMode->currentText().compare("cross and distance") == 0){
        this->settings.polarType = 1;
    }else{
        this->settings.polarType = 0;
    }
}

/*!
 * \brief WatchWindowDialog::on_radioButton_actnom_clicked
 */
void WatchWindowDialog::on_radioButton_actnom_clicked(){
    this->settings.reference = this->ui->radioButton_actnom->isChecked()?0:1;
}

/*!
 * \brief WatchWindowDialog::on_radioButton_nomact_clicked
 */
void WatchWindowDialog::on_radioButton_nomact_clicked(){
    this->settings.reference = this->ui->radioButton_actnom->isChecked()?0:1;
}

/*!
 * \brief WatchWindowDialog::on_checkBox_x_clicked
 */
void WatchWindowDialog::on_checkBox_x_clicked(){
    if(this->ui->checkBox_x->isChecked()){
        this->settings.displayValues.insert("x", this->ui->lineEdit_x->text().toDouble());
    }else{
        this->settings.displayValues.remove("x");
    }
}

/*!
 * \brief WatchWindowDialog::on_checkBox_y_clicked
 */
void WatchWindowDialog::on_checkBox_y_clicked(){
    if(this->ui->checkBox_y->isChecked()){
        this->settings.displayValues.insert("y", this->ui->lineEdit_y->text().toDouble());
    }else{
        this->settings.displayValues.remove("y");
    }
}

/*!
 * \brief WatchWindowDialog::on_checkBox_z_clicked
 */
void WatchWindowDialog::on_checkBox_z_clicked(){
    if(this->ui->checkBox_z->isChecked()){
        this->settings.displayValues.insert("z", this->ui->lineEdit_z->text().toDouble());
    }else{
        this->settings.displayValues.remove("z");
    }
}

/*!
 * \brief WatchWindowDialog::on_checkBox_d3d_clicked
 */
void WatchWindowDialog::on_checkBox_d3d_clicked(){
    if(this->ui->checkBox_d3d->isChecked()){
        this->settings.displayValues.insert("d3D", this->ui->lineEdit_d3d->text().toDouble());
    }else{
        this->settings.displayValues.remove("d3D");
    }
}

/*!
 * \brief WatchWindowDialog::on_checkBox_azimuth_clicked
 */
void WatchWindowDialog::on_checkBox_azimuth_clicked(){
    if(this->ui->checkBox_azimuth->isChecked()){
        this->settings.displayValues.insert("azimuth", this->ui->lineEdit_azimuth->text().toDouble());
    }else{
        this->settings.displayValues.remove("azimuth");
    }
}

/*!
 * \brief WatchWindowDialog::on_checkBox_zenith_clicked
 */
void WatchWindowDialog::on_checkBox_zenith_clicked(){
    if(this->ui->checkBox_zenith->isChecked()){
        this->settings.displayValues.insert("zenith", this->ui->lineEdit_zenith->text().toDouble());
    }else{
        this->settings.displayValues.remove("zenith");
    }
}

/*!
 * \brief WatchWindowDialog::on_checkBox_distance_clicked
 */
void WatchWindowDialog::on_checkBox_distance_clicked(){
    if(this->ui->checkBox_distance->isChecked()){
        this->settings.displayValues.insert("distance", this->ui->lineEdit_distance->text().toDouble());
    }else{
        this->settings.displayValues.remove("distance");
    }
}

/*!
 * \brief WatchWindowDialog::on_lineEdit_x_textChanged
 * \param arg1
 */
void WatchWindowDialog::on_lineEdit_x_textChanged(const QString &arg1){
    if(this->ui->checkBox_x->isChecked()){
        this->settings.displayValues.insert("x", this->ui->lineEdit_x->text().toDouble());
    }
}

/*!
 * \brief WatchWindowDialog::on_lineEdit_y_textChanged
 * \param arg1
 */
void WatchWindowDialog::on_lineEdit_y_textChanged(const QString &arg1){
    if(this->ui->checkBox_y->isChecked()){
        this->settings.displayValues.insert("y", this->ui->lineEdit_y->text().toDouble());
    }
}

/*!
 * \brief WatchWindowDialog::on_lineEdit_z_textChanged
 * \param arg1
 */
void WatchWindowDialog::on_lineEdit_z_textChanged(const QString &arg1){
    if(this->ui->checkBox_z->isChecked()){
        this->settings.displayValues.insert("z", this->ui->lineEdit_z->text().toDouble());
    }
}

/*!
 * \brief WatchWindowDialog::on_lineEdit_d3d_textChanged
 * \param arg1
 */
void WatchWindowDialog::on_lineEdit_d3d_textChanged(const QString &arg1){
    if(this->ui->checkBox_d3d->isChecked()){
        this->settings.displayValues.insert("d3D", this->ui->lineEdit_d3d->text().toDouble());
    }
}

/*!
 * \brief WatchWindowDialog::on_lineEdit_azimuth_textChanged
 * \param arg1
 */
void WatchWindowDialog::on_lineEdit_azimuth_textChanged(const QString &arg1){
    if(this->ui->checkBox_azimuth->isChecked()){
        this->settings.displayValues.insert("azimuth", this->ui->lineEdit_azimuth->text().toDouble());
    }
}

/*!
 * \brief WatchWindowDialog::on_lineEdit_zenith_textChanged
 * \param arg1
 */
void WatchWindowDialog::on_lineEdit_zenith_textChanged(const QString &arg1){
    if(this->ui->checkBox_zenith->isChecked()){
        this->settings.displayValues.insert("zenith", this->ui->lineEdit_zenith->text().toDouble());
    }
}

/*!
 * \brief WatchWindowDialog::on_lineEdit_distance_textChanged
 * \param arg1
 */
void WatchWindowDialog::on_lineEdit_distance_textChanged(const QString &arg1){
    if(this->ui->checkBox_distance->isChecked()){
        this->settings.displayValues.insert("distance", this->ui->lineEdit_distance->text().toDouble());
    }
}

/*!
 * \brief WatchWindowDialog::realTimeReading
 * \param reading
 */
void WatchWindowDialog::realTimeReading(const QVariantMap &reading){

    //check current job
    if(this->currentJob.isNull()){
        return;
    }

    //check active station
    if(this->activeStation.isNull() || this->activeStation->getCoordinateSystem().isNull()){
        return;
    }

    //set up watch winddow values
    if(this->settings.readingType == eCartesianReading){
        this->setUpCartesianWatchWindow(reading);
    }

}

/*!
 * \brief WatchWindowDialog::showEvent
 * \param event
 */
void WatchWindowDialog::showEvent(QShowEvent *event){

    //put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );

    //connect sensor
    this->connectSensor();

    //start reading stream
    emit this->startStreaming(this->settings.readingType);

    this->ui->toolBox->setCurrentIndex(0);

    event->accept();

}

/*!
 * \brief WatchWindowDialog::closeEvent
 * \param event
 */
void WatchWindowDialog::closeEvent(QCloseEvent *event){

    //stop reading stream
    if(!this->activeStation.isNull() && this->activeStation->getIsSensorConnected()){
        emit this->stopStreaming();
    }

    event->accept();

}

/*!
 * \brief WatchWindowDialog::initGUI
 */
void WatchWindowDialog::initGUI(){

    //init layout for watch window values
    this->masterLayout = new QVBoxLayout();

    //set initial visibility
    this->ui->comboBox_polarMode->setVisible(false);
    this->ui->label_polarMode->setVisible(false);

    //set polar modes
    this->ui->comboBox_polarMode->addItem("normal mode");
    this->ui->comboBox_polarMode->addItem("cross and distance");

    //###########################
    //set up dynamic watch window
    //###########################

    //create font for watch window
    QFont f;
    f.setFamily("Arial");

    //feature name
    QLabel *featureName = new QLabel();
    featureName->setAlignment(Qt::AlignVCenter);
    featureName->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    featureName->setScaledContents(true);
    QHBoxLayout *featureNameLayout = new QHBoxLayout();
    featureNameLayout->addWidget(featureName);
    featureNameLayout->setStretch(0,1);
    this->masterLayout->addLayout(featureNameLayout);
    this->masterLayout->setStretch(0, 1);
    streamData.insert("name", featureName);
    //this->masterLayout->addWidget(featureName);
    //streamData.insert("name", featureName);

    //x
    QLabel *x = new QLabel();
    x->setFont(f);
    x->setAutoFillBackground(true);
    x->setAlignment(Qt::AlignVCenter);
    x->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    x->setScaledContents(true);
    QHBoxLayout *xLayout = new QHBoxLayout();
    xLayout->addWidget(x);
    xLayout->setStretch(0,1);
    this->masterLayout->addLayout(xLayout);
    this->masterLayout->setStretch(1, 3);
    //this->masterLayout->addWidget(x);
    streamData.insert("x", x);

    //y
    QLabel *y = new QLabel();
    y->setFont(f);
    y->setAutoFillBackground(true);
    y->setAlignment(Qt::AlignVCenter);
    y->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    y->setScaledContents(true);
    QHBoxLayout *yLayout = new QHBoxLayout();
    yLayout->addWidget(y);
    yLayout->setStretch(0,1);
    masterLayout->addLayout(yLayout);
    //this->masterLayout->addWidget(y);
    this->masterLayout->setStretch(2, 3);
    streamData.insert("y", y);

    //z
    QLabel *z = new QLabel();
    z->setFont(f);
    z->setAutoFillBackground(true);
    z->setAlignment(Qt::AlignVCenter);
    z->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    z->setScaledContents(true);
    QHBoxLayout *zLayout = new QHBoxLayout();
    zLayout->addWidget(z);
    zLayout->setStretch(0,1);
    masterLayout->addLayout(zLayout);
    //this->masterLayout->addWidget(z);
    this->masterLayout->setStretch(3, 3);
    streamData.insert("z", z);

    //d3D
    QLabel *d3D = new QLabel();
    d3D->setFont(f);
    d3D->setAutoFillBackground(true);
    d3D->setAlignment(Qt::AlignVCenter);
    d3D->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    d3D->setScaledContents(true);
    QHBoxLayout *d3DLayout = new QHBoxLayout();
    d3DLayout->addWidget(d3D);
    d3DLayout->setStretch(0,1);
    masterLayout->addLayout(d3DLayout);
    //this->masterLayout->addWidget(d3D);
    this->masterLayout->setStretch(4, 3);
    streamData.insert("d3D", d3D);

    //assign master layout
    this->ui->pageWatchWindow->setLayout(this->masterLayout);

}

/*!
 * \brief WatchWindowDialog::initModels
 * Is called each time the active sensor has changed to update settings
 */
void WatchWindowDialog::initModels(){

    //update supported reading types
    this->ui->comboBox_readingTypes->clear();
    if(!this->activeStation.isNull()){
        QList<ReadingTypes> readingTypes = this->activeStation->getSupportedReadingTypes();
        foreach(const ReadingTypes &type, readingTypes){
            this->ui->comboBox_readingTypes->addItem(getReadingTypeName(type));
        }
    }

}

/*!
 * \brief WatchWindowDialog::connectSensor
 * Connects the active sensor so that real time readings are recognized by the watch window
 */
void WatchWindowDialog::connectSensor(){

    //check current job
    if(this->currentJob.isNull()){
        return;
    }

    //get and check the active station
    QPointer<Station> station = this->currentJob->getActiveStation();
    if(station.isNull()){
        return;
    }

    //check and disconnect the old station
    if(!this->activeStation.isNull()){
        QObject::disconnect(this->activeStation, &Station::realTimeReading, this, &WatchWindowDialog::realTimeReading);
    }

    //save and connect active station
    this->activeStation = station;
    QObject::connect(this->activeStation, &Station::realTimeReading, this, &WatchWindowDialog::realTimeReading);

    //update settings based on the new sensor
    this->initModels();

}

/*!
 * \brief WatchWindowDialog::connectJob
 */
void WatchWindowDialog::connectJob(){

    //QObject::connect(this->currentJob, &OiJob::activeStationChanged, this, &WatchWindowDialog::connectSensor, Qt::AutoConnection);

}

/*!
 * \brief WatchWindowDialog::setUpCartesianWatchWindow
 * \param reading
 */
void WatchWindowDialog::setUpCartesianWatchWindow(const QVariantMap &reading){

    //init variables
    QString name, actNom, obs, value, displayValue;

    //check and get active coordinate system
    QPointer<CoordinateSystem> activeSystem = this->currentJob->getActiveCoordinateSystem();
    if(activeSystem.isNull()){
        return;
    }

    //check and get active geometry
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getGeometry().isNull()
            || !this->currentJob->getActiveFeature()->getGeometry()->getIsSolved()
            || !this->currentJob->getActiveFeature()->getGeometry()->hasPosition()){
        return;
    }
    QPointer<Geometry> activeGeometry = this->currentJob->getActiveFeature()->getGeometry();

    //get transformation parameters to transform readings
    OiMat trafo(4,4);
    if(!this->trafoController.getTransformationMatrix(trafo, this->activeStation->getCoordinateSystem(), activeSystem)){
        return;
    }

    //get and transform tracker position
    if(!reading.contains("x") || !reading.contains("y") || !reading.contains("z")){
        return;
    }
    OiVec trackerXYZ(4);
    trackerXYZ.setAt(0, reading.value("x").toDouble());
    trackerXYZ.setAt(1, reading.value("y").toDouble());
    trackerXYZ.setAt(2, reading.value("z").toDouble());
    trackerXYZ.setAt(3, 1.0);
    trackerXYZ = trafo * trackerXYZ;

    //number of visible elements
    int numVisibleElements = 1;

    //set feature name
    name ="<p align=\"center\">" + activeGeometry->getFeatureName() + "</p>";
    actNom = "<p align=\"center\"(>" + QString(activeGeometry->getIsNominal()?"nominal":"actual") + "<)/p>";
    obs = "<p align=\"center\">obs: " + QString::number(activeGeometry->getObservations().size()) + "</p>";
    displayValue = "<table width=\"100%\"> <tr> <td>" + name + "</td> <td>" + actNom + "</td> <td>" + obs + "</td> </tr> </table>";
    this->streamData["name"]->setText(displayValue);

    //set x
    if(this->settings.displayValues.contains("x")){

        //get display value
        double displayX;
        if(this->settings.reference == 0){
            displayX = trackerXYZ.getAt(0) - activeGeometry->getPosition().getVector().getAt(0);
        }else{
            displayX = activeGeometry->getPosition().getVector().getAt(0) - trackerXYZ.getAt(0);
        }
        displayX = convertFromDefault(displayX, ModelManager::getParameterDisplayConfig().getDisplayUnit(eMetric));

        //set color depending on tolerance
        if(qFabs(displayX) >= qFabs(this->settings.displayValues.value("x"))){
            streamData.value("x")->setPalette(Qt::red);
        }else{
            streamData.value("x")->setPalette(Qt::green);
        }

        //format display value
        name = "<p align=\"left\">x</p>";
        value = "<p align=\"right\">" + QString::number(displayX, 'f', this->settings.digits) + "</p>";
        displayValue = "<table width=\"100%\"> <tr> <td width=\"50%\">" + name + "</td> <td width=\"50%\">" + value + "</td> </tr></table>";
        streamData.value("x")->setText(displayValue);

        numVisibleElements++;

    }

    //set y
    if(this->settings.displayValues.contains("y")){

        //get display value
        double displayY;
        if(this->settings.reference == 0){
            displayY = trackerXYZ.getAt(1) - activeGeometry->getPosition().getVector().getAt(1);
        }else{
            displayY = activeGeometry->getPosition().getVector().getAt(1) - trackerXYZ.getAt(1);
        }
        displayY = convertFromDefault(displayY, ModelManager::getParameterDisplayConfig().getDisplayUnit(eMetric));

        //set color depending on tolerance
        if(qFabs(displayY) >= qFabs(this->settings.displayValues.value("x"))){
            streamData.value("y")->setPalette(Qt::red);
        }else{
            streamData.value("y")->setPalette(Qt::green);
        }

        //format display value
        name = "<p align=\"left\">y</p>";
        value = "<p align=\"right\">" + QString::number(displayY, 'f', this->settings.digits) + "</p>";
        displayValue = "<table width=\"100%\"> <tr> <td width=\"50%\">" + name + "</td> <td width=\"50%\">" + value + "</td> </tr></table>";
        streamData.value("y")->setText(displayValue);

        numVisibleElements++;

    }

    //set z
    if(this->settings.displayValues.contains("z")){

        //get display value
        double displayZ;
        if(this->settings.reference == 0){
            displayZ = trackerXYZ.getAt(2) - activeGeometry->getPosition().getVector().getAt(2);
        }else{
            displayZ = activeGeometry->getPosition().getVector().getAt(2) - trackerXYZ.getAt(2);
        }
        displayZ = convertFromDefault(displayZ, ModelManager::getParameterDisplayConfig().getDisplayUnit(eMetric));

        //set color depending on tolerance
        if(qFabs(displayZ) >= qFabs(this->settings.displayValues.value("z"))){
            streamData.value("z")->setPalette(Qt::red);
        }else{
            streamData.value("z")->setPalette(Qt::green);
        }

        //format display value
        name = "<p align=\"left\">z</p>";
        value = "<p align=\"right\">" + QString::number(displayZ, 'f', this->settings.digits) + "</p>";
        displayValue = "<table width=\"100%\"> <tr> <td width=\"50%\">" + name + "</td> <td width=\"50%\">" + value + "</td> </tr></table>";
        streamData.value("z")->setText(displayValue);

        numVisibleElements++;

    }

    //set d3D
    if(this->settings.displayValues.contains("d3D")){

        //get display value
        OiVec d = activeGeometry->getPosition().getVectorH() - trackerXYZ;
        double displayD3D = qSqrt(d.getAt(0)*d.getAt(0)+d.getAt(1)*d.getAt(1)+d.getAt(2)*d.getAt(2));

        displayD3D = convertFromDefault(displayD3D, ModelManager::getParameterDisplayConfig().getDisplayUnit(eMetric));

        //set color depending on tolerance
        if(qFabs(displayD3D) >= qFabs(this->settings.displayValues.value("d3D"))){
            streamData.value("d3D")->setPalette(Qt::red);
        }else{
            streamData.value("d3D")->setPalette(Qt::green);
        }

        //format display value
        name = "<p align=\"left\">d3D</p>";
        value = "<p align=\"right\">" + QString::number(displayD3D, 'f', this->settings.digits) + "</p>";
        displayValue = "<table width=\"100%\"> <tr> <td width=\"50%\">" + name + "</td> <td width=\"50%\">" + value + "</td> </tr></table>";
        streamData.value("d3D")->setText(displayValue);

        numVisibleElements++;

    }

    //set visibility

    //list of visible layouts (0=name 1=x 2=y 3=z 4=d3D
    QStringList visibleLayouts;

    if(this->settings.displayValues.contains("x")){
        this->streamData["x"]->setVisible(true);
        visibleLayouts.append("1");
    }else{
        this->streamData["x"]->setVisible(false);
        visibleLayouts.removeOne("1");
    }
    if(this->settings.displayValues.contains("y")){
        this->streamData["y"]->setVisible(true);
        visibleLayouts.append("2");
    }else{
        this->streamData["y"]->setVisible(false);
        visibleLayouts.removeOne("2");
    }
    if(this->settings.displayValues.contains("z")){
        this->streamData["z"]->setVisible(true);
        visibleLayouts.append("3");
    }else{
        this->streamData["z"]->setVisible(false);
        visibleLayouts.removeOne("3");
    }
    if(this->settings.displayValues.contains("d3D")){
        this->streamData["d3D"]->setVisible(true);
        visibleLayouts.append("4");
    }else{
        this->streamData["d3D"]->setVisible(false);
        visibleLayouts.removeOne("4");
    }

    //stretch name field
    masterLayout->setStretch(0,1);


    for(int j = 1; j < 5; j++){
        masterLayout->setStretch(j,0);
    }

    for(int i = 1; i < 5; i++){

        if(visibleLayouts.contains(QString::number(i))){
            masterLayout->setStretch(i,numVisibleElements);
        }
    }

    //resize labels (maximum font size that is possible)
    this->resizeWatchWindowValues();

}

/*!
 * \brief WatchWindowDialog::getDefaultSettings
 */
void WatchWindowDialog::getDefaultSettings(){

    if(this->ui->checkBox_x->isChecked()){
        this->settings.displayValues.insert("x", this->ui->lineEdit_x->text().toDouble());
    }else{
        this->settings.displayValues.remove("x");
    }

    if(this->ui->checkBox_y->isChecked()){
        this->settings.displayValues.insert("y", this->ui->lineEdit_y->text().toDouble());
    }else{
        this->settings.displayValues.remove("y");
    }

    if(this->ui->checkBox_z->isChecked()){
        this->settings.displayValues.insert("z", this->ui->lineEdit_z->text().toDouble());
    }else{
        this->settings.displayValues.remove("z");
    }

    if(this->ui->checkBox_d3d->isChecked()){
        this->settings.displayValues.insert("d3D", this->ui->lineEdit_d3d->text().toDouble());
    }else{
        this->settings.displayValues.remove("d3D");
    }

    if(this->ui->checkBox_azimuth->isChecked()){
        this->settings.displayValues.insert("azimuth", this->ui->lineEdit_azimuth->text().toDouble());
    }else{
        this->settings.displayValues.remove("azimuth");
    }

    if(this->ui->checkBox_zenith->isChecked()){
        this->settings.displayValues.insert("zenith", this->ui->lineEdit_zenith->text().toDouble());
    }else{
        this->settings.displayValues.remove("zenith");
    }

    if(this->ui->checkBox_distance->isChecked()){
        this->settings.displayValues.insert("distance", this->ui->lineEdit_distance->text().toDouble());
    }else{
        this->settings.displayValues.remove("distance");
    }

}

/*!
 * \brief WatchWindowDialog::resizeWatchWindowValues
 * Resizes the watch window values so that the font is at maximum
 */
void WatchWindowDialog::resizeWatchWindowValues(){

    //init variables
    double w = 0.0, h = 0.0, scale = 0.0;

    this->ui->pageWatchWindow->setLayout(this->masterLayout);

    //show the labels first so the right font is returned
    this->streamData["name"]->show();
    if(this->settings.displayValues.contains("x")){
        this->streamData["x"]->show();
    }
    if(this->settings.displayValues.contains("y")){
        this->streamData["y"]->show();
    }
    if(this->settings.displayValues.contains("z")){
        this->streamData["z"]->show();
    }
    if(this->settings.displayValues.contains("d3D")){
        this->streamData["d3D"]->show();
    }

    //get current fonts
    QFont fName = this->streamData["name"]->font();
    QFont fX = this->streamData["x"]->font();
    QFont fY = this->streamData["y"]->font();
    QFont fZ = this->streamData["z"]->font();
    QFont fd3D = this->streamData["d3D"]->font();

    //calculate new fonts
    h = this->streamData["name"]->height();
    w = this->streamData["name"]->width();
    QFontMetrics fmName(fName);
    scale = h/fmName.height();
    fName.setPointSizeF(fName.pointSizeF()*scale);
    h = this->streamData["x"]->height();
    w = this->streamData["x"]->width();
    QFontMetrics fmX(fX);
    scale = h/fmX.height();
    fX.setPointSizeF(fX.pointSizeF()*scale);
    h = this->streamData["y"]->height();
    w = this->streamData["y"]->width();
    QFontMetrics fmY(fY);
    scale = h/fmY.height();
    fY.setPointSizeF(fY.pointSizeF()*scale);
    h = this->streamData["z"]->height();
    w = this->streamData["z"]->width();
    QFontMetrics fmZ(fZ);
    scale = h/fmZ.height();
    fZ.setPointSizeF(fZ.pointSizeF()*scale);
    h = this->streamData["d3D"]->height();
    w = this->streamData["d3D"]->width();
    QFontMetrics fmd3D(fd3D);
    scale = h/fmd3D.height();
    fd3D.setPointSizeF(fd3D.pointSizeF()*scale);

    //set new fonts
    this->streamData["name"]->setFont(fName);
    this->streamData["x"]->setFont(fX);
    this->streamData["y"]->setFont(fX);
    this->streamData["z"]->setFont(fX);
    this->streamData["d3D"]->setFont(fX);

}
