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
    return this->errors.size();
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
    if(!index.isValid())
        return QVariant();

    QString uncertaintyKey = this->errorOrder.value(index.row());
    UncertaintyComponent u = this->errors.value(uncertaintyKey);

    if(Qt::DisplayRole == role){
        switch (index.column()) {
        case 0:
            return u.name;
        case 1:
            return u.errorUnit;
        case 2:
            return u.value;
        case 3:
            return u.uncertainty;
        case 4:
             return u.distribution;
        case 5:
             return u.description;
        default:
            return QVariant();
        }
    }

    return QVariant();
}

bool SimulationErrorTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QString uncertaintyKey = this->errorOrder.value(index.row());

    UncertaintyComponent u = this->errors.value(uncertaintyKey);

    switch (index.column()) {
    case 2:
        u.value = value.toDouble();
        this->errors.insert(uncertaintyKey,u);
        break;
    case 3:
        u.uncertainty = value.toDouble();
        this->errors.insert(uncertaintyKey,u);
        break;
    case 4:
        u.distribution = value.toString();
        this->errors.insert(uncertaintyKey,u);
        break;
    default:
        break;
    }
    return false;
}

Qt::ItemFlags SimulationErrorTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags myFlags = QAbstractTableModel::flags(index);
    return (myFlags | Qt::ItemIsEditable);
}

void SimulationErrorTableModel::setErrors(QMap<QString, UncertaintyComponent> e)
{

    this->errors.clear();

    this->errors = e;

    int index = 0;

    this->errorOrder.clear();

    QMapIterator<QString, UncertaintyComponent > j(this->errors);
    while (j.hasNext()) {
        j.next();

        this->errorOrder.insert(index,j.key());

        index += 1;

    }

    emit layoutAboutToBeChanged();
    emit layoutChanged();
}

QMap<QString, UncertaintyComponent> SimulationErrorTableModel::getErrors() const
{
    return this->errors;
}

QMap<int, QString> SimulationErrorTableModel::getErrorOrder() const
{
    return this->errorOrder;
}
