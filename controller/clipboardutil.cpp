#include "clipboardutil.h"

void ClipBoardUtil::copyToClipBoard(QPointer<QAbstractItemModel> model, QPointer<QItemSelectionModel> selectionModel) {
    if(model.isNull()) {
        emit sendMessage("copyToClipboard: model is null", MessageTypes::eWarningMessage);
        return;
    }
    if(selectionModel.isNull()) {
        emit sendMessage("copyToClipboard: selectionModel is null", MessageTypes::eWarningMessage);
        return;
    }

    QModelIndexList selection = selectionModel->selectedIndexes();

    //check and sort selection
    if(selection.size() <= 0){
        emit sendMessage("copyToClipboard: selection is empty", MessageTypes::eWarningMessage);
        return;
    }
    qSort(selection);

    QString copy_table;
    // loop over all selected rows and columns
    QModelIndex previousRow = selection.first();
    foreach(QModelIndex index, selection) {
        if (index.row() > previousRow.row()){  // new line
            previousRow = index;
            copy_table.append("\n");
        } else if(index != selection.first()){ // new column
            copy_table.append("\t");
        }

        QVariant data = model->data(index);
        if(data.isValid()) {
            copy_table.append(data.toString());
        }

        if (index == selection.last()) { // last selected field
            copy_table.append("\n");
        }
    }

    //set values to clipboard, so you can paste them elsewhere
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->clear();

    if(copy_table.isEmpty()) {
        emit sendMessage("copy to clipboard failed", MessageTypes::eWarningMessage);
    } else {
        clipboard->setText(copy_table);

        emit sendMessage("successfully copied to clipboard", MessageTypes::eInformationMessage);
    }
}
