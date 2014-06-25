#include "simulationerrortablemodel.h"

SimulationErrorTableModel::SimulationErrorTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    header.append("name");
    header.append("unit");
    header.append("value");
    header.append("uncertainty");
    header.append("distribution");
    header.append("discription");

}

int SimulationErrorTableModel::rowCount(const QModelIndex &parent) const
{
    return 1;
}

int SimulationErrorTableModel::columnCount(const QModelIndex &parent) const
{
    return header.size();
}

QVariant SimulationErrorTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    QStringList m_columns = header;

    if((Qt::DisplayRole == role) &&
            (Qt::Horizontal == orientation) &&
            (0 <= section) &&
            (section < columnCount())){

        return m_columns.at(section);
    }
    return QVariant();

}

QVariant SimulationErrorTableModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

bool SimulationErrorTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}

void SimulationErrorTableModel::setErrors(QMap<QString, UncertaintyComponent> e)
{
    this->errors = e;
}

QMap<QString, UncertaintyComponent> SimulationErrorTableModel::getErrors()
{
    return this->errors;
}
