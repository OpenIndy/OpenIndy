#ifndef BUNDLEGEOMETRIESMODEL_H
#define BUNDLEGEOMETRIESMODEL_H

#include <QAbstractTableModel>
#include <QAbstractItemModel>
#include <QObject>
#include <QColor>
#include <QDebug>

#include "bundlegeometryitem.h"

/*!
 * \brief The BundleGeometriesModel class
 * Model with all bundle geometries
 */
/*class BundleGeometriesModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit BundleGeometriesModel(QObject *parent = 0);
    ~BundleGeometriesModel();

    //########################################
    //override methods of abstract table model
    //########################################

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex & index) const;

    //###############
    //getter / setter
    //###############

    void setTrackerSettings(const ebk::core::TrackerSettings &trackerSettings);
    const ebk::core::TrackerSettings &getTrackerSettings();

    void setGeneralSettings(const GeneralSettings &settings);
    const GeneralSettings &getGeneralSettings();

    void setVisualSettings(const VisualizationSettings &settings);
    const VisualizationSettings &getVisualSettings();

    void setProDatSettings(const ebk::core::ProDatSettings &settings);
    const ebk::core::ProDatSettings &getProDatSettings();

private slots:

    //###########################################
    //update the model when settings have changed
    //###########################################

    void updateModel();

private:

    //############################
    //helper methods (set up data)
    //############################

    void setupModelData(SettingsItem *parent);
    void setupModelTracker(SettingsItem *parent);
    void setupModelMeasurement(SettingsItem *parent);
    void setupGeneralSettings(SettingsItem *parent);
    void setupVisualSettings(SettingsItem *parent);
    void setupProDatSettings(SettingsItem *parent);

    //########
    //settings
    //########

    ebk::core::TrackerSettings tracker;
    GeneralSettings generalSettings;
    VisualizationSettings visualSettings;
    ebk::core::ProDatSettings proDatSettings;

    bool updateSensorSetting(SettingsItem *item);
    bool updateMeasurementSetting(SettingsItem *item);
    bool updateGeneralSetting(SettingsItem *item);
    bool updateVisualSetting(SettingsItem *item);
    bool updateProDatSetting(SettingsItem *item);

    //#################
    //helper attributes
    //#################

    SettingsItem *rootItem;


};*/

#endif // BUNDLEGEOMETRIESMODEL_H
