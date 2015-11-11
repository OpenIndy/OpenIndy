#ifndef OBSERVATIONPROXYMODEL_H
#define OBSERVATIONPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QPointer>

#include "oijob.h"
#include "observationmodel.h"
#include "observationtablecolumnconfig.h"

using namespace oi;

/*!
 * \brief The ObservationProxyModel class
 */
class ObservationProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit ObservationProxyModel(QObject *parent = 0);

    //#########################
    //get or set display config
    //#########################

    const ObservationTableColumnConfig &getObservationTableColumnConfig() const;
    void setObservationTableColumnConfig(const ObservationTableColumnConfig &config);

signals:

    //#########################
    //send messages to OpenIndy
    //#########################

    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);

protected:

    //###########################################
    //override methods of sort filter proxy model
    //###########################################

    bool filterAcceptsRow (int source_row, const QModelIndex &source_parent ) const;
    bool filterAcceptsColumn (int source_column, const QModelIndex &source_parent ) const;

private:

    //##############
    //display config
    //##############

    ObservationTableColumnConfig observationTableColumnConfig;

};

#endif // OBSERVATIONPROXYMODEL_H
