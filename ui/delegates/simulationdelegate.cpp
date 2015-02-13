#include "simulationdelegate.h"

SimulationDelegate::SimulationDelegate(QObject * parent)
    : QItemDelegate(parent)
{
}

QWidget *SimulationDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLineEdit* value = new QLineEdit(parent);
    QLineEdit* uncertainty = new QLineEdit(parent);
    QComboBox* distributions = new QComboBox(parent);

    const SimulationErrorTableModel *myModel = static_cast<const SimulationErrorTableModel*>(index.model());

    if(myModel != NULL && index.column() == 2){
        return value;
    }
    if(myModel != NULL && index.column() == 3){
        return uncertainty;
    }
    if(myModel != NULL && index.column() == 4){
        QMap<QString,UncertaintyComponent> e = myModel->getErrors();
        QMap<int,QString> order = myModel->getErrorOrder();

        QString errorKey = order.value(index.row());
        UncertaintyComponent u = e.value(errorKey);

        distributions->addItems(u.distributions);

        return distributions;
    }

}

void SimulationDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    const SimulationErrorTableModel *myModel = static_cast<const SimulationErrorTableModel*>(index.model());

    if(myModel != NULL && index.column() == 2){
        QLineEdit* value = qobject_cast<QLineEdit*>(editor);
        if(value != NULL){
            value->setText(index.data().toString());
        }
    }

    if(myModel != NULL && index.column() == 3){
        QLineEdit* uncertainty = qobject_cast<QLineEdit*>(editor);
        if(uncertainty != NULL){
            uncertainty->setText(index.data().toString());
        }
    }

    if(myModel != NULL && index.column() == 4){

        QComboBox* distribution = qobject_cast<QComboBox*>(editor);
        if(distribution != NULL){

             QString distr = index.data().toString();

             distribution->setCurrentText(distr);

        }

    }


}

void SimulationDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    const SimulationErrorTableModel *myModel = static_cast<const SimulationErrorTableModel*>(index.model());

    if(myModel != NULL && index.column() == 2){

        QLineEdit* value = qobject_cast<QLineEdit*>(editor);
        if(value != NULL){
            QString v = value->text();
                model->setData(index,v);
            return;
        }
    }

    if(myModel != NULL && index.column() == 3){

        QLineEdit* uncertainty = qobject_cast<QLineEdit*>(editor);
        if(uncertainty != NULL){
            QString u = uncertainty->text();
                model->setData(index,u);
            return;
        }
    }

    if(myModel != NULL && index.column() == 4){

        QComboBox* distribution = qobject_cast<QComboBox*>(editor);
        if(distribution != NULL){
            QString distr = distribution->currentText();
                model->setData(index,distr);
            return;
        }
    }

}
