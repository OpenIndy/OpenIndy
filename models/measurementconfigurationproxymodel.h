#ifndef MEASUREMENTCONFIGURATIONPROXYMODEL_H
#define MEASUREMENTCONFIGURATIONPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QPointer>

#include "types.h"
#include "util.h"
#include "measurementconfigurationmodel.h"

#if defined(OI_MAIN_LIB)
#  define OI_MAIN_EXPORT Q_DECL_EXPORT
#else
#  define OI_MAIN_EXPORT Q_DECL_IMPORT
#endif

using namespace oi;

enum MeasurementConfigurationFilterTypes{
    eNo_MeasurementConfigurationFilter= 0,
    eAll_MeasurementConfigurationFilter,
    eProject_MeasurementConfigurationFilter,
    eUser_MeasurementConfigurationFilter,
    eCreateFeature_MeasurementConfigurationFilter,
};

/*!
 * \brief The MeasurementConfigurationProxyModel class
 * Model that holds all available measurement configurations filtered by saved and project property
 */
class MeasurementConfigurationProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit MeasurementConfigurationProxyModel(QObject *parent = 0);

    //##########
    //set filter
    //##########

    void setFilter(const bool &showAll);
    void setFilter(const QList<ElementTypes> neededElements, FeatureTypes typeOfFeature, QList<FeatureTypes> applicableFor);
    void resetFilter();

    //###########################################
    //override methods of sort filter proxy model
    //###########################################

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

signals:

    //#########################
    //send messages to OpenIndy
    //#########################

    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);

protected:

    bool filterAcceptsRow (int source_row, const QModelIndex &source_parent) const;
    bool filterAcceptsColumn (int source_column, const QModelIndex &source_parent) const;

private:

    //################
    //filter attribute
    //################

    MeasurementConfigurationFilterTypes filterType;
    QList<ElementTypes> neededElements;
    FeatureTypes typeOfFeature;
    QList<FeatureTypes> functionIsApplicableFor;

};

#endif // MEASUREMENTCONFIGURATIONPROXYMODEL_H
