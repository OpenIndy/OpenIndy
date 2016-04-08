#include "bundletemplatesmodel.h"

/*!
 * \brief BundleTemplatesModel::BundleTemplatesModel
 * \param parent
 */
BundleTemplatesModel::BundleTemplatesModel(QObject *parent) : QAbstractListModel(parent){

}

/*!
 * \brief BundleTemplatesModel::rowCount
 * \param parent
 * \return
 */
int BundleTemplatesModel::rowCount(const QModelIndex &parent) const{
    return this->templates.size();
}

/*!
 * \brief BundleTemplatesModel::columnCount
 * \param parent
 * \return
 */
int BundleTemplatesModel::columnCount(const QModelIndex &parent) const{
    return 1;
}

/*!
 * \brief BundleTemplatesModel::data
 * \param index
 * \param role
 * \return
 */
QVariant BundleTemplatesModel::data(const QModelIndex &index, int role) const{

    //check index
    if(!index.isValid()){
        return QVariant();
    }

    //check role
    if(role != Qt::DisplayRole){
        return QVariant();
    }

    //get row index
    int row = index.row();

    //get bundle template
    if(this->templates.size() > row){
        QJsonObject bundleTemplate = this->templates.at(row);
        return bundleTemplate.value("name").toString();
    }

    return QVariant();

}

/*!
 * \brief BundleTemplatesModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant BundleTemplatesModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if((Qt::DisplayRole == role) &&
            (Qt::Horizontal == orientation) &&
            (0 <= section) &&
            (section < columnCount())){

        return "bundle templates";

    }
    return QVariant();
}

/*!
 * \brief BundleTemplatesModel::flags
 * \param index
 * \return
 */
Qt::ItemFlags BundleTemplatesModel::flags(const QModelIndex &index) const{
    Qt::ItemFlags myFlags = QAbstractListModel::flags(index);
    return myFlags;
}

/*!
 * \brief BundleTemplatesModel::updateModel
 */
void BundleTemplatesModel::updateModel(){
    emit this->layoutAboutToBeChanged();
    emit this->layoutChanged();
}

/*!
 * \brief BundleTemplatesModel::loadTemplates
 */
void BundleTemplatesModel::loadTemplates(){

    //get template path
    QString path;
    path = QString("%1%2").arg(qApp->applicationDirPath()).arg("/templates/bundle");

    //iterate over templates
    QDirIterator it(path, QDirIterator::NoIteratorFlags);
    while(it.hasNext()){

        //load file
        QFile file(it.next());
        if(!file.open(QFile::ReadOnly)){
            continue;
        }

        //save json template
        QJsonDocument bundleTemplate;
        QJsonParseError parseError;
        bundleTemplate = QJsonDocument::fromJson(file.readAll(), &parseError);
        if(parseError.error != QJsonParseError::NoError){
            continue;
        }
        this->templates.append(bundleTemplate.object());

        //close file
        file.close();

    }

}

/*!
 * \brief BundleTemplatesModel::getBundleTemplate
 * \param index
 * \return
 */
QJsonObject BundleTemplatesModel::getBundleTemplate(const int &index){

    //check index
    if(this->templates.size() <= index || index < 0){
        return QJsonObject();
    }

    //get template
    QJsonObject bundleTemplate = this->templates.at(index);
    return bundleTemplate;

}
