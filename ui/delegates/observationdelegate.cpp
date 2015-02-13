#include "observationdelegate.h"

ObservationDelegate::ObservationDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

/*!
 * \brief createEditor adds the gui element to the view.
 * \param parent
 * \param option
 * \param index
 * \return
 */
QWidget *ObservationDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    /*QCheckBox* cBox;
    cBox = new QCheckBox(parent);

    const ObservationProxyModel *myModel = static_cast<const ObservationProxyModel*>(index.model());
    if(myModel != NULL && (myModel->mapToSource(index).column() == 11)){
        return cBox;
    }
    return NULL;*/

    QComboBox *cbBox;
    cbBox = new QComboBox(parent);

    const ObservationProxyModel *myModel = static_cast<const ObservationProxyModel*>(index.model());
    if(myModel != NULL && (myModel->mapToSource(index).column() == 11)){
        cbBox->addItem("true");
        cbBox->addItem("false");
        return cbBox;
    }
    return NULL;
}

/*!
 * \brief setEditorData sets current input of editor widget
 * \param editor
 * \param index
 */
void ObservationDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    /*const ObservationProxyModel *myModel = static_cast<const ObservationProxyModel*>(index.model());

    if(myModel != NULL && (myModel->mapToSource(index).column() == 11)){
        QCheckBox* cBox = qobject_cast<QCheckBox*>(editor);
        if(cBox != NULL){
            cBox->setChecked(index.data().toBool());
        }
    }*/

    const ObservationProxyModel *myModel = static_cast<const ObservationProxyModel*>(index.model());

    if(myModel != NULL && (myModel->mapToSource(index).column() == 11)){
        QComboBox* cbBox = qobject_cast<QComboBox*>(editor);
        if(cbBox != NULL){
            bool state = index.data().toBool();
            if(state){
                cbBox->setCurrentText("true");
            }else{
                cbBox->setCurrentText("false");
            }
        }
    }
}

/*!
 * \brief setModelData
 * Commit the edited input of the editor widget back to the model
 * \param editor
 * \param model
 * \param index
 */
void ObservationDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    /*const ObservationProxyModel *myModel = static_cast<const ObservationProxyModel*>(index.model());
    if(myModel != NULL && (myModel->mapToSource(index).column() == 11)){
        QCheckBox* cBox = qobject_cast<QCheckBox*>(editor);
        if(cBox != NULL){
            model->setData(index, cBox->isChecked());
            return;
        }
    }
    return;*/

    const ObservationProxyModel *myModel = static_cast<const ObservationProxyModel*>(index.model());

    if(myModel != NULL && (myModel->mapToSource(index).column() == 11)){
        QComboBox *cbBox = qobject_cast<QComboBox*>(editor);
        if(cbBox != NULL){
            QString state = cbBox->currentText();
            if(state.compare("true") == 0){
                model->setData(index,true);
            }else{
                model->setData(index,false);
            }
        }
    }
}
