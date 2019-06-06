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

    //set values to 0 and false => on first view everything has to be resized
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
    this->settings.reference = this->ui->radioButton_actnom->isChecked()?eActualNominal:eNominalActual;
}

/*!
 * \brief WatchWindowDialog::on_radioButton_nomact_clicked
 */
void WatchWindowDialog::on_radioButton_nomact_clicked(){
    this->settings.reference = this->ui->radioButton_actnom->isChecked()?eActualNominal:eNominalActual;
}

/*!
 * \brief WatchWindowDialog::on_checkBox_x_clicked
 */
void WatchWindowDialog::on_checkBox_x_clicked(){

    //necessary to rescale all
    this->lablesRescaled = false;

    if(this->ui->checkBox_x->isChecked()){
        this->settings.displayValues.insert(eX, this->ui->lineEdit_tolerance_x->text().toDouble());
    }else{
        this->settings.displayValues.remove(eX);
    }
}

/*!
 * \brief WatchWindowDialog::on_checkBox_y_clicked
 */
void WatchWindowDialog::on_checkBox_y_clicked(){

    //necessary to rescale all
    this->lablesRescaled = false;

    if(this->ui->checkBox_y->isChecked()){
        this->settings.displayValues.insert(eY, this->ui->lineEdit_tolerance_y->text().toDouble());
    }else{
        this->settings.displayValues.remove(eY);
    }
}

/*!
 * \brief WatchWindowDialog::on_checkBox_z_clicked
 */
void WatchWindowDialog::on_checkBox_z_clicked(){

    //necessary to rescale all
    this->lablesRescaled = false;

    if(this->ui->checkBox_z->isChecked()){
        this->settings.displayValues.insert(eZ, this->ui->lineEdit_tolerance_z->text().toDouble());
    }else{
        this->settings.displayValues.remove(eZ);
    }
}

/*!
 * \brief WatchWindowDialog::on_checkBox_d3d_clicked
 */
void WatchWindowDialog::on_checkBox_d3d_clicked(){

    //necessary to rescale all
    this->lablesRescaled = false;

    if(this->ui->checkBox_d3d->isChecked()){
        this->settings.displayValues.insert(eD3D, this->ui->lineEdit_tolerance_d3d->text().toDouble());
    }else{
        this->settings.displayValues.remove(eD3D);
    }
}

/*!
 * \brief WatchWindowDialog::on_lineEdit_x_textChanged
 * \param arg1
 */
void WatchWindowDialog::on_lineEdit_tolerance_x_textChanged(const QString &arg1){
    if(this->ui->checkBox_x->isChecked()){
        this->settings.displayValues.insert(eX, this->ui->lineEdit_tolerance_x->text().toDouble());
    }
}

/*!
 * \brief WatchWindowDialog::on_lineEdit_y_textChanged
 * \param arg1
 */
void WatchWindowDialog::on_lineEdit_tolerance_y_textChanged(const QString &arg1){
    if(this->ui->checkBox_y->isChecked()){
        this->settings.displayValues.insert(eY, this->ui->lineEdit_tolerance_y->text().toDouble());
    }
}

/*!
 * \brief WatchWindowDialog::on_lineEdit_z_textChanged
 * \param arg1
 */
void WatchWindowDialog::on_lineEdit_tolerance_z_textChanged(const QString &arg1){
    if(this->ui->checkBox_z->isChecked()){
        this->settings.displayValues.insert(eZ, this->ui->lineEdit_tolerance_z->text().toDouble());
    }
}

/*!
 * \brief WatchWindowDialog::on_lineEdit_d3d_textChanged
 * \param arg1
 */
