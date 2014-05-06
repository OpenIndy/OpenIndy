#include "plugintreeviewmodel.h"

PluginTreeViewModel::PluginTreeViewModel(QObject *parent)
{
    this->rootItem = new PluginTreeItem();
}

/*!
 * \brief PluginTreeViewModel::refreshModel
 * Refresh the model by querying the database for available plugins
 */
void PluginTreeViewModel::refreshModel(){
    emit this->layoutAboutToBeChanged();
    emit this->layoutChanged();
}

/*!
 * \brief PluginTreeViewModel::index
 * \param row
 * \param column
 * \param parent
 * \return
 */
QModelIndex PluginTreeViewModel::index(int row, int column, const QModelIndex &parent) const{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    PluginTreeItem *parentItem = NULL;

    if (!parent.isValid())
        parentItem = this->rootItem;
    else
        parentItem = static_cast<PluginTreeItem*>(parent.internalPointer());

    if(parentItem != NULL){
        PluginTreeItem *childItem = parentItem->getChild(row);
        if (childItem != NULL)
            return createIndex(row, column, childItem);
        else
            return QModelIndex();
    }else{
        return QModelIndex();
    }
}

/*!
 * \brief PluginTreeViewModel::parent
 * \param index
 * \return
 */
QModelIndex PluginTreeViewModel::parent(const QModelIndex &index) const{
    if (!index.isValid())
        return QModelIndex();

    PluginTreeItem *childItem = static_cast<PluginTreeItem*>(index.internalPointer());


    if(childItem != NULL){
        PluginTreeItem *parentItem = childItem->getParent();

        if (parentItem == NULL || parentItem == this->rootItem)
            return QModelIndex();

        return createIndex(parentItem->getIndex(), 0, parentItem);
    }else{
        return QModelIndex();
    }
}

/*!
 * \brief PluginTreeViewModel::rowCount
 * \param parent
 * \return
 */
int PluginTreeViewModel::rowCount(const QModelIndex &parent) const{
    PluginTreeItem *childItem = NULL;

    if(!parent.isValid()){
        childItem = this->rootItem;
    }else{
        childItem = static_cast<PluginTreeItem*>(parent.internalPointer());
    }
    if(childItem != NULL){
        return childItem->getChildCount();
    }

    return 0;
}

/*!
 * \brief PluginTreeViewModel::columnCount
 * \param parent
 * \return
 */
int PluginTreeViewModel::columnCount(const QModelIndex &parent) const{
    return 1;
}

/*!
 * \brief PluginTreeViewModel::data
 * \param index
 * \param role
 * \return
 */
QVariant PluginTreeViewModel::data(const QModelIndex &index, int role) const{
    if (!index.isValid())
        return QVariant();

    PluginTreeItem *item = static_cast<PluginTreeItem*>(index.internalPointer());

    if(item != NULL){
        if(role == Qt::DisplayRole){ //return display text for tree view item
            return item->getDisplayValue();
        }else if(role == Qt::DecorationRole){ //return icon for tree view item
            if(item->getIsPlugin()){
                QPixmap pix(":/Images/icons/loadPlugin.png");
                return pix.scaledToHeight(12, Qt::SmoothTransformation);
            }
        }
    }

    return QVariant();
}
