#ifndef BUNDLEGEOMETRYITEM_H
#define BUNDLEGEOMETRYITEM_H

#include <QVariant>

/*!
 * \brief The BundleGeometryItem class
 */
class BundleGeometryItem
{
public:
    BundleGeometryItem(const QString &key, BundleGeometryItem *parentItem = 0);
    ~BundleGeometryItem();

    //######################
    //tree view item methods
    //######################

    //children
    void appendChild(BundleGeometryItem *child);
    void removeAllChildren();
    BundleGeometryItem *child(int row);
    int childCount() const;

    //row and column count
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;

    //parent
    BundleGeometryItem *parentItem();
    void setParentItem(BundleGeometryItem *parent);

    //#################
    //getter and setter
    //#################

    //key
    const QString &getKey() const;

private:

    //##########
    //attributes
    //##########

    //display values
    QString key;

    //children and parent
    QList<BundleGeometryItem*> m_childItems;
    BundleGeometryItem *m_parentItem;

};

#endif // BUNDLEGEOMETRYITEM_H