void WatchWindowDialog::on_lineEdit_tolerance_d3d_textChanged(const QString &arg1){
    if(this->ui->checkBox_d3d->isChecked()){
        this->settings.displayValues.insert(eD3D, this->ui->lineEdit_tolerance_d3d->text().toDouble());
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

    //necessary to rescale all
    this->lablesRescaled = false;

    //put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );

    //connect sensor
    this->connectSensor();

    //start reading stream
    emit this->startStreaming(this->settings.readingType);

    //switch to watchwindow tab
    this->ui->toolBox->setCurrentIndex(0);

    //set to 0 and false => text will be scaled when window is displayed
    oldWindowHeight = 0;
    oldWindowWidth = 0;
    this->lablesRescaled = false;

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
 * all stretch-values are set to 1. This means that all attributes have the same size in the window.
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
    addLabel(eName, f);

    //x
    addLabel(eX, f);

    //y
    addLabel(eY, f);

    //z
    addLabel(eZ, f);

    //d3D
    addLabel(eD3D, f);

    //assign master layout
    this->ui->pageWatchWindow->setLayout(this->masterLayout);
}

void WatchWindowDialog::addLabel(DisplayAttributes att,  QFont f) {
    QLabel *label = new QLabel();
    label->setFont(f);
    label->setAutoFillBackground(true);
    label->setAlignment(Qt::AlignVCenter);
    label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    label->setScaledContents(true);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(label, 1);
    masterLayout->addLayout(layout, 1);
    this->masterLayoutIndex[att] = this->masterLayout->count()-1;
    streamData.insert(att, label);
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

    //check the active position (geometry, station, coordinate system)
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
    int numVisibleElements = 1; //1, because name is always displayed

    //set feature name
    name ="<p align=\"center\">" + this->currentJob->getActiveFeature()->getFeature()->getFeatureName() + "</p>";
    displayValue = "<table width=\"100%\"> <tr> <td>" + name + "</td>  </tr> </table>";
    this->streamData[eName]->setText(displayValue);

    //set x
    if(this->settings.displayValues.contains(eX)){

        //get display value
        double displayX;
        if(this->settings.reference == eActualNominal){
            displayX = trackerXYZ.getAt(0) - pos.getVector().getAt(0);
        }else{
            displayX = pos.getVector().getAt(0) - trackerXYZ.getAt(0);
        }
        displayX = convertFromDefault(displayX, ModelManager::getParameterDisplayConfig().getDisplayUnit(eMetric));

        //set color depending on tolerance
        QString color = (qFabs(displayX) >= qFabs(this->settings.displayValues.value(eX)) ? "#FF0000" : "#00FF00");

        //format display value
        name = "<p align=\"left\">x</p>";
        value = "<p align=\"right\">" + QString::number(displayX, 'f', this->settings.digits) + "</p>";
        displayValue = "<table width=\"100%\"> <tr> <td width=\"20%\">" + name + "</td> <td width=\"70%\">" + value + "</td><td width=\"10%\" bgcolor=\"" + color + "\">&nbsp;</td> </tr></table>";
        streamData.value(eX)->setText(displayValue);

        numVisibleElements++;
    }

    //set y
    if(this->settings.displayValues.contains(eY)){

        //get display value
        double displayY;
        if(this->settings.reference == eActualNominal){
            displayY = trackerXYZ.getAt(1) - pos.getVector().getAt(1);
        }else{
            displayY = pos.getVector().getAt(1) - trackerXYZ.getAt(1);
        }
        displayY = convertFromDefault(displayY, ModelManager::getParameterDisplayConfig().getDisplayUnit(eMetric));

        //set color depending on tolerance
        QString color = (qFabs(displayY) >= qFabs(this->settings.displayValues.value(eY)) ? "#FF0000" : "#00FF00");

        //format display value
        name = "<p align=\"left\">y</p>";
        value = "<p align=\"right\">" + QString::number(displayY, 'f', this->settings.digits) + "</p>";
        displayValue = "<table width=\"100%\"> <tr> <td width=\"20%\">" + name + "</td> <td width=\"70%\">" + value + "</td><td width=\"10%\" bgcolor=\"" + color + "\">&nbsp;</td> </tr></table>";
        streamData.value(eY)->setText(displayValue);

        numVisibleElements++;
    }

    //set z
    if(this->settings.displayValues.contains(eZ)){

        //get display value
        double displayZ;
        if(this->settings.reference == eActualNominal){
            displayZ = trackerXYZ.getAt(2) - pos.getVector().getAt(2);
        }else{
            displayZ = pos.getVector().getAt(2) - trackerXYZ.getAt(2);
        }
        displayZ = convertFromDefault(displayZ, ModelManager::getParameterDisplayConfig().getDisplayUnit(eMetric));

        //set color depending on tolerance
        QString color = (qFabs(displayZ) >= qFabs(this->settings.displayValues.value(eZ)) ? "#FF0000" : "#00FF00");

        //format display value
        name = "<p align=\"left\">z</p>";
        value = "<p align=\"right\">" + QString::number(displayZ, 'f', this->settings.digits) + "</p>";
        displayValue = "<table width=\"100%\"> <tr> <td width=\"20%\">" + name + "</td> <td width=\"70%\">" + value + "</td><td width=\"10%\" bgcolor=\"" + color + "\">&nbsp;</td> </tr></table>";
        streamData.value(eZ)->setText(displayValue);

        numVisibleElements++;
    }

    //set d3D
    if(this->settings.displayValues.contains(eD3D)){

        //get display value
        OiVec d = pos.getVectorH() - trackerXYZ;
        double displayD3D = qSqrt(d.getAt(0)*d.getAt(0)+d.getAt(1)*d.getAt(1)+d.getAt(2)*d.getAt(2));

        displayD3D = convertFromDefault(displayD3D, ModelManager::getParameterDisplayConfig().getDisplayUnit(eMetric));

        //set color depending on tolerance
        QString color = (qFabs(displayD3D) >= qFabs(this->settings.displayValues.value(eD3D)) ? "#FF0000" : "#00FF00");

        //format display value
        name = "<p align=\"left\">d3D</p>";
        value = "<p align=\"right\">" + QString::number(displayD3D, 'f', this->settings.digits) + "</p>";
        displayValue = "<table width=\"100%\"> <tr> <td width=\"20%\">" + name + "</td> <td width=\"70%\">" + value + "</td><td width=\"10%\" bgcolor=\"" + color + "\">&nbsp;</td> </tr></table>";
        streamData.value(eD3D)->setText(displayValue);

        numVisibleElements++;
    }

    //set visibility
    //list of visible layouts (0=name 1=x 2=y 3=z 4=d3D
    QList<DisplayAttributes>  visibleLayouts;

    if(this->settings.displayValues.contains(eX)){
        this->streamData[eX]->setVisible(true);
        visibleLayouts.append(eX);
    }else{
        this->streamData[eX]->setVisible(false);
    }
    if(this->settings.displayValues.contains(eY)){
        this->streamData[eY]->setVisible(true);
        visibleLayouts.append(eY);
    }else{
        this->streamData[eY]->setVisible(false);
    }
    if(this->settings.displayValues.contains(eZ)){
        this->streamData[eZ]->setVisible(true);
        visibleLayouts.append(eZ);
    }else{
        this->streamData[eZ]->setVisible(false);
    }
    if(this->settings.displayValues.contains(eD3D)){
        this->streamData[eD3D]->setVisible(true);
        visibleLayouts.append(eD3D);
    }else{
        this->streamData[eD3D]->setVisible(false);
    }

    //set all streching to 0
    //except feature name label
    for ( int i = eX; i != eNotDeclared; i++ ) {
        DisplayAttributes attr = static_cast<DisplayAttributes>(i);
        if(!visibleLayouts.contains(attr)){
            this->masterLayout->setStretch(masterLayoutIndex[attr], 0);
        }
    }

    //set all active attributes to same stretch value
    for ( int i = eName; i != eNotDeclared; i++ ) {
        DisplayAttributes attr = static_cast<DisplayAttributes>(i);
        if(visibleLayouts.contains(attr)){
            this->masterLayout->setStretch(masterLayoutIndex[attr], 1);
        }
    }

    //resize labels (maximum font size that is possible)
    this->resizeWatchWindowValues();
}

/*!
 * \brief WatchWindowDialog::getDefaultSettings
 * get all attributes to display and save their tolerances
 */
void WatchWindowDialog::getDefaultSettings(){

    if(this->ui->checkBox_x->isChecked()){
        this->settings.displayValues.insert(eX, this->ui->lineEdit_tolerance_x->text().toDouble());
    }else{
        this->settings.displayValues.remove(eX);
    }

    if(this->ui->checkBox_y->isChecked()){
        this->settings.displayValues.insert(eY, this->ui->lineEdit_tolerance_y->text().toDouble());
    }else{
        this->settings.displayValues.remove(eY);
    }

    if(this->ui->checkBox_z->isChecked()){
        this->settings.displayValues.insert(eZ, this->ui->lineEdit_tolerance_z->text().toDouble());
    }else{
        this->settings.displayValues.remove(eZ);
    }

    if(this->ui->checkBox_d3d->isChecked()){
        this->settings.displayValues.insert(eD3D, this->ui->lineEdit_tolerance_d3d->text().toDouble());
    }else{
        this->settings.displayValues.remove(eD3D);
    }
}

/*!
 * \brief WatchWindowDialog::resizeWatchWindowValues
 * Resizes the watch window values so that the font is maximum
 */
void WatchWindowDialog::resizeWatchWindowValues(){

    if(!this->lablesRescaled || oldWindowHeight != this->height() || oldWindowWidth != this->width()){
        //init variables
        double h = 0.0, scale = 0.0;

        this->ui->pageWatchWindow->setLayout(this->masterLayout);

        //show the labels first so the right font is returned
        this->streamData[eName]->show();
        if(this->settings.displayValues.contains(eX)){
            this->streamData[eX]->show();
        }
        if(this->settings.displayValues.contains(eY)){
            this->streamData[eY]->show();
        }
        if(this->settings.displayValues.contains(eZ)){
            this->streamData[eZ]->show();
        }
        if(this->settings.displayValues.contains(eD3D)){
            this->streamData[eD3D]->show();
        }

        oldWindowHeight = this->rect().height();
        oldWindowWidth = this->rect().width();

        //get current fonts
        QFont fName = this->streamData[eName]->font();
        QFont fX = this->streamData[eX]->font();
        QFont fY = this->streamData[eY]->font();
        QFont fZ = this->streamData[eZ]->font();
        QFont fd3D = this->streamData[eD3D]->font();

        //calculate new fonts
        //name
        h = this->streamData[eName]->height();
        QFontMetrics fmName(fName);
        scale = h/fmName.height();
        fName.setPointSize(fName.pointSize()*scale);

        //x
        h = this->streamData[eX]->height();
        QFontMetrics fmX(fX);
        scale = h/fmX.height();
        fX.setPointSize(fX.pointSize()*scale);

        //y
        h = this->streamData[eY]->height();
        QFontMetrics fmY(fY);
        scale = h/fmY.height();
        fY.setPointSize(fY.pointSize()*scale);

        //z
        h = this->streamData[eZ]->height();
        QFontMetrics fmZ(fZ);
        scale = h/fmZ.height();
        fZ.setPointSize(fZ.pointSize()*scale);

        //d3D
        h = this->streamData[eD3D]->height();
        QFontMetrics fmd3D(fd3D);
        scale = h/fmd3D.height();
        fd3D.setPointSize(fd3D.pointSize()*scale);

        //set new fonts
        this->streamData[eName]->setFont(fName);
        this->streamData[eX]->setFont(fX);
        this->streamData[eY]->setFont(fY);
        this->streamData[eZ]->setFont(fZ);
        this->streamData[eD3D]->setFont(fd3D);

        //labels are already rescaled with this GUI setup
        this->lablesRescaled = true;
    }
}

/*!
 * \brief WatchWindowDialog::on_toolBox_currentChanged
 * \param index
 * if you switch from settings tab to watchwindow tab you have to recalc label sizes
 */
void WatchWindowDialog::on_toolBox_currentChanged(int index)
{
    if(index == 0){
        this->lablesRescaled = false;
    }
    /*!
    Display settings are changed in settings tab (activate/ deactivate attributes). Immediately the labels will
    be resized, but the used size information of the labels is not up to date.
    Because the current tab is settings, labels of watchwindow tab have their old size.
    After switching to the watchwindow tab, they will get updated and it is necessary to update the size calculation.
    */
}
