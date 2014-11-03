#include "oistakeoutmanager.h"
#include "ui_oistakeoutmanager.h"

OiStakeOutManager::OiStakeOutManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OiStakeOutManager)
{
    ui->setupUi(this);
}

OiStakeOutManager::~OiStakeOutManager()
{
    delete ui;
}

/*!
 * \brief OiStakeOutManager::setModels
 * \param myPoints
 * \param myGroups
 */
void OiStakeOutManager::setModels(PointFeatureFilterModel *myPoints, QStringListModel *myGroups){

    this->ui->comboBox_groups->setModel(myGroups);

    QStringList myMethods;
    myMethods.append("nearest");
    myMethods.append("sequence");
    this->ui->comboBox_methods->clear();
    this->ui->comboBox_methods->addItems(myMethods);

    this->ui->listView_geometries->setModel(myPoints);

}

/*!
 * \brief OiStakeOutManager::on_pushButton_ok_clicked
 */
void OiStakeOutManager::on_pushButton_ok_clicked(){

    QDomDocument request;

    //get selected points
    QModelIndexList selectedEntries = this->ui->listView_geometries->selectionModel()->selectedIndexes();
    PointFeatureFilterModel *model = dynamic_cast<PointFeatureFilterModel *>(this->ui->listView_geometries->model());
    QList<FeatureWrapper *> selectedPoints = model->getSelectedPoints(selectedEntries);

    if(selectedPoints.size() == 0){
        return;
    }

    QDomElement root = request.createElement("OiRequest");
    root.setAttribute("id", 13);
    request.appendChild(root);
    QDomElement mode = request.createElement("mode");
    mode.setAttribute("value", this->ui->comboBox_methods->currentText());
    root.appendChild(mode);
    QDomElement allGeometries = request.createElement("allGeometries");
    allGeometries.setAttribute("value", 0);
    root.appendChild(allGeometries);
    QDomElement geometries = request.createElement("geometries");
    foreach(FeatureWrapper *point, selectedPoints){
        if(point != NULL && point->getPoint() != NULL){
            QDomElement geom = request.createElement("geometry");
            geom.setAttribute("ref", point->getPoint()->getId());
            geometries.appendChild(geom);
        }
    }

    root.appendChild(geometries);

    qDebug() << request.toString();

    emit this->startStakeOut(request);

    this->close();

}

/*!
 * \brief OiStakeOutManager::on_pushButton_cancel_clicked
 */
void OiStakeOutManager::on_pushButton_cancel_clicked(){
    this->close();
}
