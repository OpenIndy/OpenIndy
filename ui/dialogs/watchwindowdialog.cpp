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

    //set up default settings
    this->getDefaultSettings();

    Qt::WindowFlags flags = Qt::Window | Qt::WindowSystemMenuHint
                                | Qt::WindowMinimizeButtonHint
                                | Qt::WindowMaximizeButtonHint
                                | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);

    oldWindowHeight = 0;
    oldWindowWidth = 0;
    this->lablesRescaled = false;
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

/*!
 * \brief WatchWindowDialog::on_spinBox_decimalDigits_valueChanged
 * \param arg1
 */
void WatchWindowDialog::on_spinBox_decimalDigits_valueChanged(int arg1){
    this->settings.digits = this->ui->spinBox_decimalDigits->value();
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

    this->lablesRescaled = false;

    if(this->ui->checkBox_x->isChecked()){
        this->settings.displayValues.insert("x", this->ui->lineEdit_tolerance_x->text().toDouble());
    }else{
        this->settings.displayValues.remove("x");
    }
}

/*!
 * \brief WatchWindowDialog::on_checkBox_y_clicked
 */
void WatchWindowDialog::on_checkBox_y_clicked(){

    this->lablesRescaled = false;

    if(this->ui->checkBox_y->isChecked()){
        this->settings.displayValues.insert("y", this->ui->lineEdit_tolerance_y->text().toDouble());
    }else{
        this->settings.displayValues.remove("y");
    }
}

/*!
 * \brief WatchWindowDialog::on_checkBox_z_clicked
 */
void WatchWindowDialog::on_checkBox_z_clicked(){

    this->lablesRescaled = false;

    if(this->ui->checkBox_z->isChecked()){
        this->settings.displayValues.insert("z", this->ui->lineEdit_tolerance_z->text().toDouble());
    }else{
        this->settings.displayValues.remove("z");
    }
}

/*!
 * \brief WatchWindowDialog::on_checkBox_d3d_clicked
 */
void WatchWindowDialog::on_checkBox_d3d_clicked(){

    this->lablesRescaled = false;

    if(this->ui->checkBox_d3d->isChecked()){
        this->settings.displayValues.insert("d3D", this->ui->lineEdit_tolerance_d3d->text().toDouble());
    }else{
        this->settings.displayValues.remove("d3D");
    }
}

/*!
 * \brief WatchWindowDialog::on_lineEdit_x_textChanged
 * \param arg1
 */
void WatchWindowDialog::on_lineEdit_tolerance_x_textChanged(const QString &arg1){
    if(this->ui->checkBox_x->isChecked()){
        this->settings.displayValues.insert("x", this->ui->lineEdit_tolerance_x->text().toDouble());
    }
}

/*!
 * \brief WatchWindowDialog::on_lineEdit_y_textChanged
 * \param arg1
 */
void WatchWindowDialog::on_lineEdit_tolerance_y_textChanged(const QString &arg1){
    if(this->ui->checkBox_y->isChecked()){
        this->settings.displayValues.insert("y", this->ui->lineEdit_tolerance_y->text().toDouble());
    }
}

/*!
 * \brief WatchWindowDialog::on_lineEdit_z_textChanged
 * \param arg1
 */
void WatchWindowDialog::on_lineEdit_tolerance_z_textChanged(const QString &arg1){
    if(this->ui->checkBox_z->isChecked()){
        this->settings.displayValues.insert("z", this->ui->lineEdit_tolerance_z->text().toDouble());
    }
}

/*!
 * \brief WatchWindowDialog::on_lineEdit_d3d_textChanged
 * \param arg1
 */
