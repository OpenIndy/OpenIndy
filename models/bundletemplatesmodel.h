#ifndef BUNDLETEMPLATESMODEL_H
#define BUNDLETEMPLATESMODEL_H

#include <QAbstractListModel>
#include <QModelIndex>
#include <QPointer>
#include <QFile>
#include <QDirIterator>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

#include "types.h"
#include "util.h"
#include "oijob.h"

using namespace oi;

/*!
 * \brief The BundleTemplatesModel class
 * Model that holds all available bundle templates
 */
class BundleTemplatesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit BundleTemplatesModel(QObject *parent = 0);

    //#######################################
    //override methods of abstract item model
    //#######################################

    int	rowCount(const QModelIndex & parent = QModelIndex()) const;
    int	columnCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    //##########################################
    //load bundle templates from template folder
    //##########################################

    void loadTemplates();

    //############
    //get template
    //############

    QJsonObject getBundleTemplate(const int &index);

signals:

    //#########################
    //send messages to OpenIndy
    //#########################

    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);

private slots:

    //################
    //update the model
    //################

    void updateModel();

private:

    //##############
    //helper methods
    //##############

    //bundle templates
    QList<QJsonObject> templates;

};

#endif // BUNDLETEMPLATESMODEL_H
