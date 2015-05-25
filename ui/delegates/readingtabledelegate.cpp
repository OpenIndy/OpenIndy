#include "readingtabledelegate.h"

/*!
 * \brief ReadingTableDelegate::ReadingTableDelegate
 * \param parent
 */
ReadingTableDelegate::ReadingTableDelegate(QObject * parent)
    : QItemDelegate(parent)
{
}

/*!
 * \brief ReadingTableDelegate::createEditor
 * Create the editor widget
 * \param parent
 * \param option
 * \param index
 */
QWidget* ReadingTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const{

    //create line edit
    QLineEdit* editor = NULL;

    return editor;

}

/*!
 * \brief ReadingTableDelegate::setEditorData
 * Set current input of editor widget
 * \param editor
 * \param index
 */
void ReadingTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{

}

/*!
 * \brief ReadingTableDelegate::setModelData
 * Commit the edited input of the editor widget back to the model
 * \param editor
 * \param model
 * \param index
 */
void ReadingTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{

}

/*!
 * \brief ReadingTableDelegate::sizeHint
 * \param option
 * \param index
 * \return
 */
QSize ReadingTableDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const{

    //check model index
    if(!index.isValid()){
        return QSize(5, 22);
    }

    //get and check models
    const ReadingProxyModel *readingProxyModel = static_cast<const ReadingProxyModel*>(index.model());
    if(readingProxyModel == NULL){
        return QSize(5, 22);
    }

    QString input = readingProxyModel->data(index).toString();

    QFontMetrics metrics(option.font);

    int width = metrics.width(input) + option.decorationSize.width();

    return QSize(width, 22);

}