void WatchWindowDialog::on_lineEdit_tolerance_d3d_textChanged(const QString &arg1){
    if(this->ui->checkBox_d3d->isChecked()){
        this->settings.displayValues.insert("d3D", this->ui->lineEdit_tolerance_d3d->text().toDouble());
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

    this->lablesRescaled = false;

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
    featureName->setWordWrap(true);
    QHBoxLayout *featureNameLayout = new QHBoxLayout();
    featureNameLayout->addWidget(featureName);
    featureNameLayout->setStretch(0,1);
    this->masterLayout->addLayout(featureNameLayout);
    this->masterLayout->setStretch(0, 1);
    streamData.insert("name", featureName);

    //x
    QLabel *x = new QLabel();
    x->setFont(f);
    x->setAutoFillBackground(true);
    x->setAlignment(Qt::AlignVCenter);
    x->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    x->setScaledContents(true);
    featureName->setWordWrap(true);
    QHBoxLayout *xLayout = new QHBoxLayout();
    xLayout->addWidget(x);
    xLayout->setStretch(0,1);
    this->masterLayout->addLayout(xLayout);
    this->masterLayout->setStretch(1,1);
    streamData.insert("x", x);

    //y
    QLabel *y = new QLabel();
    y->setFont(f);
    y->setAutoFillBackground(true);
    y->setAlignment(Qt::AlignVCenter);
    y->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    y->setScaledContents(true);
    featureName->setWordWrap(true);
    QHBoxLayout *yLayout = new QHBoxLayout();
    yLayout->addWidget(y);
    yLayout->setStretch(0,1);
    this->masterLayout->addLayout(yLayout);
    //this->masterLayout->addWidget(y);
    this->masterLayout->setStretch(2,1);
    streamData.insert("y", y);

    //z
    QLabel *z = new QLabel();
    z->setFont(f);
    z->setAutoFillBackground(true);
    z->setAlignment(Qt::AlignVCenter);
    z->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    z->setScaledContents(true);
    featureName->setWordWrap(true);
    QHBoxLayout *zLayout = new QHBoxLayout();
    zLayout->addWidget(z);
    zLayout->setStretch(0,1);
    this->masterLayout->addLayout(zLayout);
    //this->masterLayout->addWidget(z);
    this->masterLayout->setStretch(3, 1);
    streamData.insert("z", z);

    //d3D
    QLabel *d3D = new QLabel();
    d3D->setFont(f);
    d3D->setAutoFillBackground(true);
    d3D->setAlignment(Qt::AlignVCenter);
    d3D->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    d3D->setScaledContents(true);
    featureName->setWordWrap(true);
    QHBoxLayout *d3DLayout = new QHBoxLayout();
    d3DLayout->addWidget(d3D);
    d3DLayout->setStretch(0,1);
    masterLayout->addLayout(d3DLayout);
    //this->masterLayout->addWidget(d3D);
    this->masterLayout->setStretch(4, 1);
    streamData.insert("d3D", d3D);

    //assign master layout
    this->ui->pageWatchWindow->setLayout(this->masterLayout);
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
}

/*!
 * \brief WatchWindowDialog::connectJob
 */
void WatchWindowDialog::connectJob(){

}

/*!
 * \brief WatchWindowDialog::setUpCartesianWatchWindow
 * \param reading
 */
void WatchWindowDialog::setUpCartesianWatchWindow(const QVariantMap &reading){

    //init variables
    QString name, value, displayValue;

    //check and get active coordinate system
    QPointer<CoordinateSystem> activeSystem = this->currentJob->getActiveCoordinateSystem();
    if(activeSystem.isNull()){
        return;
    }

    //check the active position (geometry, station, coordinate system
    Position pos;

    if(this->currentJob.isNull() || this->currentJob->getActiveFeature().isNull()){
        return;
    //check if current feature is a solved geometry with position
    }else if(!this->currentJob->getActiveFeature()->getGeometry().isNull() && this->currentJob->getActiveFeature()->getGeometry()->hasPosition()
             && this->currentJob->getActiveFeature()->getGeometry()->getIsSolved()){

        pos = this->currentJob->getActiveFeature()->getGeometry()->getPosition();

    //check if active feature is a coordinate system
    }else if(!this->currentJob->getActiveFeature()->getCoordinateSystem().isNull()){

        pos = this->currentJob->getActiveFeature()->getCoordinateSystem()->getOrigin();

    //check if active feature is a station
    }else if(!this->currentJob->getActiveFeature()->getStation().isNull()){

        pos = this->currentJob->getActiveFeature()->getStation()->getPosition()->getPosition();

    }else{
        return;
    }

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
    name ="<p align=\"center\">" + this->currentJob->getActiveFeature()->getFeature()->getFeatureName() + "</p>";
    displayValue = "<table width=\"100%\"> <tr> <td>" + name + "</td>  </tr> </table>";
    this->streamData["name"]->setText(displayValue);

    //set x
    if(this->settings.displayValues.contains("x")){

        //get display value
        double displayX;
        if(this->settings.reference == 0){
            displayX = trackerXYZ.getAt(0) - pos.getVector().getAt(0);
        }else{
            displayX = pos.getVector().getAt(0) - trackerXYZ.getAt(0);
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
        displayValue = "<table width=\"100%\"> <tr> <td width=\"20%\">" + name + "</td> <td width=\"80%\">" + value + "</td> </tr></table>";
        streamData.value("x")->setText(displayValue);

        numVisibleElements++;
    }

    //set y
    if(this->settings.displayValues.contains("y")){

        //get display value
        double displayY;
        if(this->settings.reference == 0){
            displayY = trackerXYZ.getAt(1) - pos.getVector().getAt(1);
        }else{
            displayY = pos.getVector().getAt(1) - trackerXYZ.getAt(1);
        }
        displayY = convertFromDefault(displayY, ModelManager::getParameterDisplayConfig().getDisplayUnit(eMetric));

        //set color depending on tolerance
        if(qFabs(displayY) >= qFabs(this->settings.displayValues.value("y"))){
            streamData.value("y")->setPalette(Qt::red);
        }else{
            streamData.value("y")->setPalette(Qt::green);
        }

        //format display value
        name = "<p align=\"left\">y</p>";
        value = "<p align=\"right\">" + QString::number(displayY, 'f', this->settings.digits) + "</p>";
        displayValue = "<table width=\"100%\"> <tr> <td width=\"20%\">" + name + "</td> <td width=\"80%\">" + value + "</td> </tr></table>";
        streamData.value("y")->setText(displayValue);

        numVisibleElements++;
    }

    //set z
    if(this->settings.displayValues.contains("z")){

        //get display value
        double displayZ;
        if(this->settings.reference == 0){
            displayZ = trackerXYZ.getAt(2) - pos.getVector().getAt(2);
        }else{
            displayZ = pos.getVector().getAt(2) - trackerXYZ.getAt(2);
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
        displayValue = "<table width=\"100%\"> <tr> <td width=\"20%\">" + name + "</td> <td width=\"80%\">" + value + "</td> </tr></table>";
        streamData.value("z")->setText(displayValue);

        numVisibleElements++;
    }

    //set d3D
    if(this->settings.displayValues.contains("d3D")){

        //get display value
        OiVec d = pos.getVectorH() - trackerXYZ;
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
        displayValue = "<table width=\"100%\"> <tr> <td width=\"20%\">" + name + "</td> <td width=\"80%\">" + value + "</td> </tr></table>";
        streamData.value("d3D")->setText(displayValue);

        numVisibleElements++;
    }

    //set visibility

    //list of visible layouts (0=name 1=x 2=y 3=z 4=d3D
    QStringList visibleLayouts;

    visibleLayouts.append("0");

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

    //set all streching to 0
    for(int all=0; all < 6; all++){
        this->masterLayout->setStretch(all, 0);
    }

    //set all active attributes to same stretch value
    for(int i=0; i < 6; i++){
        if(visibleLayouts.contains(QString::number(i))){
            masterLayout->setStretch(i,1);
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
        this->settings.displayValues.insert("x", this->ui->lineEdit_tolerance_x->text().toDouble());
    }else{
        this->settings.displayValues.remove("x");
    }

    if(this->ui->checkBox_y->isChecked()){
        this->settings.displayValues.insert("y", this->ui->lineEdit_tolerance_y->text().toDouble());
    }else{
        this->settings.displayValues.remove("y");
    }

    if(this->ui->checkBox_z->isChecked()){
        this->settings.displayValues.insert("z", this->ui->lineEdit_tolerance_z->text().toDouble());
    }else{
        this->settings.displayValues.remove("z");
    }

    if(this->ui->checkBox_d3d->isChecked()){
        this->settings.displayValues.insert("d3D", this->ui->lineEdit_tolerance_d3d->text().toDouble());
    }else{
        this->settings.displayValues.remove("d3D");
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

    if(!this->lablesRescaled || oldWindowHeight != this->height() || oldWindowWidth != this->width()){

        oldWindowHeight = this->height();
        oldWindowWidth = this->width();

        //get current fonts
        QFont fName = this->streamData["name"]->font();
        QFont fX = this->streamData["x"]->font();
        QFont fY = this->streamData["y"]->font();
        QFont fZ = this->streamData["z"]->font();
        QFont fd3D = this->streamData["d3D"]->font();
    /*
        this->calcFontSize(fName, "name");
        this->calcFontSize(fX, "x");
        this->calcFontSize(fY, "y");
        this->calcFontSize(fZ, "z");
        this->calcFontSize(fd3D, "d3D");
    */

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

        this->lablesRescaled = true;
    }
}

/*!
 * \brief WatchWindowDialog::calcFontSize
 * \param f
 * \param attribute
 */
void WatchWindowDialog::calcFontSize(QFont f, QString attribute)
{
    const QRect baseRect = this->streamData[attribute]->rect();
    const QString baseText = this->streamData[attribute]->text();
    int fontSizeGuess = qMax(1,f.pixelSize());

    for(;;++fontSizeGuess){
        QFont testFont(f);
        testFont.setPixelSize(fontSizeGuess);
        const QRect fontRect = QFontMetrics(testFont).boundingRect(baseText);
        if(fontRect.height() > baseRect.height() || fontRect.width() > baseRect.width()){
            break;
        }
    }
    for(;fontSizeGuess>1; --fontSizeGuess){
        QFont testFont(f);
        testFont.setPixelSize(fontSizeGuess);
        const QRect fontRect = QFontMetrics(testFont).boundingRect(baseText);
        if(fontRect.height() <= baseRect.height() && fontRect.width() <= baseRect.width()){
            break;
        }
    }
    f.setPixelSize(fontSizeGuess);
    this->streamData[attribute]->setFont(f);
}
