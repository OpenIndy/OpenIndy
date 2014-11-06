#ifndef READINGMODEL_H
#define READINGMODEL_H

#include <QAbstractTableModel>
#include "feature.h"
#include "featurewrapper.h"
#include "QStringList"
#include "oifeaturestate.h"
#include "guiconfiguration.h"

/*!
 * \brief The ReadingModel class handles to display all the reading attributes.
 */
class ReadingModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ReadingModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

signals:

    void resizeView();

public slots:
    void updateModel();

private:
    QString getUndefValues(QMap<QString,double> undefReading) const;
};

#endif // READINGMODEL_H
