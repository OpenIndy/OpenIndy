#ifndef SIMULATIONDELEGATE_H
#define SIMULATIONDELEGATE_H

#include <QItemDelegate>
#include <QLineEdit>
#include <QComboBox>

#include "simulationerrortablemodel.h"

class SimulationDelegate : public QItemDelegate
{
public:
    SimulationDelegate(QObject * parent = 0);

    QWidget* createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    void setEditorData(QWidget * editor, const QModelIndex & index) const;
    void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;
};

#endif // SIMULATIONDELEGATE_H
