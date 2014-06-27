#ifndef SIMULATIONERRORTABLEMODEL_H
#define SIMULATIONERRORTABLEMODEL_H

#include <QAbstractTableModel>
#include <QMap>
#include "uncertaintycomponent.h"

class SimulationErrorTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SimulationErrorTableModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex & index) const;

    void setErrors(QMap<QString, UncertaintyComponent> e);

    QMap<QString, UncertaintyComponent> getErrors() const;
    QMap<int, QString> getErrorOrder() const;

signals:

public slots:


private:
    QMap<int, QString> errorOrder;
    QMap<QString, UncertaintyComponent> errors;

    QStringList header;

};

#endif // SIMULATIONERRORTABLEMODEL_H
