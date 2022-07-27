#ifndef MEASUREMENTCONFIGURATIONMODEL_H
#define MEASUREMENTCONFIGURATIONMODEL_H

#include <QAbstractListModel>
#include <QModelIndex>
#include <QPixmap>
#include <QPointer>

#include "types.h"
#include "util.h"
#include "measurementconfigmanager.h"

#if defined(OI_MAIN_LIB)
#  define OI_MAIN_EXPORT Q_DECL_EXPORT
#else
#  define OI_MAIN_EXPORT Q_DECL_IMPORT
#endif

using namespace oi;

/*!
 * \brief The MeasurementConfigurationModel class
 * Model that holds all available measurement configurations
 */
class OI_MAIN_EXPORT MeasurementConfigurationModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit MeasurementConfigurationModel(QObject *parent = 0);

    //#######################################
    //override methods of abstract item model
    //#######################################

    int	rowCount(const QModelIndex & parent = QModelIndex()) const;
    int	columnCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    //##########################################
    //get informations about measurement configs
    //##########################################

    QModelIndex getIndex(const MeasurementConfig &mConfig) const;

    MeasurementConfig getMeasurementConfig(const QModelIndex &index) const;

    bool getIsSaved(const QModelIndex &index) const;

    //#################################
    //add or remove measurement configs
    //#################################

    QModelIndex addMeasurementConfig(const MeasurementConfig &mConfig);
    void removeMeasurementConfig(const QModelIndex &index);

    void replaceMeasurementConfig(const QString &name, const MeasurementConfig &mConfig);

    void cloneMeasurementConfig(const MeasurementConfig &mConfig);

    //#######################################
    //get or set sensor configuration manager
    //#######################################

    const QPointer<MeasurementConfigManager> &getMeasurementConfigurationManager() const;
    void setMeasurementConfigurationManager(const QPointer<MeasurementConfigManager> &manager);

signals:

    //#########################
    //send messages to OpenIndy
    //#########################

    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);

    //########################################################
    //inform when the name of a measurement config has changed
    //########################################################

    void measurementConfigNameChanged(const MeasurementConfig &mConfig);

private slots:

    //###########################################
    //update the model when configuration changes
    //###########################################

    void updateModel();

private:

    //##############
    //helper methods
    //##############

    void connectConfigManager();

    //#####################
    //sensor config manager
    //#####################

    QPointer<MeasurementConfigManager> measurementConfigManager;

};

#endif // MEASUREMENTCONFIGURATIONMODEL_H
