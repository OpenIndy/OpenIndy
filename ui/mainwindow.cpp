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

}

/*!
 * \brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow(){
    delete ui;
}

/*!
 * \brief MainWindow::connectController
 */
void MainWindow::connectController(){

    QObject::connect(this, SIGNAL(addFeatures(const FeatureAttributes&)), &this->control, SLOT(addFeatures(const FeatureAttributes&)));

}

/*!
 * \brief MainWindow::assignModels
 * Assign the models of ModelManager to GUI-elements in MainWindow
 */
void MainWindow::assignModels(){

    //assign feature table views
    this->ui->tableView_features->setModel(&ModelManager::getFeatureTableProxyModel());

}

/*!
 * \brief MainWindow::initFeatureTableViews
 */
void MainWindow::initFeatureTableViews(){

    //resize rows and columns to table view contents
    this->ui->tableView_features->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    this->ui->tableView_features->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
    this->ui->tableView_trafoParams->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    this->ui->tableView_trafoParams->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);

}
