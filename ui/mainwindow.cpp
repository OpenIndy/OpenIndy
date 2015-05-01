#include "mainwindow.h"
#include "ui_mainwindow.h"

/*!
 * \brief MainWindow::MainWindow
 * \param parent
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){

    ui->setupUi(this);

    //assign models of ModelManager to GUI-elements
    this->assignModels();

    //init GUI elements
    this->initFeatureTableViews();

    //connect controller and dialogs
    this->connectDialogs();
    this->connectController();

}

/*!
 * \brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow(){
    delete ui;
}

/*!
 * \brief MainWindow::importNominalsStarted
 * Triggered by Controller whenever an nominal import task has been started
 */
void MainWindow::importNominalsStarted(){
    this->loadingDialog.show();
}

/*!
 * \brief MainWindow::importNominalsFinished
 * Triggered by Controller whenever an import nominals task has been finished
 * \param success
 */
void MainWindow::importNominalsFinished(const bool &success){

    //print import success to console
    if(success){
        Console::getInstance()->addLine("Nominals successfully imported");
    }else{
        Console::getInstance()->addLine("Nominals not imported successfully");
    }

    this->loadingDialog.close();

}

/*!
 * \brief MainWindow::connectController
 */
void MainWindow::connectController(){

    //connect actions triggered by user to slots in controller
    QObject::connect(this, &MainWindow::addFeatures, &this->control, &Controller::addFeatures, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::importNominals, &this->control, &Controller::importNominals, Qt::AutoConnection);

    //connect actions triggered by controller to slots in main window
    QObject::connect(&this->control, &Controller::nominalImportStarted, this, &MainWindow::importNominalsStarted, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::nominalImportFinished, this, &MainWindow::importNominalsFinished, Qt::AutoConnection);

}

/*!
 * \brief MainWindow::connectDialogs
 */
void MainWindow::connectDialogs(){

    //connect create feature dialog
    QObject::connect(&this->createFeatureDialog, &CreateFeatureDialog::addFeatures, this, &MainWindow::addFeatures, Qt::AutoConnection);

    //connect console
    QObject::connect(Console::getInstance().data(), &Console::lineAdded, this->ui->listView_console, &QListView::scrollToBottom, Qt::AutoConnection);

    //connect import dialogs
    QObject::connect(&this->importNominalDialog, &ImportNominalDialog::startImport, this, &MainWindow::importNominals, Qt::AutoConnection);

    //connect loading dialog
    QObject::connect(&this->control, &Controller::nominalImportProgressUpdated, &this->loadingDialog, &LoadingDialog::updateProgress, Qt::AutoConnection);

}

/*!
 * \brief MainWindow::assignModels
 * Assign the models of ModelManager to GUI-elements in MainWindow
 */
void MainWindow::assignModels(){

    //assign feature table views
    this->ui->tableView_features->setModel(&ModelManager::getFeatureTableProxyModel());

    //assign console model
    this->ui->listView_console->setModel(&Console::getInstance()->getConsoleModel());

}

/*!
 * \brief MainWindow::initFeatureTableViews
 */
void MainWindow::initFeatureTableViews(){

    //resize rows and columns to table view contents
    this->ui->tableView_features->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    this->ui->tableView_features->verticalHeader()->setDefaultSectionSize(22);
    this->ui->tableView_trafoParams->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    this->ui->tableView_trafoParams->verticalHeader()->setDefaultSectionSize(22);

}

/*!
 * \brief MainWindow::on_actionCreate_point_triggered
 */
void MainWindow::on_actionCreate_point_triggered(){
    this->createFeatureDialog.setFeatureType(ePointFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_line_triggered
 */
void MainWindow::on_actionCreate_line_triggered(){
    this->createFeatureDialog.setFeatureType(eLineFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_plane_triggered
 */
void MainWindow::on_actionCreate_plane_triggered(){
    this->createFeatureDialog.setFeatureType(ePlaneFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_sphere_triggered
 */
void MainWindow::on_actionCreate_sphere_triggered(){
    this->createFeatureDialog.setFeatureType(eSphereFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_station_triggered
 */
void MainWindow::on_actionCreate_station_triggered(){
    this->createFeatureDialog.setFeatureType(eStationFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_coordinatesystem_triggered
 */
void MainWindow::on_actionCreate_coordinatesystem_triggered(){
    this->createFeatureDialog.setFeatureType(eCoordinateSystemFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_scalar_entity_triggered
 */
void MainWindow::on_actionCreate_scalar_entity_triggered(){
    this->createFeatureDialog.setFeatureType(eScalarEntityAngleFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_trafoParam_triggered
 */
void MainWindow::on_actionCreate_trafoParam_triggered(){
    this->createFeatureDialog.setFeatureType(eTrafoParamFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_cone_triggered
 */
void MainWindow::on_actionCreate_cone_triggered(){
    this->createFeatureDialog.setFeatureType(eConeFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_cylinder_triggered
 */
void MainWindow::on_actionCreate_cylinder_triggered(){
    this->createFeatureDialog.setFeatureType(eCylinderFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_ellipsoid_triggered
 */
void MainWindow::on_actionCreate_ellipsoid_triggered(){
    this->createFeatureDialog.setFeatureType(eEllipsoidFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_hyperboloid_triggered
 */
void MainWindow::on_actionCreate_hyperboloid_triggered(){
    this->createFeatureDialog.setFeatureType(eHyperboloidFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_paraboloid_triggered
 */
void MainWindow::on_actionCreate_paraboloid_triggered(){
    this->createFeatureDialog.setFeatureType(eParaboloidFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_nurbs_triggered
 */
void MainWindow::on_actionCreate_nurbs_triggered(){
    this->createFeatureDialog.setFeatureType(eNurbsFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_pointcloud_triggered
 */
void MainWindow::on_actionCreate_pointcloud_triggered(){
    this->createFeatureDialog.setFeatureType(ePointCloudFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_circle_triggered
 */
void MainWindow::on_actionCreate_circle_triggered(){
    this->createFeatureDialog.setFeatureType(eCircleFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_torus_triggered
 */
void MainWindow::on_actionCreate_torus_triggered(){
    this->createFeatureDialog.setFeatureType(eTorusFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_slotted_hole_triggered
 */
void MainWindow::on_actionCreate_slotted_hole_triggered(){
    this->createFeatureDialog.setFeatureType(eSlottedHoleFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_ellipse_triggered
 */
void MainWindow::on_actionCreate_ellipse_triggered(){
    this->createFeatureDialog.setFeatureType(eEllipseFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionLoad_plugins_triggered
 */
void MainWindow::on_actionLoad_plugins_triggered(){
    this->pluginLoaderDialog.show();
}

/*!
 * \brief MainWindow::on_action_importNominals_triggered
 */
void MainWindow::on_action_importNominals_triggered(){
    this->importNominalDialog.show();
}
