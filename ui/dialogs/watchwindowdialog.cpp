#include "watchwindowdialog.h"
#include "ui_watchwindowdialog.h"

/*!
 * \brief WatchWindowDialog::WatchWindowDialog
 * \param parent
 */
WatchWindowDialog::WatchWindowDialog(WatchWindowBehavior b, QPointer<OiJob> job, QList<QPointer<FeatureWrapper> > f, QWidget *parent) : QDialog(parent),
    ui(new Ui::WatchWindowDialog), behavior(b), currentJob(job), features(f)
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

    // time that monitors "watchWindowUpdated" state
    this->watchWindowUpdated = false;
    static QTimer *timer = new QTimer(parent);
    connect(timer, SIGNAL(timeout()), this, SLOT(clearWatchWindow()));
    timer->start(500);
}

/*!
 * \brief WatchWindowDialog::~WatchWindowDialog
 */
WatchWindowDialog::~WatchWindowDialog(){
    delete this->ui;
}

/*!
 * \brief WatchWindowDialog::on_spinBox_decimalDigits_valueChanged
 * \param arg1
 */
void WatchWindowDialog::on_spinBox_decimalDigits_valueChanged(int arg1){
    this->settings.digits = this->ui->spinBox_decimalDigits->value();
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
    label->setAlignment(Qt::AlignCenter);
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


QString WatchWindowDialog::getNameLabel(QPointer<FeatureWrapper> feature) {
    if(feature.isNull()){
        return "";
    }

    return QString("%1%2")
            .arg(feature->getFeature()->getFeatureName())
            .arg(feature->getGeometry().isNull() ? "" : feature->getGeometry()->getIsNominal() ? "  nom" : "  act");
}

/*!
 * \brief WatchWindowDialog::setUpCartesianWatchWindow
 * \param reading
 */
void WatchWindowDialog::setUpCartesianWatchWindow(const QVariantMap &reading){
    //get and transform tracker position
    if(!reading.contains("x") || !reading.contains("y") || !reading.contains("z")){
        return;
    }

    //check and get active coordinate system
    QPointer<CoordinateSystem> activeSystem = this->currentJob->getActiveCoordinateSystem();
    if(activeSystem.isNull()){
        return;
    }

    //get transformation parameters to transform readings
    OiMat trafo(4,4);
    if(!this->trafoController.getTransformationMatrix(trafo, this->activeStation->getCoordinateSystem(), activeSystem)){
        return;
    }

    OiVec trackerXYZ(4);
    trackerXYZ.setAt(0, reading.value("x").toDouble());
    trackerXYZ.setAt(1, reading.value("y").toDouble());
    trackerXYZ.setAt(2, reading.value("z").toDouble());
    trackerXYZ.setAt(3, 1.0);
    trackerXYZ = trafo * trackerXYZ;

    QPointer<FeatureWrapper> feature = getFeature(trackerXYZ);
    //check the active position (geometry, station, coordinate system)
    Result result = util.getPosition(feature, trackerXYZ);

    if(result.position.isNull()) {
        return;
    }

    OiVec delta = result.delta;
    //set feature name
    this->streamData[eName]->setText(getNameLabel(feature));

    //set x
    setDisplayValue(eX, "x", [&](){
        //get display value
        return delta.getAt(0);
    });

    //set y
    setDisplayValue(eY, "y", [&](){
        //get display value
        return delta.getAt(1);
    });

    //set z
    setDisplayValue(eZ, "z", [&](){
        //get display value
        return delta.getAt(2);
    });

    //set d3D    
    setDisplayValue(eD3D, "d3D", [&](){
        //get display value
        return result.d3D;
    });

    setVisibility();

    // valid reading available
    this->watchWindowUpdated = true;

    //resize labels (maximum font size that is possible)
    this->resizeWatchWindowValues();
}

void WatchWindowDialog::setVisibility() {
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
}

void WatchWindowDialog::setDisplayValue(DisplayAttributes attr, QString name, std::function<double()> v) {
    if(this->settings.displayValues.contains(attr)){
        double value = convertFromDefault(v(), ModelManager::getParameterDisplayConfig().getDisplayUnit(eMetric));
        //set color depending on tolerance
        QString color = (qFabs(value) >= qFabs(this->settings.displayValues.value(attr)) ? "#FF0000" : "#00FF00");

        //format display value
        streamData.value(attr)->setText(QString("<table width=\"100%\"> <tr> <td width=\"20%\"><p align=\"left\">%1</p></td> <td width=\"70%\"><p align=\"right\">%2</p></td><td width=\"10%\" bgcolor=\"%3\">&nbsp;</td> </tr></table>")
                                        .arg(name)
                                        .arg(QString::number(value, 'f', this->settings.digits))
                                        .arg(color));
    }
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

    this->settings.showLastMeasurement = this->ui->checkBox_showLastMeasurement->isChecked();
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
        double w = this->streamData[eName]->width();
        QFontMetrics fmName(fName);
        fName.setPointSize(fName.pointSize() * min(h/fmName.height(), w/fmName.width(streamData[eName]->text()) ));

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

void WatchWindowDialog::clearWatchWindow() {
    if(!settings.showLastMeasurement &&  !this->watchWindowUpdated) {
        this->streamData[eX]->setVisible(false);
        this->streamData[eY]->setVisible(false);
        this->streamData[eZ]->setVisible(false);
        this->streamData[eD3D]->setVisible(false);
    }
    this->watchWindowUpdated = false;
}

void WatchWindowDialog::on_checkBox_showLastMeasurement_clicked()
{
    this->settings.showLastMeasurement = this->ui->checkBox_showLastMeasurement->isChecked();
}

QPointer<FeatureWrapper> WatchWindowDialog::getFeature(OiVec trackerXYZ){
    switch(this->behavior) {
    case eShowAlwaysActiveFeature:
        if(!this->currentJob.isNull()) {
            return this->currentJob->getActiveFeature();
        }
        break;
    case eShowCurrentSelectedFeature:
        if(!this->features.isEmpty()) {
            return this->features[0];
        }
        break;
    case eShowNearestNominal:
        if(!this->currentJob.isNull()) {
            double dd = -1.0;
            QPointer<FeatureWrapper> nearestFeature;
            for(QPointer<FeatureWrapper> feature : (true ? this->features : this->currentJob->getFeaturesList())) {

                Position pos = util.getPosition(feature, trackerXYZ).position;
                if(!pos.isNull() && !feature->getPoint().isNull() /* filter point */) {

                    OiVec d = pos.getVectorH() - trackerXYZ;
                    double fdd =  d.getAt(0)*d.getAt(0)+d.getAt(1)*d.getAt(1)+d.getAt(2)*d.getAt(2); // no need for sqrt
                    if(fdd < dd || dd < 0.0 /* first element */) {
                        dd = fdd;
                        nearestFeature = feature;
                    }

                }

            }
            return nearestFeature;
        }
        break;
    }

    return QPointer<FeatureWrapper>();
}
