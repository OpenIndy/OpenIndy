#ifndef BUNDLESTATIONITEM_H
#define BUNDLESTATIONITEM_H

#include <QVariant>

/*!
 * \brief The BundleStationItem class
 */
class BundleStationItem
{
public:
    BundleStationItem(const QString &key, const bool &value, const int &stationId, BundleStationItem *parentItem = 0);
    ~BundleStationItem();

    //######################
    //tree view item methods
    //######################

    //children
    void appendChild(BundleStationItem *child);
    void removeAllChildren();
    BundleStationItem *child(int row);
    int childCount() const;

    //row and column count
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;

    //parent
    BundleStationItem *parentItem();
    void setParentItem(BundleStationItem *parent);

    //#################
    //getter and setter
    //#################

    //station id
    const int &getStationId() const;

    //key
    const QString &getKey() const;

    //value
    bool getValue() const;
    void setValue(const bool &value);

private:

    //##########
    //attributes
    //##########

    //display values
    QString key;
    int stationId;
    bool value;

    //children and parent
    QList<BundleStationItem*> m_childItems;
    BundleStationItem *m_parentItem;

};

#endif // BUNDLESTATIONITEM_H
