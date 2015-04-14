#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QObject>
#include <QPointer>
#include <QAbstractTableModel>

#include "function.h"
#include "feature.h"
#include "featurewrapper.h"
#include "oijob.h"
#include "reading.h"

class FeatureTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit FeatureTableModel(const QPointer<OiJob> &job, QObject *parent = 0);
    explicit FeatureTableModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex & index) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    QPointer<OiJob> getCurrentJob();
    void setCurrentJob(const QPointer<OiJob> &job);

/*signals:
    void groupNameChanged(QString oldValue, QString nweValue);
    void resizeTable();*/
    
public slots:
    void updateModel();

private:
    QPointer<OiJob> currentJob;
    
};

#endif // TABLEMODEL_H
