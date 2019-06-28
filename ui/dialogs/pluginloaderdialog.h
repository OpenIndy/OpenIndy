#ifndef PLUGINLOADERDIALOG_H
#define PLUGINLOADERDIALOG_H

#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QShowEvent>
#include <QDesktopWidget>

#include "pluginloader.h"

using namespace oi;

namespace Ui {
class PluginLoaderDialog;
}

/*!
 * \brief The PluginLoaderDialog class
 */
class PluginLoaderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PluginLoaderDialog(QWidget *parent = 0);
    ~PluginLoaderDialog();

signals:

    //#############
    //send messages
    //#############

    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);

private slots:

    //#########################
    //actions triggered by user
    //#########################

    void on_toolButton_Path_clicked();
    void on_pushButton_Cancel_clicked();
    void on_pushButton_Ok_clicked();

    //#######################
    //task finished callbacks
    //#######################

    void importFinished(const bool &success);

private:
    Ui::PluginLoaderDialog *ui;

};

#endif // PLUGINLOADERDIALOG_H
