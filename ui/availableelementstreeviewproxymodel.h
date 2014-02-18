#ifndef AVAILABLEELEMENTSTREEVIEWPROXYMODEL_H
#define AVAILABLEELEMENTSTREEVIEWPROXYMODEL_H

#include <QSortFilterProxyModel>

#include "configuration.h"
#include "featuretreeitem.h"
#include "featuretreeviewmodel.h"

class AvailableElementsTreeViewProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit AvailableElementsTreeViewProxyModel(QObject *parent = 0);

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void setHeader(QString header);

    Configuration::ElementTypes getNeededElement();
    FeatureTreeItem* getSelectedItem(const QModelIndex &index);

public slots:
    void setFilter(Configuration::ElementTypes typeOfElement, bool hideAll);

protected:
    bool filterAcceptsColumn (int source_column, const QModelIndex & source_parent) const;
    bool filterAcceptsRow (int source_row, const QModelIndex & source_parent) const;

private:
    QString header;
    bool hideAll;
    Configuration::ElementTypes neededElement;

    bool filterAcceptPoint(int source_row, const QModelIndex & source_parent) const;
    bool filterAcceptLine(int source_row, const QModelIndex & source_parent) const;
    bool filterAcceptPlane(int source_row, const QModelIndex & source_parent) const;
    bool filterAcceptSphere(int source_row, const QModelIndex & source_parent) const;
    bool filterAcceptScalarEntityDistance(int source_row, const QModelIndex & source_parent) const;
    bool filterAcceptScalarEntityAngle(int source_row, const QModelIndex & source_parent) const;
    bool filterAcceptStation(int source_row, const QModelIndex & source_parent) const;
    bool filterAcceptCoordinateSystem(int source_row, const QModelIndex & source_parent) const;
    bool filterAcceptTrafoParam(int source_row, const QModelIndex & source_parent) const;
    bool filterAcceptObservation(int source_row, const QModelIndex & source_parent) const;
    bool filterAcceptReadingDistance(int source_row, const QModelIndex & source_parent) const;
    bool filterAcceptReadingCartesian(int source_row, const QModelIndex & source_parent) const;
    bool filterAcceptReadingPolar(int source_row, const QModelIndex & source_parent) const;
    bool filterAcceptReadingDirection(int source_row, const QModelIndex & source_parent) const;

};

#endif // AVAILABLEELEMENTSTREEVIEWPROXYMODEL_H
