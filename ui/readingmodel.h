#ifndef READINGMODEL_H
#define READINGMODEL_H

#include <QAbstractTableModel>
#include "feature.h"
#include "featurewrapper.h"
#include "QStringList"

class ReadingModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ReadingModel(FeatureWrapper &activeFeature,QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    FeatureWrapper &selectedFeature;
    //QStringList m_columns;

signals:

public slots:
    void updateModel();
};

#endif // READINGMODEL_H
