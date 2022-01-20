#include "observationtabledelegate.h"

/*!
 * \brief ObservationTableDelegate::ObservationTableDelegate
 * \param parent
 */
ObservationTableDelegate::ObservationTableDelegate(QObject * parent)
    : QItemDelegate(parent)
{
}

/*!
 * \brief ObservationTableDelegate::createEditor
 * Create the editor widget
 * \param parent
 * \param option
 * \param index
 */
QWidget* ObservationTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const{

    //create line edit
    QLineEdit* editor = NULL;

    return editor;

}

/*!
 * \brief ObservationTableDelegate::setEditorData
 * Set current input of editor widget
 * \param editor
 * \param index
 */
void ObservationTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{

}

/*!
 * \brief ObservationTableDelegate::setModelData
 * Commit the edited input of the editor widget back to the model
 * \param editor
 * \param model
 * \param index
 */
void ObservationTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{

}
