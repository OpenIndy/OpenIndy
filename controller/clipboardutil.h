#ifndef CLIPBOARDUTIL_H
#define CLIPBOARDUTIL_H

#include <QModelIndexList>
#include <QModelIndex>
#include <QString>
#include <QDebug>
#include <QClipboard>
#include <QApplication>
#include <QPointer>
#include <QItemSelectionModel>

#include "types.h"

using namespace oi;

class ClipBoardUtil: public QObject
{
    Q_OBJECT
public:
    //ClipBoardUtil();

    // copy displayed values
    void copySelectionToClipBoard(QPointer<QAbstractItemModel> model,QPointer<QItemSelectionModel> selectionModel);
    // copy text to clipboard
    void copyToClipBoard(QString coyp_table);


signals:
    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage) const;
};

#endif // CLIPBOARDUTIL_H
